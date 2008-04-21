#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "model.h"

class Floor : public Model
{
protected:
  void drawQuadsMultiTex(float* floor, uint nQuads)
  {
    for(uint c = 0; c < nQuads; c++)
      drawQuadMultiTex(&floor[(c*4)*3], &floor[(c*4+1)*3], &floor[(c*4+2)*3], &floor[(c*4+3)*3]);
  }

  inline void drawVertex3fvMultiTex(float* p)
  {
    glMultiTexCoord2fv(GL_TEXTURE0, uvMap0->generateUV(p));
    glMultiTexCoord2fv(GL_TEXTURE1, uvMap1->generateUV(p));
    glVertex3fv(p);
  }

  void drawQuadMultiTex(float* point1, float* point2, float* point3, float* point4)
  {
    // calculate normal
    Vector3 v1 = (Vector3)point2-(Vector3)point1;
    Vector3 v2 = (Vector3)point4-(Vector3)point1;
    Vector3 nv = Vector3::cross(v1, v2).normalize();

    // draw
    glNormal3fv(nv); drawVertex3fvMultiTex(point1);
    glNormal3fv(nv); drawVertex3fvMultiTex(point2);
    glNormal3fv(nv); drawVertex3fvMultiTex(point3);
    glNormal3fv(nv); drawVertex3fvMultiTex(point4);
  }

public:
  GLuint displayList, platformDisplayList;

  virtual void build()
  {
    displayList = glGenLists(1);
    platformDisplayList = glGenLists(1);

    // step data
    float radius = 8.0f; // step radius
    float startStepHeight = 0.0f;
    float endStepHeight = 0.4f;

    float step[6][3] =
    { {-radius+radius/2.0f, startStepHeight, -radius},
      {-radius,             startStepHeight, -radius},
      {-radius,             startStepHeight,  radius},
      { radius,             startStepHeight,  radius},
      { radius,             startStepHeight, -radius},
      { radius-radius/2.0f, startStepHeight, -radius}};

    // floor data
    float floor[][3] = //quads from top to bottom (and left to right)
    { {-12.0f,  endStepHeight, 28.0f},
      { -8.0f,  endStepHeight, 28.0f},
      { -8.0f,  endStepHeight, 16.0f},
      {-24.0f,  endStepHeight, 16.0f},

      { -8.0f,  endStepHeight, 28.0f},
      {  8.0f,  endStepHeight, 28.0f},
      {  8.0f,  endStepHeight, 16.0f},
      { -8.0f,  endStepHeight, 16.0f},

      {  8.0f,  endStepHeight, 28.0f},
      { 12.0f,  endStepHeight, 28.0f},
      { 24.0f,  endStepHeight, 16.0f},
      {  8.0f,  endStepHeight, 16.0f},

      {-24.0f,  endStepHeight, 16.0f},
      { -8.0f,  endStepHeight, 16.0f},
      { -8.0f,  endStepHeight,  8.0f},
      {-24.0f,  endStepHeight,  8.0f},

      { -8.0f,  endStepHeight, 16.0f},
      {  8.0f,  endStepHeight, 16.0f},
      {  8.0f,  endStepHeight,  8.0f},
      { -8.0f,  endStepHeight,  8.0f},

      {  8.0f,  endStepHeight, 16.0f},
      { 24.0f,  endStepHeight, 16.0f},
      { 24.0f,  endStepHeight,  8.0f},
      {  8.0f,  endStepHeight,  8.0f},

      {-radius,             endStepHeight, -radius},
      {-radius+radius/2.0f, endStepHeight, -radius},
      {-radius+radius/2.0f, endStepHeight, -30.5},
      {-radius,             endStepHeight, -30.5},

      { radius-radius/2.0f, endStepHeight, -radius},
      { radius,             endStepHeight, -radius},
      { radius,             endStepHeight, -30.5},
      { radius-radius/2.0f, endStepHeight, -30.5},

      {-24.0f,  endStepHeight*2,  radius},
      {-radius, endStepHeight*2,  radius},
      {-radius, endStepHeight*2, -radius},
      {-24.0f,  endStepHeight*2, -radius},

      {-24.0f,  endStepHeight*2, -radius},
      {-radius, endStepHeight*2, -radius},
      {-radius, endStepHeight*2, -30.5},
      {-24.0f,  endStepHeight*2, -30.5},

      { radius, endStepHeight*2,  radius},
      { 24.0f,  endStepHeight*2,  radius},
      { 24.0f,  endStepHeight*2, -radius},
      { radius, endStepHeight*2, -radius},

      { radius, endStepHeight*2, -radius},
      { 24.0f,  endStepHeight*2, -radius},
      { 24.0f,  endStepHeight*2, -30.5},
      { radius, endStepHeight*2, -30.5}
    };

    float path[][3] =
    { {-radius+radius/2.0f, startStepHeight, -radius},
      { radius-radius/2.0f, startStepHeight, -radius},
      { radius-radius/2.0f, startStepHeight, -30.5f},
      {-radius+radius/2.0f, startStepHeight, -30.5f} };

    float shadowUV1[][2] =
    { { 0.0f, 1.0f}, //
      { 1.0f, 1.0f},
      { 1.0f, 0.5f},
      { 0.0f, 0.5f},
      { 0.0f, 0.5f}, //
      { 1.0f, 0.5f},
      { 1.0f, 0.0f},
      { 0.0f, 0.0f}};

    float shadowUV2[][2] =
    { { 1.0f, 1.0f}, //
      { 0.0f, 1.0f},
      { 0.0f, 0.5f},
      { 1.0f, 0.5f},
      { 1.0f, 0.5f}, //
      { 0.0f, 0.5f},
      { 0.0f, 0.0f},
      { 1.0f, 0.0f}};

    UVGenerator* oldUVMap = uvMap0;

    glNewList(platformDisplayList, GL_COMPILE);
      glPushMatrix();

      //draw lower platform floors
      glActiveTexture(GL_TEXTURE0); //multi-texturing
      glBindTexture(GL_TEXTURE_2D, textures.get(7));
      glActiveTexture(GL_TEXTURE1); //multi-texturing
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textures.get(8));

