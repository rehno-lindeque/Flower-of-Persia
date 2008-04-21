#ifndef __SHAPES_H__
#define __SHAPES_H__

#define SHAPE_ACCURACY 1.0f

/*enum ShapeFacing { INWARDS, OUTWARDS };
enum ShapeShading { SMOOTH_SHADING, FLAT_SHADING };*/

/*class UVMapping
{
public:
  virtual Vector2 getUV(float x, float y, float z) = 0;
  Vector2 getUV(float* p) { return getUV(x, y, z); }
};
class XYZMapping
{
public:
  virtual Vector2 getUV(float x, float y, float z) { return Vector2(x/8.0f + z/8.0f, y/8.0f + z/8.0f); }
};*/
//extern UVGenerator* uvMap;

/*extern enum VERTEX_WINDING
{
  REGULAR,
  INVERTED
} vertexWinding;*/


//inline void drawVertex3f(float x, float y, float z) { glTexCoord2f(x/8.0f + z/8.0f, y/8.0f + z/8.0f); glVertex3f(x, y, z); }
inline void drawVertex3f(float x, float y, float z) { glTexCoord2fv(uvMap0->generateUV(x, y,z)); glVertex3f(x, y, z); }
inline void drawVertex3fv(float* p)
{
  //glTexCoord2f(p[0]/8.0f, p[1]/8.0f + p[2]/8.0f); glVertex3fv(p);
  glTexCoord2fv(uvMap0->generateUV(p)); glVertex3fv(p);
}

void drawQuadNormUV(float* n1, float* uv1, float* point1, float* n2, float* uv2, float* point2, float* n3, float* uv3, float* point3, float* n4, float* uv4, float* point4)
{
  // draw
  glTexCoord2fv(uv1); glNormal3fv(n1); glVertex3fv(point1);
  glTexCoord2fv(uv2); glNormal3fv(n2); glVertex3fv(point2);
  glTexCoord2fv(uv3); glNormal3fv(n3); glVertex3fv(point3);
  glTexCoord2fv(uv4); glNormal3fv(n4); glVertex3fv(point4);
}

void drawReverseQuadNormUV(float* n1, float* uv1, float* point1, float* n2, float* uv2, float* point2, float* n3, float* uv3, float* point3, float* n4, float* uv4, float* point4)
{
  // draw
  glTexCoord2fv(uv4); glNormal3fv(-(Vector3)n4); glVertex3fv(point4);
  glTexCoord2fv(uv3); glNormal3fv(-(Vector3)n3); glVertex3fv(point3);
  glTexCoord2fv(uv2); glNormal3fv(-(Vector3)n2); glVertex3fv(point2);
  glTexCoord2fv(uv1); glNormal3fv(-(Vector3)n1); glVertex3fv(point1);
}

void drawQuadNorm(float* n1, float* point1, float* n2, float* point2, float* n3, float* point3, float* n4, float* point4)
{
  // draw
  glNormal3fv(n1); drawVertex3fv(point1);
  glNormal3fv(n2); drawVertex3fv(point2);
  glNormal3fv(n3); drawVertex3fv(point3);
  glNormal3fv(n4); drawVertex3fv(point4);
}

void drawReverseQuadNorm(float* n1, float* point1, float* n2, float* point2, float* n3, float* point3, float* n4, float* point4)
{
  // draw
  glNormal3fv(-(Vector3)n4); drawVertex3fv(point4);
  glNormal3fv(-(Vector3)n3); drawVertex3fv(point3);
  glNormal3fv(-(Vector3)n2); drawVertex3fv(point2);
  glNormal3fv(-(Vector3)n1); drawVertex3fv(point1);
}

void drawQuad(float* point1, float* point2, float* point3, float* point4)
{
  // calculate normal
  Vector3 v1 = (Vector3)point2-(Vector3)point1;
  Vector3 v2 = (Vector3)point4-(Vector3)point1;
  Vector3 nv = Vector3::cross(v1, v2).normalize();

  // draw
  glNormal3fv(nv); drawVertex3fv(point1);
  glNormal3fv(nv); drawVertex3fv(point2);
  glNormal3fv(nv); drawVertex3fv(point3);
  glNormal3fv(nv); drawVertex3fv(point4);
}

