/*
  BuzzerPlayer.cpp - Library for controlling a buzzer player.
  Created by Austin Brown, June 2019
*/

#include "Arduino.h"
#include "BuzzerPlayer.h"
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

/**
 *  Create a new instance of a buzzer player and setup their pins
 */
BuzzerPlayer::BuzzerPlayer(Adafruit_VS1053_FilePlayer& shield, int inputPin, bool useShieldInput, bool isAnalogInput, int outputPin, bool useShieldOutput)
{ 
  /* Configure the input */
  InputPin = inputPin;
  UseShieldInput = useShieldInput;
  IsAnalogInput = isAnalogInput;

  if (IsAnalogInput) {
    /* Use arduino analog input */
    pinMode(InputPin, INPUT);
  } else if (UseShieldInput) {
    /* Use shield GPIO */    
    shield.GPIO_pinMode(InputPin, INPUT);
  } else {
    /* Use default arduino GPIO */
    pinMode(InputPin, INPUT);
  }

  /* Configure the output */
  OutputPin = outputPin;
  UseShieldOutput = useShieldOutput;

  if (UseShieldOutput) {
     shield.GPIO_pinMode(OutputPin, OUTPUT);
  } else {
    pinMode(OutputPin, OUTPUT);
  }
}

/**
 *  Flash this user's LED's for 5 seconds
 */
void BuzzerPlayer::Flash(Adafruit_VS1053_FilePlayer& shield)
{
  bool lightEnabled = false;
  
  for (int x = 0; x < 25; x++) {
    if (lightEnabled) {
      // turn off
      StopFlash(shield);
    } else {
      // turn on
      TurnOn(shield);
    }
    
    lightEnabled = !lightEnabled;
    delay(200);
  }

  // Always leave the LED's on after flashing, to continue indicating who won
  if (UseShieldOutput) {
    shield.GPIO_pinMode(OutputPin, OUTPUT);
    shield.GPIO_digitalWrite(OutputPin, HIGH);
  } else {
    digitalWrite(OutputPin, HIGH);
  }  
}

/**
 *  Turn on this player's LED's
 */
void BuzzerPlayer::TurnOn(Adafruit_VS1053_FilePlayer& shield)
{
  if (UseShieldOutput) {
    shield.GPIO_pinMode(OutputPin, OUTPUT);
    shield.GPIO_digitalWrite(OutputPin, HIGH);
  } else {
    digitalWrite(OutputPin, HIGH);
  }
}

/**
 *  Turn off this player's LED's
 */
void BuzzerPlayer::StopFlash(Adafruit_VS1053_FilePlayer& shield)
{
  if (UseShieldOutput) {
    shield.GPIO_pinMode(OutputPin, OUTPUT);
    shield.GPIO_digitalWrite(OutputPin, LOW);
  } else {
    digitalWrite(OutputPin, LOW);
  }
}

/**
 *  Check if this player's button is pressed.
 */
bool BuzzerPlayer::CheckPressed(Adafruit_VS1053_FilePlayer& shield) 
{ 
  if (UseShieldInput) {
      shield.GPIO_pinMode(InputPin, INPUT);
     return shield.GPIO_digitalRead(InputPin);
  } else if (IsAnalogInput) {
    if (analogRead(InputPin) >= 900) { /* AnalogRead can fluctuate. With a pulldown resistor, this should never be above 900 unless pressed by the player */
      return true;
    } else {
      return false;
    }
  } else {
    return digitalRead(InputPin);
  }
}

