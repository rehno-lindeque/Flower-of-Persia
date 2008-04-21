#ifndef __ARCH_H__
#define __ARCH_H__

class Arch : public Model
{
public:  
  GLuint displayList, normalList;

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
    static float h = startHeight + collumnHeight;    

    float t1 = t;
    float t2 = (t <= 0.5f) ? t*2.0f : (1.0f-t)*2.0f;

    Vector3 va(-cos(t1*PI)*arcWidth,            h+sin(t1*PI)*arcHeight,                       z);
    Vector3 vb;
    /*if(t1 <= 0.5f)
      vb = Vector3(-arcWidth + sin(t2*PI/2.0f)*arcWidth, h+(arcHeight-cos(t2*PI/2.0f)*arcHeight), z);      
    else
      vb = Vector3( arcWidth - sin(t2*PI/2.0f)*arcWidth, h+(arcHeight-cos(t2*PI/2.0f)*arcHeight), z);*/
    //vb = Vector3(-arcWidth + t1*arcWidth*2.0, h + t2*arcHeight, z);
    vb = Vector3(-arcWidth + t1*arcWidth*2.0, h + 3.0 + t2*(arcHeight-3.0), z);
    
    
    Vector3 vl = va*(1.0f-t2) + vb*t2; // linearly interpolate between va and vb
    Vector3 vq = va*(1.0f-t2*t2) + vb*t2*t2; // quadratic interpolation between va and vb

    //Vector3 v = vl*0.2f + vq*0.8f;
    Vector3 v = vl;
    //Vector3 v = va;

