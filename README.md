# BB-8 ESP8266/TouchOSC/Arduino WiFi Robot Remote Control

## Use your existing smartphone/tablet to control a a robot like BB-8 with TouchOSC.

This example is a demo to control the Star Wars BB-8 lifesize robot that I am building but can be adapted to control any robot.

### Connect ESP8266 to Leonardo
![Alt text](/ESP8266/ESP8266 Leonardo.png?raw=true "Connect ESP8266 to Leonardo")

### ESP8266 Pin Map
![Alt text](/ESP8266/ESP8266 Pin Map.png?raw=true "ESP8266 Pin Map")

### TouchOSC Interface
![Alt text](/TouchOSC/preview.png?raw=true "TouchOSC Interface")

### OSC Connection Settings
![Alt text](/TouchOSC/setup.png?raw=true "OSC Connection Settings")

This was designed using TouchOSC app as remote control. This is a not a free app but is cheap and works great! It´s available for IOS and Android.

TouchOSC has a layout editor for PC. Download and install from http://hexler.net/software/touchosc (go down to the download section).
Open the layout BB-8.touchosc on the editor and follow the instructions to send the layout to your smartphone/table using your WiFi network.

The code for setting up the AP from B-ROBOT https://github.com/jjrobots/B-ROBOT

Tested on Arduino Leonardo/Micro.
