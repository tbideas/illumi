/*
 * IllumiWiflyDelegate.h: Wifly delegate
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */


#ifndef __ILLUMI_WIFLY_DELEGATE_H__
#define __ILLUMI_WIFLY_DELEGATE_H__

#include "Arduino.h"
#include "Wifly.h"
#include "ColorLED.h"

class IllumiWiflyDelegate : public WiflyDelegate
{
  ColorLED &led;
  
  public:
  IllumiWiflyDelegate(ColorLED &led);
  boolean initializationProgress(int status);
};

#endif