void drawReverseQuad(float* point1, float* point2, float* point3, float* point4)
{
  // calculate normal
  Vector3 v1 = (Vector3)point2-(Vector3)point1;
  Vector3 v2 = (Vector3)point4-(Vector3)point1;
  Vector3 nv = Vector3::cross(v2, v1).normalize();

  // draw
  glNormal3fv(nv); drawVertex3fv(point4);
  glNormal3fv(nv); drawVertex3fv(point3);
  glNormal3fv(nv); drawVertex3fv(point2);
  glNormal3fv(nv); drawVertex3fv(point1);
}

void drawQuads(float* floor, uint nQuads)
{
  for(uint c = 0; c < nQuads; c++)
    drawQuad(&floor[(c*4)*3], &floor[(c*4+1)*3], &floor[(c*4+2)*3], &floor[(c*4+3)*3]);
}

void extrudeYLine(float* point1, float* point2, float height)
{
  //note: point1[1] should == point2[1] (i.e. same y-values)

  //calculate normal vector
  Vector3 v1 = Vector3(point1[0], point1[1]+height, point1[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 v2 = Vector3(point2[0], point2[1],        point2[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 nv = Vector3::cross(v1, v2).normalize();

  //draw
  glNormal3fv(nv); drawVertex3f(point1[0], point1[1],        point1[2]);
  glNormal3fv(nv); drawVertex3f(point1[0], point1[1]+height, point1[2]);
  glNormal3fv(nv); drawVertex3f(point2[0], point2[1]+height, point2[2]);
  glNormal3fv(nv); drawVertex3f(point2[0], point2[1],        point2[2]);
}


void extrudeYSegments(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments; ++cSegment)
    extrudeYLine(&shapeVertices[3*(cSegment*2)], &shapeVertices[3*(cSegment*2+1)], height);
}


void extrudeInvertedYShape(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine(&shapeVertices[3*(cSegment+1)], &shapeVertices[3*cSegment], height);
}

void extrudeYShape(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], height);
}

void extrudeClosedYShape(float* shapeVertices, int nSegments, float height)
{
  int cSegment;

  for(cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], height);

  extrudeYLine(&shapeVertices[3*cSegment], &shapeVertices[0], height);
}

void extrudeYQuad(float radius, float startHeight, float endHeight)
{
  float quad[4][3] =
  { {-radius, startHeight, -radius},
    { radius, startHeight, -radius},
    { radius, startHeight,  radius},
    {-radius, startHeight,  radius} };

  //draw
  extrudeClosedYShape((float*)quad, 4, endHeight-startHeight);
}

void extrudeYQuad(float radius, float startHeight, float endHeight, float xPos, float zPos)
{
  float quad[4][3] =
  { {-radius+xPos, startHeight, -radius+zPos},
    { radius+xPos, startHeight, -radius+zPos},
    { radius+xPos, startHeight,  radius+zPos},
    {-radius+xPos, startHeight,  radius+zPos} };

  //draw
  extrudeClosedYShape((float*)quad, 4, endHeight-startHeight);
}

void drawZTrapesium(float basisRadius, float topRadius, float startZ, float endZ, float yHeight)
{
  Vector3 nv;
  if(startZ < endZ)
    nv = Vector3(0.0f, 1.0f, 0.0f);   // it points upwards
  else
    nv = Vector3(0.0f, -1.0f, 0.0f);  // it points downwards

  //draw
  glNormal3fv(nv); drawVertex3f(-basisRadius, yHeight, startZ);
  glNormal3fv(nv); drawVertex3f(-topRadius,   yHeight, endZ);
  glNormal3fv(nv); drawVertex3f( topRadius,   yHeight, endZ);
  glNormal3fv(nv); drawVertex3f( basisRadius, yHeight, startZ);
}

void drawXTrapesium(float basisRadius, float topRadius, float startX, float endX, float yHeight)
{
  if(startX < endX)
    glNormal3f(0.0f, 1.0f, 0.0f);   // it points upwards
  else
    glNormal3f(0.0f, -1.0f, 0.0f);  // it points downwards

  drawVertex3f(startX, yHeight,  basisRadius);
  drawVertex3f(endX,   yHeight,  topRadius);
  drawVertex3f(endX,   yHeight, -topRadius);
  drawVertex3f(startX, yHeight, -basisRadius);
}

