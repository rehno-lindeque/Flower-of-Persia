#ifndef __PILLAR_H__
#define __PILLAR_H__

class Pillar : public Model
{
public:  
  GLuint displayList;


  virtual void build()
  {    
    uint segments = 20;
    float pillarHeight = 9.4f;

    float pillars[][3] = 
    { // left side
      {-20.0f, 0.8f, -20.0f},
      {-20.0f, 0.8f, -12.0f},
      {-20.0f, 0.8f,  -4.0f},
      {-12.0f, 0.8f,  -4.0f},
      {-20.0f, 0.8f,   4.0f},
      {-12.0f, 0.8f,   4.0f},
      
      //right side
      { 20.0f, 0.8f, -20.0f},
      { 20.0f, 0.8f, -12.0f},
      { 20.0f, 0.8f,  -4.0f},
      { 12.0f, 0.8f,  -4.0f},
      { 20.0f, 0.8f,   4.0f},
      { 12.0f, 0.8f,   4.0f} };

    float rads[] = 
    { 0.5f, 0.7f, 0.75f, 0.7f, 0.4f, 0.5f, 0.3f };
    
    float heights[] = 
    { 0.45f, 0.6f, 0.75f, 0.9f, 1.05f, 1.25f, 1.35f };

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
      glColor3f(0.6f, 0.5f, 0.4f);
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D, textures.get(6));
      glBegin(GL_QUADS);
        for(int c = 0; c < 12; c++)
        {
          extrudeYCylinder(pillars[c], segments, rads, heights, 6);
          drawYCylinder(Vector3(0.0f, heights[6], 0.0f) + pillars[c], rads[6], pillarHeight-heights[6]*2.0f, segments);
          extrudeReverseYCylinder(Vector3(0.0f, pillarHeight, 0.0f) + pillars[c], segments, rads, heights, 6);
        }
      glEnd();

      //glBindTexture(GL_TEXTURE_2D, textures.get(0));
      glBegin(GL_QUADS);
        for(int c = 0; c < 12; c++)
        {
          drawYBlock(pillars[c], 0.81f, heights[0]);
          drawReverseYBlock(Vector3(0.0f, pillarHeight, 0.0f) + pillars[c], 0.81f, -heights[0]);

          glBindTexture(GL_TEXTURE_2D, textures.get(6));
          extrudeYCylinder(pillars[c], segments, rads, heights, 6);
          drawYCylinder(Vector3(0.0f, heights[6], 0.0f) + pillars[c], rads[6], pillarHeight-heights[6]*2.0f, segments);
          extrudeReverseYCylinder(Vector3(0.0f, pillarHeight, 0.0f) + pillars[c], segments, rads, heights, 6);
          glBindTexture(GL_TEXTURE_2D, textures.get(0));
        }
      glEnd();      
      glPopMatrix();
      glColor3f(1.0f, 1.0f, 1.0f);
    glEndList();
  }

  virtual void render()
  {
    //draw railing
    glCallList(displayList);
  }

  virtual void renderNormals()
  {
  }
};


#endif