      glBegin(GL_QUADS);
        XZUVGenerator uvGenerator(1.0f);
        uvMap0 = &uvGenerator;
        BufferUVGenerator uvShadowGenerator((float*)shadowUV1);
        uvMap1 = &uvShadowGenerator;
        drawQuadsMultiTex((float*)floor[8*4], 2);

        uvShadowGenerator.buffer = (float*)shadowUV2;
        uvShadowGenerator.reset();
        drawQuadsMultiTex((float*)floor[10*4], 2);

        // reset uv map
        uvMap0 = oldUVMap;
        uvMap1 = null;
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE0);
      glPopMatrix();
    glEndList();

    glNewList(displayList, GL_COMPILE);
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D, textures.get(0));
      glBegin(GL_QUADS);
        // draw path
        drawQuad(path[0], path[1], path[2], path[3]);

        //draw inner floor (close to pool)
        drawOpenXZQuad(4.2f, 8.0f, 0.0f);

        // draw step
        extrudeYLine(path[3], path[0], endStepHeight - startStepHeight); //path
        extrudeYLine(path[1], path[2], endStepHeight - startStepHeight);
        extrudeYShape((float*)step, 6, endStepHeight-startStepHeight); //inner floor
        extrudeYLine(Vector3(step[2][0], endStepHeight,-30.5), (Vector3)step[2]+Vector3(0.0f, endStepHeight, 0.0f), endStepHeight-startStepHeight);
        extrudeYLine((Vector3)step[3]+Vector3(0.0f, endStepHeight, 0.0f), Vector3(step[3][0], endStepHeight,-30.5), endStepHeight-startStepHeight);

        extrudeYLine((Vector3)step[2]+Vector3(0.0f, endStepHeight, 0.0f), Vector3(-24.0f, endStepHeight,step[2][2]), endStepHeight-startStepHeight);
        extrudeYLine(Vector3( 24.0f, endStepHeight,step[3][2]), (Vector3)step[3]+Vector3(0.0f, endStepHeight, 0.0f), endStepHeight-startStepHeight);

        // draw outer floor
        drawQuads((float*)floor, 8);
      glEnd();
      glPopMatrix();
    glEndList();
  }

  virtual void render()
  {
    //draw floor
    glCallList(displayList);
  }
  virtual void renderPlatforms()
  {
    //draw platforms
    //glCallList(platformDisplayList);

    // floor data
    float radius = 8.0f; // step radius
    float startStepHeight = 0.0f;
    float endStepHeight = 0.4f;

    float floor[][3] = //quads from top to bottom (and left to right)
    { {-12.0f,  endStepHeight, 28.0f},
      { -8.0f,  endStepHeight, 28.0f},
      { -8.0f,  endStepHeight, 16.0f},
      {-24.0f,  endStepHeight, 16.0f},

      { -8.0f,  endStepHeight, 28.0f},
      {  8.0f,  endStepHeight, 28.0f},
      {  8.0f,  endStepHeight, 16.0f},
      { -8.0f,  endStepHeight, 16.0f},

      {  8.0f,  endStepHeight, 28.0f},
      { 12.0f,  endStepHeight, 28.0f},
      { 24.0f,  endStepHeight, 16.0f},
      {  8.0f,  endStepHeight, 16.0f},

      {-24.0f,  endStepHeight, 16.0f},
      { -8.0f,  endStepHeight, 16.0f},
      { -8.0f,  endStepHeight,  8.0f},
      {-24.0f,  endStepHeight,  8.0f},

      { -8.0f,  endStepHeight, 16.0f},
      {  8.0f,  endStepHeight, 16.0f},
      {  8.0f,  endStepHeight,  8.0f},
      { -8.0f,  endStepHeight,  8.0f},

      {  8.0f,  endStepHeight, 16.0f},
      { 24.0f,  endStepHeight, 16.0f},
      { 24.0f,  endStepHeight,  8.0f},
      {  8.0f,  endStepHeight,  8.0f},

      {-radius,             endStepHeight, -radius},
      {-radius+radius/2.0f, endStepHeight, -radius},
      {-radius+radius/2.0f, endStepHeight, -30.5},
      {-radius,             endStepHeight, -30.5},

      { radius-radius/2.0f, endStepHeight, -radius},
      { radius,             endStepHeight, -radius},
      { radius,             endStepHeight, -30.5},
      { radius-radius/2.0f, endStepHeight, -30.5},

      {-24.0f,  endStepHeight*2,  radius},
      {-radius, endStepHeight*2,  radius},
      {-radius, endStepHeight*2, -radius},
      {-24.0f,  endStepHeight*2, -radius},

      {-24.0f,  endStepHeight*2, -radius},
      {-radius, endStepHeight*2, -radius},
      {-radius, endStepHeight*2, -30.5},
      {-24.0f,  endStepHeight*2, -30.5},

      { radius, endStepHeight*2,  radius},
      { 24.0f,  endStepHeight*2,  radius},
      { 24.0f,  endStepHeight*2, -radius},
      { radius, endStepHeight*2, -radius},

      { radius, endStepHeight*2, -radius},
      { 24.0f,  endStepHeight*2, -radius},
      { 24.0f,  endStepHeight*2, -30.5},
      { radius, endStepHeight*2, -30.5}
    };

    float path[][3] =
    { {-radius+radius/2.0f, startStepHeight, -radius},
      { radius-radius/2.0f, startStepHeight, -radius},
      { radius-radius/2.0f, startStepHeight, -30.5f},
      {-radius+radius/2.0f, startStepHeight, -30.5f} };

    float shadowUV1[][2] =
    { { 0.0f, 1.0f}, //
      { 1.0f, 1.0f},
      { 1.0f, 0.5f},
      { 0.0f, 0.5f},
      { 0.0f, 0.5f}, //
      { 1.0f, 0.5f},
      { 1.0f, 0.0f},
      { 0.0f, 0.0f}};

    float shadowUV2[][2] =
    { { 1.0f, 1.0f}, //
      { 0.0f, 1.0f},
      { 0.0f, 0.5f},
      { 1.0f, 0.5f},
      { 1.0f, 0.5f}, //
      { 0.0f, 0.5f},
      { 0.0f, 0.0f},
      { 1.0f, 0.0f}};

    UVGenerator* oldUVMap = uvMap0;

    glPushMatrix();

    //draw lower platform floors
    /*glActiveTexture(GL_TEXTURE0); //multi-texturing
    glBindTexture(GL_TEXTURE_2D, textures.get(7));
    glActiveTexture(GL_TEXTURE1); //multi-texturing
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures.get(8));*/

    glBegin(GL_QUADS);
      XZUVGenerator uvGenerator(1.0f);
      uvMap0 = &uvGenerator;
      BufferUVGenerator uvShadowGenerator((float*)shadowUV1);
      uvMap1 = &uvShadowGenerator;
      drawQuadsMultiTex((float*)floor[8*4], 2);

      uvShadowGenerator.buffer = (float*)shadowUV2;
      uvShadowGenerator.reset();
      drawQuadsMultiTex((float*)floor[10*4], 2);

      // reset uv map
      uvMap0 = oldUVMap;
      uvMap1 = null;
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glPopMatrix();
  }

  virtual void renderNormals()
  {
  }
};

#endif