void extrudeXZLine(float* point1, float* point2, float scale)
{
  //calculate normal vector
  /*Vector3 v1 = Vector3(point1[0], point1[1]-xzHeight, point1[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 v2 = Vector3(point2[0], point2[1],          point2[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 nv = Vector3::cross(v1.normalize(), v2.normalize());*/
  Vector3 nv(0.0f, 1.0f, 0.0f);

  //draw
  glNormal3fv(nv); drawVertex3f(point2[0],       point2[1], point2[2]);
  glNormal3fv(nv); drawVertex3f(point2[0]*scale, point2[1], point2[2]*scale);
  glNormal3fv(nv); drawVertex3f(point1[0]*scale, point1[1], point1[2]*scale);
  glNormal3fv(nv); drawVertex3f(point1[0],       point1[1], point1[2]);
}

void extrudeXZShape(float* shapeVertices, int nSegments, float scale)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeXZLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], scale);
}

void extrudeClosedXZShape(float* shapeVertices, int nSegments, float scale)
{
  int cSegment;

  for(cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeXZLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], scale);

  extrudeXZLine(&shapeVertices[3*cSegment], &shapeVertices[0], scale);
}

void drawOpenXZQuad(float innerRadius, float outerRadius, float yHeight)
{
  /*drawZTrapesium(innerRadius, outerRadius,  innerRadius,  outerRadius, yHeight); // top
  drawXTrapesium(innerRadius, outerRadius,  innerRadius,  outerRadius, yHeight); // right
  drawZTrapesium(outerRadius, innerRadius, -outerRadius, -innerRadius, yHeight); // bottom
  drawXTrapesium(outerRadius, innerRadius, -outerRadius, -innerRadius, yHeight); // left*/
  float quad[4][3] =
  { {-innerRadius, yHeight, -innerRadius},
    { innerRadius, yHeight, -innerRadius},
    { innerRadius, yHeight,  innerRadius},
    {-innerRadius, yHeight,  innerRadius} };
  extrudeClosedXZShape((float*)quad, 4, outerRadius/innerRadius);
}

void drawOpenXZBlock(float innerRadius, float outerRadius, float startHeight, float endHeight)
{
  /*float quad[4][3] =
  { {-1.0f, -1.0f, 0.0f},
    { 1.0f, -1.0f, 0.0f},
    { 1.0f,  1.0f, 0.0f},
    {-1.0f,  1.0f, 0.0f} };*/

  //outer quads
  /*drawYQuad(Vector3(quad[0])*outerRadius, Vector3(quad[1])*outerRadius, startHeight, endHeight);
  drawYQuad(Vector3(quad[1])*outerRadius, Vector3(quad[2])*outerRadius, startHeight, endHeight);
  drawYQuad(Vector3(quad[2])*outerRadius, Vector3(quad[3])*outerRadius, startHeight, endHeight);
  drawYQuad(Vector3(quad[3])*outerRadius, Vector3(quad[0])*outerRadius, startHeight, endHeight);*/
  extrudeYQuad(outerRadius, startHeight, endHeight);

  //inner quads
  /*drawYQuad(Vector3(quad[0])*innerRadius, Vector3(quad[3])*innerRadius, startHeight, endHeight);
  drawYQuad(Vector3(quad[3])*innerRadius, Vector3(quad[2])*innerRadius, startHeight, endHeight);
  drawYQuad(Vector3(quad[2])*innerRadius, Vector3(quad[1])*innerRadius, startHeight, endHeight);
  drawYQuad(Vector3(quad[1])*innerRadius, Vector3(quad[0])*innerRadius, startHeight, endHeight);*/
  extrudeYQuad(innerRadius, endHeight, startHeight);

  //top quads
  drawOpenXZQuad(innerRadius, outerRadius, endHeight);
}

void drawBox(float radius)
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

  drawQuad(box[0], box[3], box[2], box[1]); //front
  drawQuad(box[1], box[2], box[6], box[5]); //right
  drawQuad(box[4], box[5], box[6], box[7]); //back
  drawQuad(box[3], box[0], box[4], box[7]); //left
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
  drawQuad(p+box[1], p+box[0], p+box[4], p+box[5]); //bottom
}

