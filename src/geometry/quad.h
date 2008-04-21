#ifndef __QUAD_H__
#define __QUAD_H__

class Quad : public Geometry
{
public:
  Quad(float* v1, float* v2, float* v3, float* v4)
  {
    *this << Tri(v1, v2, v4);
    *this << Tri(v2, v3, v4);
  }

  Quad(float* v)
  {
    *this << Tri(v);
    *this << Tri(v+9);
  }
};

#endif
