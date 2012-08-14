/*
 * ColorLed.cpp
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */
 
#include "ColorLED.h"

void ColorLED::setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setColor(0, 0, 0);
}

ColorLED::ColorLED(int redPin, int greenPin, int bluePin, boolean commonAnode) : redPin(redPin), greenPin(greenPin), bluePin(bluePin), commonAnode(commonAnode), shutdownPin(0)
{
  setup();
}

ColorLED::ColorLED(int redPin, int greenPin, int bluePin, boolean commonAnode, int shutdownPin) : redPin(redPin), greenPin(greenPin), bluePin(bluePin), commonAnode(commonAnode), shutdownPin(shutdownPin)
{
  setup();
  pinMode(shutdownPin, OUTPUT);
}

void ColorLED::setColor(int red, int green, int blue)
{

  if (commonAnode) {
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  }
  else {
    analogWrite(redPin, 255 - red);
    analogWrite(greenPin, 255 - green);
    analogWrite(bluePin, 255 - blue);
  }
  
  if (shutdownPin != 0) {
    if (!red && !green && !blue)
      digitalWrite(shutdownPin, LOW);
    else 
      // Enable the board (shutdown is enabled when the signal is LOW)
      digitalWrite(shutdownPin, HIGH);
  }
}

