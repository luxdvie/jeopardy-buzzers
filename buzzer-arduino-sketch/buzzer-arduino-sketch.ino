/*
  buzzer-arudino-sketch.ino
 
  Handles the click of 1-4 buttons in a Jeopardy / Family Feud style format.
 
  Handle the click of a button and illuminate that button's LED's if it's the first one pressed.
  Supports up to (4) buttons.
  Originally developed on an Arduino Uno and the Adafruit "Music Maker" MP3 Shield (VS1053)

  @author       Austin Brown <austinbrown2500@gmail.com>
  @description  Jeopardy Buzzer / Family Feud Buzzer
  @date         June 2019
  @license      WTFPL (https://en.wikipedia.org/wiki/WTFPL)
  @requirements BuzzerPlayer, Adafruit_VS1053, SPI, SD
*/

/* Includes */
  #include <BuzzerPlayer.h>
  #include <SPI.h>
  #include <Adafruit_VS1053.h>
  #include <SD.h>

/* Adafruit "Music Maker" MP3 Shield (VS1053) Pins */
  #define SHIELD_RESET  -1  // VS1053 reset pin (unused!)
  #define SHIELD_CS      7  // VS1053 chip select pin (output)
  #define SHIELD_DCS     6  // VS1053 Data/command select pin (output)
  #define CARDCS         4  // Card chip select pin
  #define DREQ           3  // VS1053 Data request, DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt

/* Globals */
  Adafruit_VS1053_FilePlayer buzzerShield = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  const int CheckPressedDelay  = 10; /* The delay in the event loop iteration */
  
/* Master Button Setup */
  /* Not yet fully implemented */
  const int MasterButtonInput  = A2; /* Analog Input #2 on the Uno */
  const int MasterButtonOutput = 10; /* Digital I/O #10 on the Uno */

/* PlayerOne Setup */
  const int  PlayerOneInput        = 5;     /* Digital I/O #5 on the VS1053 */
  const bool PlayerOneInputShield  = true;  /* We are using the shield for the input for PlayerOne */
  const bool PlayerOneInputAnalog  = false; /* We are not using an analog input for PlayerOne */
  
  const int  PlayerOneOutput       = 1;     /* Digital I/O #1 on the VS1053 */
  const bool PlayerOneOutputShield = true;  /* We are using the shield for the output for PlayerOne */

/* PlayerTwo Setup */
  const int  PlayerTwoInput        = 6;     /* Digital I/O #6 on the VS1053 */
  const bool PlayerTwoInputShield  = true;  /* We are using the shield for the input for PlayerTwo */
  const bool PlayerTwoInputAnalog  = false; /* We are not using an analog input for PlayerTwo */
  
  const int  PlayerTwoOutput       = 2;     /* Digital I/O #2 on the VS1053 */
  const bool PlayerTwoOutputShield = true;  /* We are using the shield for the output for PlayerTwo */
  
/* PlayerThree Setup */
  const int  PlayerThreeInput        = 7;     /* Digital I/O #7 on the VS1053 */
  const bool PlayerThreeInputShield  = true;  /* We are using the shield for the input for PlayerThree */
  const bool PlayerThreeInputAnalog  = false; /* We are not using an analog input for PlayerThree */
  
  const int  PlayerThreeOutput       = 3;     /* Digital I/O #3 on the VS1053 */
  const bool PlayerThreeOutputShield = true;  /* We are using the shield for the output for PlayerThree */
  
/* PlayerFour Setup */
  const int  PlayerFourInput        = A5;     /* Analog I/O #5 on the arduino */
  const bool PlayerFourInputShield  = false;  /* We are using the shield for the input for PlayerFour */
  const bool PlayerFourInputAnalog  = true;   /* We are not using an analog input for PlayerFour */
  
  const int  PlayerFourOutput       = 4;      /* Digital I/O #4 on the VS1053 */
  const bool PlayerFourOutputShield = true;   /* We are using the shield for the output for PlayerFour */
 
