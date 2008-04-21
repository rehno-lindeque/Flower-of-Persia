#ifndef __VIEWCUBE_H__
#define __VIEWCUBE_H__

/* This is a (temporary) simple cube for testing both render-to-texture and cubemappping
*/

class ViewCube : public Model
{
protected:
  void drawQuad(float* point1, float* point2, float* point3, float* point4)
  {
    glVertex3fv(point1);
    glVertex3fv(point2);
    glVertex3fv(point3);
    glVertex3fv(point4);
  }
  void drawBox(float* position, float radius)
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
    drawQuad(p+box[0], p+box[3], p+box[2], p+box[1]); //front
    drawQuad(p+box[1], p+box[2], p+box[6], p+box[5]); //right
    drawQuad(p+box[4], p+box[5], p+box[6], p+box[7]); //back
    drawQuad(p+box[3], p+box[0], p+box[4], p+box[7]); //left
    drawQuad(p+box[2], p+box[3], p+box[7], p+box[6]); //top
    drawQuad(p+box[5], p+box[4], p+box[0], p+box[1]); //bottom
  }
public:  
  virtual void render()
  {
    glBegin(GL_QUADS);
      drawBox(Vector3(0.0, 0.0, 0.0), 2.0);
    glEnd();
  }

  virtual void renderNormals(){}
};

#endif

