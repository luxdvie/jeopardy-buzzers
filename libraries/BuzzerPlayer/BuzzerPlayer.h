/*
  BuzzerPlayer.h - Library for controlling a buzzer player.
  Created by Austin Brown, June 2019
*/
#ifndef BuzzerPlayer_h
#define BuzzerPlayer_h

#include "Arduino.h"
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

class BuzzerPlayer
{
  public:
    /**
     * Construct a new instance of a buzzer player
     */
    BuzzerPlayer(Adafruit_VS1053_FilePlayer& shield, int inputPin, bool useShieldInput, bool isAnalogInput, int outputPin, bool useShieldOutput);
    
    /**
     * Begin flashing this player's LED's
     */
    void Flash(Adafruit_VS1053_FilePlayer& shield);

    /**
     * Turn on this player's LED's
     */
    void TurnOn(Adafruit_VS1053_FilePlayer& shield);

    /**
     * Stop flashing this player's LED's
     */
    void StopFlash(Adafruit_VS1053_FilePlayer& shield);

    /**
     * Check if this player's button is pressed
     */
    bool CheckPressed(Adafruit_VS1053_FilePlayer& shield);
    
  private:
    /* The input pin for this player's button switch */
    int InputPin;
    
    /* Whether to use the VS1053 shield input, or the normal arduino analog or digital input */
    bool UseShieldInput;

    /* Whether the button switch input is connected to an analog input instead of a digital input */
    bool IsAnalogInput;

    /* The output pin for this player's LED's */
    int OutputPin;
    
    /* Whether to use the VS1053 shield input, or the normal arduino digital output */
    bool UseShieldOutput;   
};

#endif