void drawYCylinder(float* position, float radius, float height, uint divisions)
{
  Vector3 v1(-radius, 0.0f, 0.0f);
  Vector3 v2(-radius, height, 0.0f);
  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);

  for(uint c = 1; c < divisions; c++)
  {
    float t = c / (float)divisions;
    Vector3 v1(-cos(t*PI*2.0f)*radius, 0.0f,   sin(t*PI*2.0f)*radius);
    Vector3 v2(-cos(t*PI*2.0f)*radius, height, sin(t*PI*2.0f)*radius);
    Vector3 vn(-cos(t*PI*2.0f),        0.0f,   sin(t*PI*2.0f));
    glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
    glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
    glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
    glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
  }
  /*float t = c / (float)divisions;
  Vector3 v1(-cos(t*PI*2.0f)*radius, 0.0f,   sin(t*PI*2.0f)*radius);
  Vector3 v2(-cos(t*PI*2.0f)*radius, height, sin(t*PI*2.0f)*radius);
  Vector3 vn(-cos(t*PI*2.0f),        0.0f,   sin(t*PI*2.0f)*radius);*/

  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);
  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
}

void drawZCylinder(float* position, float radius, float length, uint divisions)
{
  Vector3 v1(-radius, 0.0f,  length/2.0f);
  Vector3 v2(-radius, 0.0f, -length/2.0f);
  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);

  for(uint c = 1; c < divisions; c++)
  {
    float t = c / (float)divisions;
    Vector3 v1(-cos(t*PI*2.0f)*radius, sin(t*PI*2.0f)*radius, length/2.0f);
    Vector3 v2(-cos(t*PI*2.0f)*radius, sin(t*PI*2.0f)*radius, -length/2.0f);
    Vector3 vn(-cos(t*PI*2.0f),        sin(t*PI*2.0f), 0.0f);
    glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
    glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
    glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
    glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
  }
  /*float t = c / (float)divisions;
  Vector3 v1(-cos(t*PI*2.0f)*radius, 0.0f,   sin(t*PI*2.0f)*radius);
  Vector3 v2(-cos(t*PI*2.0f)*radius, height, sin(t*PI*2.0f)*radius);
  Vector3 vn(-cos(t*PI*2.0f),        0.0f,   sin(t*PI*2.0f)*radius);*/

  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);
  glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
}

void extrudeYCylinder(float* position, uint divisions, float* radia, float* heights, uint heightSegments)
{
  for(uint ch = 0; ch < heightSegments; ch++)
  {
    Vector3 v1(-radia[ch], heights[ch],   0.0f);
    Vector3 v2(-radia[ch+1], heights[ch+1], 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);

    for(uint c = 1; c < divisions; c++)
    {
      float t = c / (float)divisions;
      Vector3 v1(-cos(t*PI*2.0f)*radia[ch],   heights[ch],   sin(t*PI*2.0f)*radia[ch]);
      Vector3 v2(-cos(t*PI*2.0f)*radia[ch+1], heights[ch+1], sin(t*PI*2.0f)*radia[ch+1]);

      float heightDiff = sqrt((heights[ch] - heights[ch+1])*(heights[ch] - heights[ch+1]) + (radia[ch+1] - radia[ch])*(radia[ch+1] - radia[ch]));
      Vector3 vn(-cos(t*PI*2.0f), heightDiff, sin(t*PI*2.0f)); //todo
      vn.normalize();

      glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
      glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
      glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
      glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
    }

    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);
    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
  }
}

void extrudeReverseYCylinder(float* position, uint divisions, float* radia, float* heights, uint heightSegments)
{
  for(uint ch = 0; ch < heightSegments; ch++)
  {
    Vector3 v2(-radia[ch],   -heights[ch],   0.0f);
    Vector3 v1(-radia[ch+1], -heights[ch+1], 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);

    for(uint c = 1; c < divisions; c++)
    {
      float t = c / (float)divisions;
      Vector3 v2(-cos(t*PI*2.0f)*radia[ch],   -heights[ch],   sin(t*PI*2.0f)*radia[ch]);
      Vector3 v1(-cos(t*PI*2.0f)*radia[ch+1], -heights[ch+1], sin(t*PI*2.0f)*radia[ch+1]);
      Vector3 vn(-cos(t*PI*2.0f),        0.0f,          sin(t*PI*2.0f)); //todo
      glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
      glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
      glNormal3fv(vn); drawVertex3fv(v2 + (Vector3)position);
      glNormal3fv(vn); drawVertex3fv(v1 + (Vector3)position);
    }

    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v1 + (Vector3)position);
    glNormal3f(-1.0f, 0.0f, 0.0f); drawVertex3fv(v2 + (Vector3)position);
  }
}

