#ifndef __RAILING_H__
#define __RAILING_H__

#include "model.h"

class Railing : public Model
{
protected:
  uint tangentIndex;
  uint binormalIndex;

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

  /* overrided draw functions to include tangent and binormal data */

  void extrudeYLine(float* point1, float* point2, float height)
  {
    //note: point1[1] should == point2[1] (i.e. same y-values)

    //calculate normal vector
    Vector3 v1 = Vector3(point1[0], point1[1]+height, point1[2]) - Vector3(point1[0], point1[1], point1[2]);
    Vector3 v2 = Vector3(point2[0], point2[1],        point2[2]) - Vector3(point1[0], point1[1], point1[2]);
    Vector3 nv = Vector3::cross(v1, v2).normalize();

    // calculate tangent and binormal
    Vector3 tv = (Vector3(point2) - Vector3(point1)).normalize();
    Vector3 bnv(0.0, 0.0, 1.0);

    /*cout << v1.x << ' ' << v1.y << ' ' << v1.z << std::endl;
    cout << v2.x << ' ' << v2.y << ' ' << v2.z << std::endl; 
    cout << nv.x << ' ' << nv.y << ' ' << nv.z << std::endl;
    cout << tv.x << ' ' << tv.y << ' ' << tv.z << std::endl;
    cout << bnv.x << ' ' << bnv.y << ' ' << bnv.z << std::endl;
    cout << tangentIndex << std::endl;
    cout << binormalIndex << std::endl;*/

    //draw
    glNormal3fv(nv); // normal
    glVertexAttrib3fv(tangentIndex, tv); // tangent
    glVertexAttrib3fv(binormalIndex, bnv); // binormal
    drawVertex3f(point1[0], point1[1],        point1[2]); // vertex & uv

    glNormal3fv(nv);
    glVertexAttrib3fv(tangentIndex, tv); // tangent
    glVertexAttrib3fv(binormalIndex, bnv); // binormal
    drawVertex3f(point1[0], point1[1]+height, point1[2]); // vertex & uv

    glNormal3fv(nv);
    glVertexAttrib3fv(tangentIndex, tv); // tangent
    glVertexAttrib3fv(binormalIndex, bnv); // binormal
    drawVertex3f(point2[0], point2[1]+height, point2[2]); // vertex & uv

    glNormal3fv(nv); // normal
    glVertexAttrib3fv(tangentIndex, tv); // tangent
    glVertexAttrib3fv(binormalIndex, bnv); // binormal
    drawVertex3f(point2[0], point2[1],        point2[2]); // vertex & uv
  }

  void extrudeYShape(float* shapeVertices, int nSegments, float height)
  {
    for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
      extrudeYLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], height);
  }

public:
  GLuint displayList;

  virtual void build(uint tangentIndex, uint binormalIndex)
  {
    Railing::tangentIndex = tangentIndex;
    Railing::binormalIndex = binormalIndex;

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
      glAlphaFunc(GL_GREATER, 0.2f);*/ //turned off so masking can be done in a shader (a good idea?? - maybe not??)

      // geometry
      glBegin(GL_QUADS);
        // draw pool railing
        Y_XZUVGenerator uvGenerator1(1.0f);
        uvMap0 = &uvGenerator1;
        extrudeYShape(*rail, 6, 1.0f);

        //draw platform railing
        Y_XZUVGenerator uvGenerator2(0.5484f);
        uvMap0 = &uvGenerator2;
        extrudeYShape((float*)platformRail, 5, 1.5f);
        extrudeYShape((float*)platformRail2, 5, 1.5f);

        glEnable(GL_CULL_FACE);
      glEnd();

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
