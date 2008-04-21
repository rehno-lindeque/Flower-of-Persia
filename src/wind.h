#ifndef __WIND_H__
#define __WIND_H__

#include "modifier.h"

extern class Wind : public Modifier
{
public:
  Vector3 velocity;
  Vector3 acceleration;
  float MAX_VELOCITY, MAX_ACCELERATION;
  bool enabled;

  Wind() : enabled(true)
  {
    MAX_VELOCITY = 5.0f;
    MAX_ACCELERATION = 1.0f;
  }

  Vector3 apply(const Vector3& source)
  {

    //apply (with slight random value?)
    //(perhaps slightly randomized by position, so that far off vertices will be blown in a slightly different direction)
    if(enabled)
      return velocity + 5.0f*Vector3(Random::getFloat(-1.0f, 1.0f), Random::getFloat(-1.0f, 1.0f), Random::getFloat(-1.0f, 1.0f));
    else
      return Vector3(0.0f, 0.0f, 0.0f);
  }

  void update()
  {
    acceleration = Vector3(Random::getFloat(-1.0f, 1.0f), Random::getFloat(-0.4f, 0.4f), Random::getFloat(-1.0f, 1.0f));
    if(acceleration.getLength() != 0.0f)
      acceleration.normalize() *= MAX_ACCELERATION;
    velocity += acceleration;
    if(velocity.getLength() > MAX_VELOCITY)
    {
      velocity *= MAX_VELOCITY/velocity.getLength();
      //scale to max
      // remember max.y must be less than max.x and max.z because wind doesn't generally blow up or down
    }
  }
} wind;

#endif
