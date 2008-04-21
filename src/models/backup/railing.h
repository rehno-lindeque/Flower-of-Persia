#ifndef __RAILING_H__
#define __RAILING_H__

#include "Model.h"

class Railing : public Model
{
protected:
  void drawBoxPath(float* path, uint pathSegs, float radius)
  {
    /*Vector3 dir = ((Vector3)&path[0] - (Vector3)&path[3]).normalize();
    Vector3 start = radius*dir + (Vector3)&path[0];
    Vector3 right = Vector3::cross(dir, Vector3(0.0f, 1.0f, 0.0f));
    drawQuad( start - radius*right + radius*Vector3(0.0f, 1.0f, 0.0f), // tl
              start + radius*right + radius*Vector3(0.0f, 1.0f, 0.0f), // tr
              start + radius*right + radius*Vector3(0.0f,-1.0f, 0.0f), // br
              start - radius*right + radius*Vector3(0.0f,-1.0f, 0.0f)  // bl
              );

    for(uint c = 0; c < 1; c++);
    {
      Vector3 dirNext = ((Vector3)&path[(c+1)*3] - (Vector3)&path[(c+2)*3]).normalize();
      Vector3 rightNext = Vector3::cross(dirNext, Vector3(0.0f, 1.0f, 0.0f));
      Vector3 posNext = (Vector3)&path[c*3];

      //top
      Vector3 p1 =  posNext - radius*(right+rightNext).normalize() + radius*Vector3(0.0f, 1.0f, 0.0f); //tl
      Vector3 p2 =  posNext + radius*(right+rightNext).normalize() + radius*Vector3(0.0f, 1.0f, 0.0f);
      Vector3 p3 =  start + radius*right + radius*Vector3(0.0f, 1.0f, 0.0f);
      Vector3 p4 =  start - radius*right + radius*Vector3(0.0f, 1.0f, 0.0f);
      
      drawQuad(p1, p2, p3, p4);

      dir = dirNext;
      right = rightNext;      
    }*/
    
  }

public:
  GLuint displayList;

  virtual void build()
  {
    float radius = 5.5;
    float startHeight = 0.0f;
    float rail[6][3] = 
    { { radius-radius/6.0f, startHeight, -radius},
      { radius,             startHeight, -radius},
      { radius,             startHeight,  radius},
      {-radius,             startHeight,  radius},      
      {-radius,             startHeight, -radius},
      {-radius+radius/6.0f, startHeight, -radius}};
    
    float platformHeight = 12.2f;
    float platformRail[][3] = 
    { {-24.0f, platformHeight,  7.5f},
      {-8.5f,  platformHeight,  7.5f},
      {-8.5f,  platformHeight, -7.5f},
      {-19.0f, platformHeight, -7.5f},
      {-19.0f, platformHeight,-23.0f} };    
    float platformRail2[][3] = 
    { { 24.0f, platformHeight,  7.5f},
      { 8.5f,  platformHeight,  7.5f},
      { 8.5f,  platformHeight, -7.5f},
      { 19.0f, platformHeight, -7.5f},
      { 19.0f, platformHeight,-23.0f} };
    
    UVGenerator* oldUVMap = uvMap0;    
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);    
      glDisable(GL_CULL_FACE); //for the shader
      glPushMatrix();
      
      /*glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, 0.2f);*/ //turned off so masking can be done in a shader
            
      glBindTexture(GL_TEXTURE_2D, textures.get(5));
      glBegin(GL_QUADS);
        // draw pool railing
        Y_XZUVGenerator uvGenerator1(1.0f);
        uvMap0 = &uvGenerator1;
        //extrudeYQuad(4.8f, 0.0f, 1.0f);        
        extrudeYShape((float*)rail, 6, 1.0f);
        //extrudeInvertedYShape((float*)rail, 6, 1.0f);

        //draw platform railing
        Y_XZUVGenerator uvGenerator2(0.5484f);
        uvMap0 = &uvGenerator2;
        extrudeYShape((float*)platformRail, 5, 1.5f);
        //extrudeInvertedYShape((float*)platformRail, 5, 1.5f);
        extrudeYShape((float*)platformRail2, 5, 1.5f);
        //extrudeInvertedYShape((float*)platformRail2, 5, 1.5f);

        glEnable(GL_CULL_FACE);
      glEnd();
      //glDisable(GL_ALPHA_TEST); //turned off for shader

      /*Y_XZUVGenerator uvGenerator3(8.0f);
      uvMap0 = &uvGenerator3;
      glBindTexture(GL_TEXTURE_2D, textures.get(6));
      glTranslatef(0.0f, 1.0f, 0.0f);
      glBegin(GL_QUADS);
        drawBoxPath((float*)rail, 6, 0.2f);      
      glEnd();*/
      glPopMatrix();
      glEnable(GL_CULL_FACE);
    glEndList();
    uvMap0 = oldUVMap;
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
