#ifndef __BOX_H__
#define __BOX_H__

class Box : public Geometry
{
public:
  Box(float radius)
  {
    float box[8][3] = 
    { //front
      {-radius, -radius, -radius},
      { radius, -radius, -radius},
      { radius,  radius, -radius},
      {-radius,  radius, -radius},
      
      //back
      {-radius, -radius, radius},
      { radius, -radius, radius},
      { radius,  radius, radius},
      {-radius,  radius, radius}};
    
    *this << Quad(box[0], box[3], box[2], box[1]) //front
          << Quad(box[1], box[2], box[6], box[5]) //right
          << Quad(box[4], box[5], box[6], box[7]) //back
          << Quad(box[3], box[0], box[4], box[7]); //left
  }

  Box(float* position, float radius)
  {
    float box[8][3] = 
    { //front
      {-radius, -radius, -radius},
      { radius, -radius, -radius},
      { radius,  radius, -radius},
      {-radius,  radius, -radius},
      
      //back
      {-radius, -radius, radius},
      { radius, -radius, radius},
      { radius,  radius, radius},
      {-radius,  radius, radius}};
    
    Vector3 p = position;
    *this << Quad(p+box[0], p+box[3], p+box[2], p+box[1]) //front
          << Quad(p+box[1], p+box[2], p+box[6], p+box[5]) //right
          << Quad(p+box[4], p+box[5], p+box[6], p+box[7]) //back
          << Quad(p+box[3], p+box[0], p+box[4], p+box[7]) //left
          << Quad(p+box[2], p+box[3], p+box[7], p+box[6]) //top
          << Quad(p+box[1], p+box[0], p+box[4], p+box[5]); //bottom
  }
};

#endif
