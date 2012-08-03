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

ColorLED::ColorLED(int redPin, int greenPin, int bluePin, boolean commonAnode) : redPin(redPin), greenPin(greenPin), bluePin(bluePin), commonAnode(commonAnode)
{
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
}

