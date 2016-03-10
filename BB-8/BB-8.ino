#include <OSCMessage.h>

#include <SLIPEncodedSerial.h>
SLIPEncodedSerial SLIPSerial(Serial1);

//#define DEBUG

String MAC;
int index = 0;
uint8_t UDPBuffer[50];
OSCMessage msgIN;

int ESPwait(String stopstr, int timeout_secs)
{
  String response;
  bool found = false;
  char c;
  long timer_init;
  long timer;

  timer_init = millis();
  while (!found) {
    timer = millis();
    if (((timer - timer_init) / 1000) > timeout_secs) { // Timeout?
      Serial.println("!Timeout!");
      return 0;  // timeout
    }
    if (SLIPSerial.available()) {
      c = SLIPSerial.read();
      Serial.print(c);
      response += c;
      if (response.endsWith(stopstr)) {
        found = true;
        delay(10);
        SLIPSerial.flush();
        Serial.println();
      }
    } // end SLIPSerial.available()
  } // end while (!found)
  return 1;
}

int ESPsendCommand(char *command, String stopstr, int timeout_secs)
{
  SLIPSerial.println(command);
  ESPwait(stopstr, timeout_secs);
  delay(250);
}

void setup(void)
{
  Serial.begin(115200);
  while (!Serial) {}
  Serial.print("setup begin\r\n");

  SLIPSerial.begin(115200);
  // With the new ESP8266 WIFI MODULE WE NEED TO MAKE AN INITIALIZATION PROCESS
  Serial.println("Initalizing ESP Wifi Module...");
  Serial.println("WIFI RESET");
  SLIPSerial.flush();
  SLIPSerial.print("+++");  // To ensure we exit the transparent transmision mode
  delay(100);
  ESPsendCommand("AT", "OK", 1);
  ESPsendCommand("AT+RST", "OK", 2); // ESP Wifi module RESET
  ESPwait("ready", 6);
  ESPsendCommand("AT+GMR", "OK", 5);
  SLIPSerial.println("AT+CIPSTAMAC?");
  delay(250);
  ESPsendCommand("AT+CWQAP", "OK", 3);
  ESPsendCommand("AT+CWMODE=2", "OK", 3); // Soft AP mode
  // Generate Soft AP. SSID=JJROBOTS, PASS=87654321
  char *cmd =  "AT+CWSAP=\"BB8\",\"87654321\",5,3";
  ESPsendCommand(cmd, "OK", 6);
  // Start UDP SERVER on port 2222
  Serial.println("Start UDP server at port 2222");
  ESPsendCommand("AT+CIPMUX=0", "OK", 3);  // Single connection mode
  ESPsendCommand("AT+CIPMODE=1", "OK", 3); // Transparent mode
  //ESPsendCommand("AT+CIPSTART=\"UDP\",\"0\",2223,2222,0", "OK", 3);
  ESPsendCommand("AT+CIPSTART=\"UDP\",\"192.168.4.2\",2223,2222,0", "OK", 3);
  delay(250);
  ESPsendCommand("AT+CIPSEND", ">", 2); // Start transmission (transparent mode)

  Serial.print("setup end\r\n");
  Serial.println("I am online and waiting commands ...");
}

void loop() {
  int size;
  int n;

  if ( (size = SLIPSerial.available()) > 0)
  {
    char c = SLIPSerial.read();
    n = SLIPSerial.peek();
#ifdef DEBUG
    Serial.print(c);
    Serial.print(" - ");
    Serial.print(n);
    Serial.print(" - ");
    Serial.println(index);
#endif
    delayMicroseconds(50);
    if (c == '/' && (n == 255 || n == 49)) {
      Serial.println("New command arrived");
      index = 0;
      msgIN.empty();
    }
    if (c == '\0') {
      msgIN.fill('\0');
      UDPBuffer[index] = c;
    } else {
      msgIN.fill(c);
      UDPBuffer[index] = c;
    }
    index++;
  }

  if (n == 255 && UDPBuffer[0] == '/' && UDPBuffer[2] == '/') {
    Serial.print("Routing ");
    Serial.println((const char *) UDPBuffer);
    if (!msgIN.hasError()) {
      msgIN.route("/1", routePage1);
    }
    for (int i = 0; i < sizeof(UDPBuffer); i++) {
      UDPBuffer[i] = '\0';
    }
  }
}

void routePage1(OSCMessage &msg, int patternOffset) {
  //Serial.println("Match page 1");
  msg.route("/throttle", routeP1Throttle, patternOffset);
  msg.route("/steering", routeP1Steering, patternOffset);
  msg.route("/dome", routeP1Dome, patternOffset);

  msg.route("/push1", routeP1Action1, patternOffset);
  msg.route("/push2", routeP1Action2, patternOffset);
  msg.route("/push3", routeP1Action3, patternOffset);
  msg.route("/push4", routeP1Action4, patternOffset);
}

void routeP1Throttle(OSCMessage &msg, int addressOffset) {
  float val = msg.getFloat(0);
  if (val > 0) {
    Serial.println("Match: /1/throttle");
    Serial.println(val);
  }
}
void routeP1Steering(OSCMessage &msg, int addressOffset) {
  float val = msg.getFloat(0);
  if (val > 0) {
    Serial.println("Match: /1/steering");
    Serial.println(val);
  }
}
void routeP1Dome(OSCMessage &msg, int addressOffset) {
  float val = msg.getFloat(0);
  if (val > 0) {
    Serial.println("Match: /1/dome");
    Serial.println(val);
  }
}
void routeP1Action1(OSCMessage &msg, int addressOffset) {
  Serial.println("Match: /1/push1");
}
void routeP1Action2(OSCMessage &msg, int addressOffset) {
  Serial.println("Match: /1/push2");
}
void routeP1Action3(OSCMessage &msg, int addressOffset) {
  Serial.println("Match: /1/action3");
}
void routeP1Action4(OSCMessage &msg, int addressOffset) {
  Serial.println("Match: /1/action4");
}

