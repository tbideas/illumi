/*
 * IllumiWiflyDelegate.cpp: Wifly delegate
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */

#include "IllumiWiflyDelegate.h"

IllumiWiflyDelegate::IllumiWiflyDelegate(ColorLED &led) : led(led)
{
  
}

boolean IllumiWiflyDelegate::initializationProgress(int status)
{
  if (status == WIFLY_RESETTING) {
    // red
    led.setColor(100, 0, 0);
  }
  else if (status == WIFLY_JOINING) {
    // green
    led.setColor(0, 100, 0);
  }
  else if (status == WIFLY_JOINED) {
    // blue
    led.setColor(0, 0, 100);
  }
    
  return true;
}