void drawYBlock(Vector3 pos, float radius, float height)
{
  float box[][3] =
  { //front
    {-radius, 0.0f,   -radius},
    { radius, 0.0f,   -radius},
    { radius, height, -radius},
    {-radius, height, -radius},

    //back
    {-radius, 0.0f,   radius},
    { radius, 0.0f,   radius},
    { radius, height, radius},
    {-radius, height, radius} };

  drawQuad(pos + box[0], pos + box[3], pos + box[2], pos + box[1]); //front
  drawQuad(pos + box[1], pos + box[2], pos + box[6], pos + box[5]); //right
  drawQuad(pos + box[4], pos + box[5], pos + box[6], pos + box[7]); //back
  drawQuad(pos + box[3], pos + box[0], pos + box[4], pos + box[7]); //left
  drawQuad(pos + box[2], pos + box[3], pos + box[7], pos + box[6]); //top
}

void drawXBlock(Vector3 pos, float radius, float startX, float endX)
{
  float box[][3] =
  { //front
    { startX, -radius, -radius},
    { endX,   -radius, -radius},
    { endX,    radius, -radius},
    { startX,  radius, -radius},

    //back
    { startX,-radius, radius},
    { endX,  -radius, radius},
    { endX,   radius, radius},
    { startX, radius, radius} };

  drawQuad(pos + box[0], pos + box[3], pos + box[2], pos + box[1]); //front
  drawQuad(pos + box[1], pos + box[2], pos + box[6], pos + box[5]); //right
  drawQuad(pos + box[4], pos + box[5], pos + box[6], pos + box[7]); //back
  drawQuad(pos + box[3], pos + box[0], pos + box[4], pos + box[7]); //left
  drawQuad(pos + box[2], pos + box[3], pos + box[7], pos + box[6]); //top
}

void drawBlock(Vector3 pos, float width, float height, float depth)
{
  float box[][3] =
  { //front
    { -width/2.0, -height/2.0, -depth/2.0},
    {  width/2.0, -height/2.0, -depth/2.0},
    {  width/2.0,  height/2.0, -depth/2.0},
    { -width/2.0,  height/2.0, -depth/2.0},

    //back
    {-width/2.0, -height/2.0, depth/2.0},
    { width/2.0, -height/2.0, depth/2.0},
    { width/2.0,  height/2.0, depth/2.0},
    {-width/2.0,  height/2.0, depth/2.0} };

  drawQuad(pos + box[0], pos + box[3], pos + box[2], pos + box[1]); //front
  drawQuad(pos + box[1], pos + box[2], pos + box[6], pos + box[5]); //right
  drawQuad(pos + box[4], pos + box[5], pos + box[6], pos + box[7]); //back
  drawQuad(pos + box[3], pos + box[0], pos + box[4], pos + box[7]); //left
  drawQuad(pos + box[2], pos + box[3], pos + box[7], pos + box[6]); //top
  drawQuad(pos + box[0], pos + box[1], pos + box[5], pos + box[4]); //bottom
}

void drawReverseYBlock(Vector3 pos, float radius, float height)
{
  float box[][3] =
  { //front
    {-radius, 0.0f,   -radius},
    { radius, 0.0f,   -radius},
    { radius, height, -radius},
    {-radius, height, -radius},

    //back
    {-radius, 0.0f,   radius},
    { radius, 0.0f,   radius},
    { radius, height, radius},
    {-radius, height, radius} };

  drawQuad(pos + box[1], pos + box[2], pos + box[3], pos + box[0]); //front
  drawQuad(pos + box[5], pos + box[6], pos + box[2], pos + box[1]); //right
  drawQuad(pos + box[7], pos + box[6], pos + box[5], pos + box[4]); //back
  drawQuad(pos + box[7], pos + box[4], pos + box[0], pos + box[3]); //left
  drawQuad(pos + box[6], pos + box[7], pos + box[3], pos + box[2]); //top
}

void drawQuadList(float* q, uint nQuads)
{
  for(uint c = 0; c < nQuads; c++)
    drawQuad(q+(c*2)*3, q+(c*2+2)*3, q+(c*2+3)*3, q+(c*2+1)*3);
}

#endif