    return v;
  }

  void drawBackArc(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
        
      Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w,  colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap,   arcHeight-0.2,colRadius) + pos;

      if(c == 0)
      {
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v2);
      }
      else if(c == segments-1)
      {
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v2);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v1);
      }
      else
      {
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v2);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v1);

        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, 1.0f); drawVertex3fv(v2);
      }
    }
  }

  void drawFrontArc(Vector3 pos)
  {
    //float h = startHeight + collumnHeight;
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
        
      Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w,   -colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap,   arcHeight-0.2, -colRadius) + pos;

      if(c == 0)
      {
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
      }
      else if(c == segments-1)
      {
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
      }
      else
      {
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);

        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v2);
        glNormal3f(0.0f, 0.0f, -1.0f); drawVertex3fv(v1);
      }
    }
  }

  void drawInsideArc(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
              
      Vector3 v1 = getArcVertex(t1, gap,   arcHeight-0.2, -colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap,   arcHeight-0.2, colRadius) + pos;
      Vector3 vnext = getArcVertex(t1+1.0f/(segments-1), gap , arcHeight-0.2, colRadius) + pos;
      Vector3 vprev = getArcVertex(t1-1.0f/(segments-1), gap , arcHeight-0.2, colRadius) + pos;

      if(c == 0)
      {        
        Vector3 vprev = Vector3(-gap, collumnHeight-1.0f, colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        glNormal3fv(vn); drawVertex3fv(v2);
        glNormal3fv(vn); drawVertex3fv(v1);
      }
      else if(c == segments-1)
      {         
        Vector3 vnext = Vector3( gap, collumnHeight-1.0f, colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        glNormal3fv(vn); drawVertex3fv(v1);
        glNormal3fv(vn); drawVertex3fv(v2);
      }
      else
      {
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        glNormal3fv(vn); drawVertex3fv(v1);
        glNormal3fv(vn); drawVertex3fv(v2);

        glNormal3fv(vn); drawVertex3fv(v2);
        glNormal3fv(vn); drawVertex3fv(v1);
      }
    }
  }

  void drawOutsideArc(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
              
      Vector3 v1 = getArcVertex(t1, gap+w,   arcHeight+w, colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap+w,   arcHeight+w, -colRadius) + pos;
      Vector3 vnext = getArcVertex(t1+1.0f/(segments-1), gap+w, arcHeight+w, -colRadius) + pos;
      Vector3 vprev = getArcVertex(t1-1.0f/(segments-1), gap+w, arcHeight+w, -colRadius) + pos;

      if(c == 0)
      {        
        Vector3 vprev = Vector3(-gap-w, collumnHeight-1.0f, -colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        glNormal3fv(vn); drawVertex3fv(v2);
        glNormal3fv(vn); drawVertex3fv(v1);
      }
      else if(c == segments-1)
      {         
        Vector3 vnext = Vector3( gap+w, collumnHeight-1.0f, -colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        glNormal3fv(vn); drawVertex3fv(v1);
        glNormal3fv(vn); drawVertex3fv(v2);
      }
      else
      {
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        glNormal3fv(vn); drawVertex3fv(v1);
        glNormal3fv(vn); drawVertex3fv(v2);

        glNormal3fv(vn); drawVertex3fv(v2);
        glNormal3fv(vn); drawVertex3fv(v1);
      }
    }
  }

  void drawBackArc_drawNormals(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
        
      Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w,  colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap,   arcHeight-0.2,colRadius) + pos;

      if(c == 0)
      {
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v1);
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v2);
      }
      else if(c == segments-1)
      {
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v2);
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v1);
      }
      else
      {
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v2);
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v1);
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v1);
        drawNormal(Vector3(0.0f, 0.0f, 1.0f), v2);
      }
    }
  }

  void drawFrontArc_drawNormals(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
        
      Vector3 v1 = getArcVertex(t1, gap+w, arcHeight+w,   -colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap,   arcHeight-0.2, -colRadius) + pos;

      if(c == 0)
      {
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v2);
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v1);
      }
      else if(c == segments-1)
      {
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v1);
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v2);
      }
      else
      {
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v1);
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v2);
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v2);
        drawNormal(Vector3(0.0f, 0.0f, -1.0f), v1);
      }
    }
  }

  void drawInsideArc_drawNormals(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
              
      Vector3 v1 = getArcVertex(t1, gap,   arcHeight-0.2, -colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap,   arcHeight-0.2, colRadius) + pos;
      Vector3 vnext = getArcVertex(t1+1.0f/(segments-1), gap , arcHeight-0.2, colRadius) + pos;
      Vector3 vprev = getArcVertex(t1-1.0f/(segments-1), gap , arcHeight-0.2, colRadius) + pos;

      if(c == 0)
      {        
        Vector3 vprev = Vector3(-gap, collumnHeight-1.0f, colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        drawNormal(vn, v2);
        drawNormal(vn, v1);
      }
      else if(c == segments-1)
      {         
        Vector3 vnext = Vector3( gap, collumnHeight-1.0f, colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        drawNormal(vn, v1);
        drawNormal(vn, v2);
      }
      else
      {
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        drawNormal(vn, v1);
        drawNormal(vn, v2);
        drawNormal(vn, v2);
        drawNormal(vn, v1);
      }
    }
  }

  void drawOutsideArc_drawNormals(Vector3 pos)
  {
    static float w = colRadius*2.0f;

    for(uint c = 0; c < segments; c++)
    {
      float t1 = c / ((float)segments-1);
      float t2 = c <= segments/2? c : segments-1-c;
      t2 /= segments/2;
              
      Vector3 v1 = getArcVertex(t1, gap+w,   arcHeight+w, colRadius) + pos;
      Vector3 v2 = getArcVertex(t1, gap+w,   arcHeight+w, -colRadius) + pos;
      Vector3 vnext = getArcVertex(t1+1.0f/(segments-1), gap+w, arcHeight+w, -colRadius) + pos;
      Vector3 vprev = getArcVertex(t1-1.0f/(segments-1), gap+w, arcHeight+w, -colRadius) + pos;

      if(c == 0)
      {        
        Vector3 vprev = Vector3(-gap-w, collumnHeight-1.0f, -colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        drawNormal(vn, v2);
        drawNormal(vn, v1);
      }
      else if(c == segments-1)
      {         
        Vector3 vnext = Vector3( gap+w, collumnHeight-1.0f, -colRadius) + pos;
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        drawNormal(vn, v1);
        drawNormal(vn, v2);
      }
      else
      {
        Vector3 vn1 = Vector3::cross((v1-v2).normalize(), (vnext-v2).normalize()).normalize();
        Vector3 vn2 = -Vector3::cross((v1-v2).normalize(), (vprev-v2).normalize()).normalize();
        Vector3 vn = (vn1 + vn2).normalize();

        drawNormal(vn, v1);
        drawNormal(vn, v2);

        drawNormal(vn, v2);
        drawNormal(vn, v1);
      }
    }
  }

  virtual void build()
  {    
    colRadius = 0.4f;    
    startHeight = 0.4f;
    collumnHeight = 22.0f;
    arcHeight = 10.0f;
    gap = 4.0 - colRadius*2.0f;
    pointHeight = 4.0f;
    segments = 21; // this number must be odd
  
    float arches[][3] = 
    { {-20.0f, 0.0f, 12.0+colRadius},
      {-12.0f, 0.0f, 12.0+colRadius},
      { -4.0f, 0.0f, 12.0+colRadius},
      {  4.0f, 0.0f, 12.0+colRadius},
      { 12.0f, 0.0f, 12.0+colRadius},
      { 20.0f, 0.0f, 12.0+colRadius}};

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);    
      glPushMatrix();
      glBegin(GL_QUADS);        
        for(int c = 0; c < 6; c++)
        {
          arcHeight = 10.0f+sin(c*PI/6.0f+PI/12.0f)*24.0;
                        
          // draw collumns
          extrudeYQuad(colRadius, startHeight, collumnHeight+startHeight, -gap - colRadius + arches[c][0], arches[c][2]);
          extrudeYQuad(colRadius, startHeight, collumnHeight+startHeight,  gap + colRadius + arches[c][0], arches[c][2]);

          // draw upper arc
          drawFrontArc(arches[c]);
          drawBackArc(arches[c]);
          drawInsideArc(arches[c]);
          drawOutsideArc(arches[c]);
          
          arcHeight = 10.0f+sin(c*PI/6.0f+PI/12.0f)*14.0;

          drawFrontArc(arches[c]);
          drawBackArc(arches[c]);
          drawInsideArc(arches[c]);
          drawOutsideArc(arches[c]);

          arcHeight = 10.0f+sin(c*PI/6.0f+PI/12.0f)*8.0;

          drawFrontArc(arches[c]);
          drawBackArc(arches[c]);
          drawInsideArc(arches[c]);
          drawOutsideArc(arches[c]);
        }        

        // draw bases
/*        for(int c = 0; c < 5; c++)
        {

        }*/

      glEnd();
      glPopMatrix();
    glEndList();


    normalList = glGenLists(1);
    glNewList(normalList, GL_COMPILE);    
      glPushMatrix();
      glBegin(GL_LINES);        
        for(int c = 0; c < 6; c++)
        {
          arcHeight = 10.0f+sin(c*PI/6.0f+PI/12.0f)*24.0;

          // draw collumns
          extrudeYQuad_drawNormals(colRadius, startHeight, collumnHeight+startHeight, -gap - colRadius + arches[c][0], arches[c][2]);
          extrudeYQuad_drawNormals(colRadius, startHeight, collumnHeight+startHeight,  gap + colRadius + arches[c][0], arches[c][2]);

          // draw upper arc
          drawFrontArc_drawNormals(arches[c]);
          drawBackArc_drawNormals(arches[c]);
          drawInsideArc_drawNormals(arches[c]);
          drawOutsideArc_drawNormals(arches[c]);
          
          arcHeight = 10.0f+sin(c*PI/6.0f+PI/12.0f)*14.0;

          drawFrontArc_drawNormals(arches[c]);
          drawBackArc_drawNormals(arches[c]);
          drawInsideArc_drawNormals(arches[c]);
          drawOutsideArc_drawNormals(arches[c]);

          arcHeight = 10.0f+sin(c*PI/6.0f+PI/12.0f)*8.0;

          drawFrontArc_drawNormals(arches[c]);
          drawBackArc_drawNormals(arches[c]);
          drawInsideArc_drawNormals(arches[c]);
          drawOutsideArc_drawNormals(arches[c]);
        }        
      glEnd();
      glPopMatrix();
    glEndList();
  }

  virtual void render()
  {
    //draw railing
    glCallList(displayList);
  }
  
  virtual void renderNormals()
  {
    glCallList(normalList);
  }
};


#endif
