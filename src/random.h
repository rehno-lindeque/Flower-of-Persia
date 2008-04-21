#ifndef __RANDOM_H__
#define __RANDOM_H__
#include <stdlib.h>

class Random
{
public:
  static float getFloat(float min = 0.0f, float max = 1.0f)
  {
    return rand()/(float)RAND_MAX*(max-min) + min;
  }

};

#endif
