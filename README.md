# rounddisplay_gc9a01
Code samples and projects for GC9A01 based round display.

#Getting started
To get an easy start, I recommend you to watch https://www.youtube.com/watch?v=Y0BGnHFuYBU&feature=emb_logo.

# Current samples
## Hello World
Based on the Arduino GFX samples the hello world. Tested with ESP32 Mini Dev Kit. Used standard SPI wiring.

## gc9a01_ImgViewerJpg
Based on the Arduino GFX samples to display a JPGEG stored in SPIFFS of ESP32 Mini Dev Kit. Used standard SPI wiring.

## Analog Watch
First edit the secrets and add your WiFi SSID and Password. After that you have to set up the correct time zones and there you go. The sketch connects with your local wifi then to a NTP server and after that displays an analog watch. The colors of the cursors, etc. can be defined. This was tested with ESP32 Mini Dev Kit and standard SPI wiring.
