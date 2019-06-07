# jeopardy-buzzers

Jeopardy / Family Feud Buzzer game. Works with up to 4 players. Built on `Arduino Uno` and `Adafruit Music Maker MP3 Shield (VS1053)`. 

## Requirements

**C++ Classes**
-  `BuzzerPlayer` C++ class
-  `Adafruit_VS1053` class for the shield
-  `SPI` and `SD` classes

**Hardware**
-  Standard switch per player. I used large arcade-style buttons
-  LED or LED Strip per player (controlled via out GPIO)
-  Electrical components such as: resistors, diodes, transistors, etc. 
    -  Used `TIP120` in this project

## Setup

Make sure you put the two libraries, `BuzzerPlayer` and `Adafruit_VS1053_Library-master` in your library location for whatever IDE you're using to flash the Arduino.

## License

WTFPL (https://en.wikipedia.org/wiki/WTFPL)

## Author

Austin Brown (austinbrown2500@gmail.com)