#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "Model.h"

class Portal : public Model
{
public:  
  GLuint displayList;

  //parameters
  float colRadius;
  float startHeight;
  float collumnHeight;
  float arcHeight;
  float gap;
  float pointHeight;
  uint segments;

  Vector3 getArcVertex(float t, float arcWidth, float arcHeight, float z)
  {
    float h = startHeight + collumnHeight;    

    float t1 = t;
    float t2 = (t <= 0.5f) ? t*2.0f : (1.0f-t)*2.0f;

    Vector3 va(cos(t1*PI)*arcWidth,            h+sin(t1*PI)*arcHeight,                       z);
    Vector3 vb(arcWidth - cos(t2*PI/2.0f)*arcWidth, h+(arcHeight-sin(t2*PI/2.0f)*arcHeight), z);
    Vector3 v = va*(1.0f-t2) + vb*t2; // linearly interpolate between va and vb
    return va;
  }

  void drawBackArc()
  {
    float h = startHeight + collumnHeight;
    float w = colRadius*2.0f;
    for(int c = 0; c < segments; c++)
    {
      float t1 = c / (float)segments;
      float t2 = c < segments/2+1? c : segments-c;
      t2 /= segments/2+1;
      
      //float line[][3] = { {} }
      if(c == 0)
      {
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*gap,     h+sin(t1*PI/2.0f)*arcHeight,             -8.0);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*(gap+w), h+sin(t1*PI/2.0f)*(arcHeight+colRadius), -8.0);
      }
      else if(c == segments-1)
      {
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*(gap+w), h+sin(t1*PI/2.0f)*(arcHeight+w), -8.0);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*gap,     h+sin(t1*PI/2.0f)*arcHeight,     -8.0);
      }
      else
      {
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*(gap+w), h+sin(t1*PI/2.0f)*(arcHeight+w), -8.0);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*gap,     h+sin(t1*PI/2.0f)*arcHeight,     -8.0);

        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*gap,     h+sin(t1*PI/2.0f)*arcHeight,     -8.0);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3f(cos(t1*PI/2.0f)*(gap+w), h+sin(t1*PI/2.0f)*(arcHeight+w), -8.0);
      }
    }
  }

  void drawFrontArc()
  {
    float h = startHeight + collumnHeight;
    float w = colRadius*2.0f;
    for(int c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
            
      if(c == 0)
      {
        /*glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3f(cos(t1*PI)*(gap+w), h+sin(t1*PI)*(arcHeight+colRadius), -8.0 - w);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3f(cos(t1*PI)*gap,     h+sin(t1*PI)*arcHeight,             -8.0 - w);*/
        Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w, -8.0-w);
        Vector3 v2 = getArcVertex(t1, gap, arcHeight,     -8.0-w);

        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
      }
      else if(c == segments-1)
      {
        Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w, -8.0-w);
        Vector3 v2 = getArcVertex(t1, gap, arcHeight,     -8.0-w);

        /*Vector3 v2a(cos(t1*PI)*(gap+w),           h+sin(t1*PI)*(arcHeight+w),                 -8.0 - w);
        Vector3 v2b((gap+w) - cos(t2*PI)*(gap+w), h+((arcHeight+w)-sin(t2*PI)*(arcHeight+w)), -8.0 - w);
        Vector3 v2 = v2a*(1.0f-t2) + v2b*t2; // linearly interpolate between v1a and v1b*/
        

        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
      }
      else
      {
        Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w, -8.0-w);
        Vector3 v2 = getArcVertex(t1, gap,   arcHeight,   -8.0-w);

        /*glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3f(cos(t1*PI)*gap,     h+sin(t1*PI)*arcHeight,     -8.0 - w);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3f(cos(t1*PI)*(gap+w), h+sin(t1*PI)*(arcHeight+w), -8.0 - w);
        
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3f(cos(t1*PI)*(gap+w), h+sin(t1*PI)*(arcHeight+w), -8.0 - w);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3f(cos(t1*PI)*gap,     h+sin(t1*PI)*arcHeight,     -8.0 - w);*/
        
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);

        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
      }
    }
  }

  virtual void build()
  {    
    colRadius = 0.65f;    
    startHeight = 0.0f;
    collumnHeight = 4.0f;
    arcHeight = 4.0f;
    gap = 4.0 - colRadius*2.0f;
    pointHeight = 4.0f;
    segments = 11; // this number must be odd
  
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);    
      glPushMatrix();
      glBegin(GL_QUADS);        
        // draw collumns
        extrudeYQuad(colRadius, startHeight, collumnHeight, -gap - colRadius, -8.0 - colRadius);
        extrudeYQuad(colRadius, startHeight, collumnHeight,  gap + colRadius, -8.0 - colRadius);

        // draw upper arc
        drawFrontArc();
        drawBackArc();
        

      glEnd();
      glPopMatrix();
    glEndList();
  }

  virtual void render()
  {
    //draw railing
    glCallList(displayList);
  }
};


#endif
