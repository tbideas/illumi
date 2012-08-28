/*
 * Color.h
 * Part of the Loochi project: http://www.tbideas.com/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */

#include "Color.h"

Color::Color() : red(0), green(0), blue(0)
{
}

Color::Color(unsigned long red, unsigned long green, unsigned long blue) : red(red), green(green), blue(blue)
{
}

Color Color::interpolateColor(const unsigned long p, const unsigned long posA, const unsigned long posB, const Color A,  const Color B)
{
  Color C;
  
  C.red    = map(p, posA, posB, A.red, B.red);
  C.green  = map(p, posA, posB, A.green, B.green);
  C.blue   = map(p, posA, posB, A.blue, B.blue);
  
  return C;
}
