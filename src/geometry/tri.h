#ifndef __TRI_H__
#define __TRI_H__

class Tri : public Geometry
{
public:
  Tri(float* v1, float* v2, float* v3)
  {
    coordinates << Stream<float>(v1, 3);
    coordinates << Stream<float>(v2, 3);
    coordinates << Stream<float>(v3, 3);
  }

  Tri(float* v)
  {
    coordinates << Stream<float>(v, 9);
  }
};

#endif
