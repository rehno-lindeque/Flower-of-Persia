#ifndef __WALL_H__
#define __WALL_H__

#include "model.h"

class Walls : public Model
{
protected:
  //copied from steps
  void drawCurveWall(Vector3 centerOfRotation, int nSteps, float stepsHeight, float basisHeight, float startAngle, float endAngle, float radius)
  {
    float a = (endAngle - startAngle) / (nSteps-1); // the angle step

    float h = stepsHeight/nSteps; // single step height
    float yPos = basisHeight + centerOfRotation(1);
    float anglePos = startAngle;

    for(int c = 0; c < nSteps-1; c++)
    {
      Vector3 v1 = centerOfRotation + Vector3(-cos(anglePos)*radius, 0.0f, sin(anglePos)*radius);
      Vector3 v2 = centerOfRotation + Vector3(-cos(anglePos+a)*radius, 0.0f, sin(anglePos+a)*radius);
      Vector3 n1 = -Vector3(-cos(anglePos), 0.0f, sin(anglePos));
      Vector3 n2 = -Vector3(-cos(anglePos+a), 0.0f, sin(anglePos+a));

      // draw side quad
      drawQuadNorm(n1, v1+Vector3(0.0f, yPos+h, 0.0f),  // bl
                   n1, v1+Vector3(0.0f, 32.0f,  0.0f),  //tl
                   n2, v2+Vector3(0.0f, 32.0f,  0.0f),  //tr
                   n2, v2+Vector3(0.0f, yPos+h, 0.0f)); //br
      yPos += h;
      anglePos += a;
    }
  }

  void drawReverseCurveWall(Vector3 centerOfRotation, int nSteps, float stepsHeight, float basisHeight, float startAngle, float endAngle, float radius)
  {
    float a = (endAngle - startAngle) / (nSteps-1); // the angle step

    float h = stepsHeight/nSteps; // single step height
    float yPos = basisHeight + centerOfRotation(1);
    float anglePos = startAngle;

    for(int c = 0; c < nSteps-1; c++)
    {
      Vector3 v1 = centerOfRotation + Vector3(-cos(anglePos)*radius, 0.0f, sin(anglePos)*radius);
      Vector3 v2 = centerOfRotation + Vector3(-cos(anglePos+a)*radius, 0.0f, sin(anglePos+a)*radius);
      Vector3 n1 = Vector3(-cos(anglePos), 0.0f, sin(anglePos));
      Vector3 n2 = Vector3(-cos(anglePos+a), 0.0f, sin(anglePos+a));

      // draw side quad
      drawReverseQuadNorm(n1, v1+Vector3(0.0f, yPos+h, 0.0f),  // bl
                   n1, v1+Vector3(0.0f, 32.0f,  0.0f),  //tl
                   n2, v2+Vector3(0.0f, 32.0f,  0.0f),  //tr
                   n2, v2+Vector3(0.0f, yPos+h, 0.0f)); //br
      yPos += h;
      anglePos += a;
    }
  }

public:
  GLuint displayList, normalList;

  virtual void build()
  {
    Y_XZUVGenerator uvGenerator(1/6.0f);
    UVGenerator* oldUVMap = uvMap0;
    uvMap0 = &uvGenerator;

    float startHeight = 0.0f;
    float wallHeight = 32.0f;
    float stepHeight = 0.5f;
    //float doubleDoorHeight = 11.5f;
    //float singleDoorHeight = 8.0f;
    float doubleDoorHeight = 0.5f;
    float singleDoorHeight = 0.5f;

    float walls[][3] =
    { // sides (left and right)
      {-24.0f, startHeight, -24.0}, // left (west)
      {-24.0f, startHeight,  24.0},
      { 24.0f, startHeight,  24.0}, // right (east)
      { 24.0f, startHeight, -24.0},

      // back wall
      {-20.0f, startHeight, 28.0f},
      {  0.0f, startHeight, 28.0f},
      {  0.0f, startHeight, 28.0f},
      { 20.0f, startHeight, 28.0f},

      // front wall
      { 12.0f, stepHeight*4, -36.0f}, // east side
      {  4.0f, stepHeight*4, -36.0f},
      { -4.0f, stepHeight*4, -36.0f}, // west side
      {-12.0f, stepHeight*4, -36.0f},

      // front wall door space
      {  4.0f, doubleDoorHeight, -36.0f},
      { -4.0f, doubleDoorHeight, -36.0f},

      // side door spaces
      {-24.0f, singleDoorHeight, 24.0},
      {-20.0f, singleDoorHeight, 28.0},
      { 20.0f, singleDoorHeight, 28.0},
      { 24.0f, singleDoorHeight, 24.0}
    };

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
      glPushMatrix();
      glBegin(GL_QUADS);

        // draw roof

        // draw all straight walls (sides, back and front)
        extrudeYSegments((float*)walls, 7, wallHeight);

        // front wall
        extrudeYSegments((float*)walls[8], 2, wallHeight-stepHeight*4);

        // double door space
        extrudeYSegments((float*)walls[12], 1, wallHeight - doubleDoorHeight);

        // side door spaces
        extrudeYSegments((float*)walls[14], 2, wallHeight - singleDoorHeight);

        // draw curved walls
        drawCurveWall(Vector3(-12.0f, 0.0f, -24.0f), 12, stepHeight*12, stepHeight*4-0.4f + stepHeight*6, (float)(PI*1.5f), (float)(2.0f*PI), 12.0f );
        drawReverseCurveWall(Vector3( 12.0f, 0.0f, -24.0f), 12, stepHeight*12, stepHeight*4-0.4f + stepHeight*6, (float)(1.5f*PI), (float)(PI*1.0f), 12.0f );

        // draw windows


      glEnd();
      glPopMatrix();
    glEndList();

    uvMap0 = oldUVMap;


    normalList = glGenLists(1);
    glNewList(normalList, GL_COMPILE);
      glPushMatrix();
      glBegin(GL_LINES);
        // draw all straight walls (sides, back and front)
        extrudeYSegments_drawNormals((float*)walls, 7, wallHeight);

        // front wall
        extrudeYSegments_drawNormals((float*)walls[8], 2, wallHeight-stepHeight*4);

        // double door space
        extrudeYSegments_drawNormals((float*)walls[12], 1, wallHeight - doubleDoorHeight);

        // side door spaces
        extrudeYSegments_drawNormals((float*)walls[14], 2, wallHeight - singleDoorHeight);

        // draw curved walls
        //drawCurveWall(Vector3(-12.0f, 0.0f, -24.0f), 12, stepHeight*12, stepHeight*4-0.4f + stepHeight*6, (float)(PI*1.5f), (float)(2.0f*PI), 12.0f );
        //drawReverseCurveWall(Vector3( 12.0f, 0.0f, -24.0f), 12, stepHeight*12, stepHeight*4-0.4f + stepHeight*6, (float)(1.5f*PI), (float)(PI*1.0f), 12.0f );

        // draw windows


      glEnd();
      glPopMatrix();
    glEndList();
  }

  virtual void render()
  {
    //draw floor
    glCallList(displayList);
  }

  virtual void renderNormals()
  {
    glCallList(normalList);
  }
};

#endif
