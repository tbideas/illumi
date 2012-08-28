/*
 * Color.h
 * Part of the Loochi project: http://www.tbideas.com/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */


#ifndef __COLOR_H__
#define __COLOR_H__

#include "Arduino.h"

class Color
{
  
  public:
    Color();
    Color(unsigned long red, unsigned long green, unsigned long blue);
    unsigned long red;
    unsigned long green;
    unsigned long blue;
    
    /*
     * Returns the color at point p calculated by linear interpolation of the 
     * color A at point posA and the color B at point posB.
     */
    static Color interpolateColor(const unsigned long p, const unsigned long posA, const unsigned long posB, const Color A,  const Color B);
};

#endif