/* PlayerList */
  const int PlayerCount = 4;
  BuzzerPlayer PlayerList[PlayerCount] = {
    BuzzerPlayer(buzzerShield, PlayerOneInput,   PlayerOneInputShield,   PlayerOneInputAnalog,   PlayerOneOutput,   PlayerOneOutputShield),
    BuzzerPlayer(buzzerShield, PlayerTwoInput,   PlayerTwoInputShield,   PlayerTwoInputAnalog,   PlayerTwoOutput,   PlayerTwoOutputShield),
    BuzzerPlayer(buzzerShield, PlayerThreeInput, PlayerThreeInputShield, PlayerThreeInputAnalog, PlayerThreeOutput, PlayerThreeOutputShield),
    BuzzerPlayer(buzzerShield, PlayerFourInput,  PlayerFourInputShield,  PlayerFourInputAnalog,  PlayerFourOutput,  PlayerFourOutputShield)
  };

/* Game Operation */
  bool State_Won      = false; /* Whether our round is in the "Won" state, meaning we're awaiting a reset */
  int  RoundResetTime = 2500;  /* The number of milliseconds to wait after resetting a round, before the next one begins */
  
/**
 * This method runs once upon boot and configures the environment.
 */
void setup() {
  Serial.begin(9600);
  Serial.println("Jeopary / Family Feud Buzzer Game");

  /* Disable the SD card on the shield */
  pinMode(CARDCS, OUTPUT);
  digitalWrite(CARDCS, HIGH);  
  
  /* Initialize the shield */
  if (!buzzerShield.begin()) {
    Serial.println("Critical Error");
    Serial.println("Adafruit Music Maker Shield (VS1053) not found. Cannot continue.");
    while (1);  // don't do anything more
  } else {
      Serial.println("Initalize Completed Successfully");  
      BlinkAll(2, 200);
  }
}

/**
 * This method executes with each iteration of the event loop.
 */
void loop() {   
    if (!State_Won) { /* If we aren't showing someone as a winner, and no one has won yet, check to see if any buttons have been pressed */
      for(int x = 0; x < PlayerCount; x++) {
          bool playerPressed = PlayerList[x].CheckPressed(buzzerShield);
          if (playerPressed) {
            Serial.print("Player ");
            Serial.print(x+1);
            Serial.println(" has won this round!");
            PlayerList[x].Flash(buzzerShield); /* Flash the winning button for 5 seconds */
            State_Won = true;
            Serial.println("Awaiting round reset.");
            break;
          } else {
            PlayerList[x].StopFlash(buzzerShield);
          }
      }
    } else if (State_Won) { /* If we are in the "won" state, that means any button press will reset the round */
      for(int x = 0; x < PlayerCount; x++) {
          bool playerPressed = PlayerList[x].CheckPressed(buzzerShield);
          if (playerPressed) {
            StopAllFlashing();
            State_Won = false;
            Serial.println("The round has been reset. Please wait for the next round to begin.");
            delay(RoundResetTime); // wait 5 seconds to ensure that the person doesn't "win" the round again immediately
            Serial.println("The next round is beginning.");
            BlinkAll(1, 200);
            break;
          }
      }      
    }
    
    delay(CheckPressedDelay);
}

/**
 * Blink all the lights the given number of times with the given frequency
 */
void BlinkAll(int times, int blinkDelay) {
  StopAllFlashing();
  bool lightEnabled = false;
  
  for (int n = 0; n < times; n++) {
    if (lightEnabled) {
      StopAllFlashing(); // turn off lights
    } else {
      for(int x = 0; x < PlayerCount; x++){ // turn on lights
        PlayerList[x].TurnOn(buzzerShield);
      }
    }

    lightEnabled = !lightEnabled;
    delay(blinkDelay);    
  }           
}

/**
 * Stop the flashing of any player
 */
void StopAllFlashing() {
  for(int x = 0; x < PlayerCount; x++){
    PlayerList[x].StopFlash(buzzerShield);
  }          
}
