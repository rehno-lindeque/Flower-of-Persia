#ifndef __GRAVITY_H__
#define __GRAVITY_H__

#include "modifier.h"

extern class Gravity : public Modifier
{
public:
  const float GRAVITY_ACCEL;
  bool enabled;

  Gravity() : GRAVITY_ACCEL(90.8f), enabled(true) {}

  Vector3 apply(const Vector3& source)
  {
    // this is wrong: it needs mass to be accurate
    if(enabled)
      return Vector3(0.0f, -GRAVITY_ACCEL, 0.0f);
    else
      return Vector3(0.0f, 0.0f, 0.0f);
  }

  Vector3 apply(const Vector3& source, float mass)
  {
    //F = m*a
    if(enabled)
      return Vector3(0.0f, -GRAVITY_ACCEL*mass, 0.0f);
    else
      return Vector3(0.0f, 0.0f, 0.0f);
  }

  void update()
  {
  }
} gravity;

#endif
