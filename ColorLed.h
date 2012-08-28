/*
 * ColorLED.h
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */


#ifndef __COLORLED_H__
#define __COLORLED_H__

#include "Arduino.h"
#include "Color.h"

class ColorLED
{
  int redPin, greenPin, bluePin;
  boolean commonAnode;
  int shutdownPin;
  
  void setup();
  
  public:
    ColorLED(int redPin, int greenPin, int bluePin, boolean commonAnode);
    ColorLED(int redPin, int greenPin, int bluePin, boolean commonAnode, int shutdownPin);
    void setColor(int red, int green, int blue);
    void setColor(const Color c);
};

#endif
