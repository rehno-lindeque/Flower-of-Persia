#ifndef __SHAPES_NORMALS_H__
#define __SHAPES_NORMALS_H__

void drawNormal(float* normal, float* vertex)
{
  glVertex3fv(vertex);
  glVertex3fv((Vector3)vertex + (Vector3)normal);
}

void drawQuadNorm_drawNormals(float* n1, float* point1, float* n2, float* point2, float* n3, float* point3, float* n4, float* point4)
{
  drawNormal(n1, point1);
  drawNormal(n2, point2);
  drawNormal(n3, point3);
  drawNormal(n4, point4);
}

void drawReverseQuadNorm_drawNormals(float* n1, float* point1, float* n2, float* point2, float* n3, float* point3, float* n4, float* point4)
{
  drawNormal(-(Vector3)n4, point4);
  drawNormal(-(Vector3)n3, point3);
  drawNormal(-(Vector3)n2, point2);
  drawNormal(-(Vector3)n1, point1);
}

void drawQuad_drawNormals(float* point1, float* point2, float* point3, float* point4)
{
  // calculate normal
  Vector3 v1 = (Vector3)point2-(Vector3)point1;
  Vector3 v2 = (Vector3)point4-(Vector3)point1;
  Vector3 nv = Vector3::cross(v1, v2).normalize();

  drawNormal(nv, point1);
  drawNormal(nv, point2);
  drawNormal(nv, point3);
  drawNormal(nv, point4);
}

void drawReverseQuad_drawNormals(float* point1, float* point2, float* point3, float* point4)
{
  // calculate normal
  Vector3 v1 = (Vector3)point2-(Vector3)point1;
  Vector3 v2 = (Vector3)point4-(Vector3)point1;
  Vector3 nv = Vector3::cross(v2, v1).normalize();

  // draw
  drawNormal(nv, point4);
  drawNormal(nv, point3);
  drawNormal(nv, point2);
  drawNormal(nv, point1);
}

void drawQuads_drawNormals(float* floor, uint nQuads)
{
  for(uint c = 0; c < nQuads; c++)
    drawQuad_drawNormals(&floor[(c*4)*3], &floor[(c*4+1)*3], &floor[(c*4+2)*3], &floor[(c*4+3)*3]);
}

void extrudeYLine_drawNormals(float* point1, float* point2, float height)
{
  //calculate normal vector
  Vector3 v1 = Vector3(point1[0], point1[1]+height, point1[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 v2 = Vector3(point2[0], point2[1],        point2[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 nv = Vector3::cross(v1, v2).normalize();

  //draw
  drawNormal(nv, Vector3(point1[0], point1[1],        point1[2]));
  drawNormal(nv, Vector3(point1[0], point1[1]+height, point1[2]));
  drawNormal(nv, Vector3(point2[0], point2[1]+height, point2[2]));
  drawNormal(nv, Vector3(point2[0], point2[1],        point2[2]));
}


void extrudeYSegments_drawNormals(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments; ++cSegment)
    extrudeYLine_drawNormals(&shapeVertices[3*(cSegment*2)], &shapeVertices[3*(cSegment*2+1)], height);
}


void extrudeInvertedYShape_drawNormals(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine_drawNormals(&shapeVertices[3*(cSegment+1)], &shapeVertices[3*cSegment], height);
}

void extrudeYShape_drawNormals(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine_drawNormals(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], height);
}

void extrudeClosedYShape_drawNormals(float* shapeVertices, int nSegments, float height)
{
  int cSegment;

  for(cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine_drawNormals(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], height);

  extrudeYLine_drawNormals(&shapeVertices[3*cSegment], &shapeVertices[0], height);
}

void extrudeYQuad_drawNormals(float radius, float startHeight, float endHeight)
{
  float quad[4][3] =
  { {-radius, startHeight, -radius},
    { radius, startHeight, -radius},
    { radius, startHeight,  radius},
    {-radius, startHeight,  radius} };

  //draw
  extrudeClosedYShape_drawNormals((float*)quad, 4, endHeight-startHeight);
}

void extrudeYQuad_drawNormals(float radius, float startHeight, float endHeight, float xPos, float zPos)
{
  float quad[4][3] =
  { {-radius+xPos, startHeight, -radius+zPos},
    { radius+xPos, startHeight, -radius+zPos},
    { radius+xPos, startHeight,  radius+zPos},
    {-radius+xPos, startHeight,  radius+zPos} };

  //draw
  extrudeClosedYShape_drawNormals((float*)quad, 4, endHeight-startHeight);
}

void drawZTrapesium_drawNormals(float basisRadius, float topRadius, float startZ, float endZ, float yHeight)
{
  Vector3 nv;
  if(startZ < endZ)
    nv = Vector3(0.0f, 1.0f, 0.0f);   // it points upwards
  else
    nv = Vector3(0.0f, -1.0f, 0.0f);  // it points downwards

  //draw
  drawNormal(nv, Vector3(-basisRadius, yHeight, startZ));
  drawNormal(nv, Vector3(-topRadius,   yHeight, endZ));
  drawNormal(nv, Vector3( topRadius,   yHeight, endZ));
  drawNormal(nv, Vector3( basisRadius, yHeight, startZ));
}

void drawXTrapesium_drawNormals(float basisRadius, float topRadius, float startX, float endX, float yHeight)
{
  Vector3 nv;
  if(startX < endX)
    nv = Vector3(0.0f, 1.0f, 0.0f);   // it points upwards
  else
    nv = Vector3(0.0f, -1.0f, 0.0f);  // it points downwards

  drawNormal(nv, Vector3(startX, yHeight,  basisRadius));
  drawNormal(nv, Vector3(endX,   yHeight,  topRadius));
  drawNormal(nv, Vector3(endX,   yHeight, -topRadius));
  drawNormal(nv, Vector3(startX, yHeight, -basisRadius));
}

void extrudeXZLine_drawNormals(float* point1, float* point2, float scale)
{
  Vector3 nv(0.0f, 1.0f, 0.0f);

  drawNormal(nv, Vector3(point2[0],       point2[1], point2[2]));
  drawNormal(nv, Vector3(point2[0]*scale, point2[1], point2[2]*scale));
  drawNormal(nv, Vector3(point1[0]*scale, point1[1], point1[2]*scale));
  drawNormal(nv, Vector3(point1[0],       point1[1], point1[2]));
}

void extrudeXZShape_drawNormals(float* shapeVertices, int nSegments, float scale)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeXZLine_drawNormals(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], scale);
}

void extrudeClosedXZShape_drawNormals(float* shapeVertices, int nSegments, float scale)
{
  int cSegment;

  for(cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeXZLine_drawNormals(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], scale);

  extrudeXZLine_drawNormals(&shapeVertices[3*cSegment], &shapeVertices[0], scale);
}

void drawOpenXZQuad_drawNormals(float innerRadius, float outerRadius, float yHeight)
{
  float quad[4][3] =
  { {-innerRadius, yHeight, -innerRadius},
    { innerRadius, yHeight, -innerRadius},
    { innerRadius, yHeight,  innerRadius},
    {-innerRadius, yHeight,  innerRadius} };

  extrudeClosedXZShape_drawNormals((float*)quad, 4, outerRadius/innerRadius);
}

void drawOpenXZBlock_drawNormals(float innerRadius, float outerRadius, float startHeight, float endHeight)
{
  extrudeYQuad_drawNormals(outerRadius, startHeight, endHeight);
  extrudeYQuad_drawNormals(innerRadius, endHeight, startHeight);
  drawOpenXZQuad_drawNormals(innerRadius, outerRadius, endHeight);
}

void drawBox_drawNormals(float radius)
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

  drawQuad_drawNormals(box[0], box[3], box[2], box[1]); //front
  drawQuad_drawNormals(box[1], box[2], box[6], box[5]); //right
  drawQuad_drawNormals(box[4], box[5], box[6], box[7]); //back
  drawQuad_drawNormals(box[3], box[0], box[4], box[7]); //left
}

void drawBox_drawNormals(float* position, float radius)
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
  drawQuad_drawNormals(p+box[0], p+box[3], p+box[2], p+box[1]); //front
  drawQuad_drawNormals(p+box[1], p+box[2], p+box[6], p+box[5]); //right
  drawQuad_drawNormals(p+box[4], p+box[5], p+box[6], p+box[7]); //back
  drawQuad_drawNormals(p+box[3], p+box[0], p+box[4], p+box[7]); //left
  drawQuad_drawNormals(p+box[2], p+box[3], p+box[7], p+box[6]); //top
  drawQuad_drawNormals(p+box[1], p+box[0], p+box[4], p+box[5]); //bottom
}

void drawYCylinder_drawNormals(float* position, float radius, float height, uint divisions)
{
  Vector3 v1(-radius, 0.0f, 0.0f);
  Vector3 v2(-radius, height, 0.0f);
  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), Vector3(v2 + (Vector3)position));
  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), Vector3(v1 + (Vector3)position));

  for(uint c = 1; c < divisions; c++)
  {
    float t = c / (float)divisions;
    Vector3 v1(-cos(t*PI*2.0f)*radius, 0.0f,   sin(t*PI*2.0f)*radius);
    Vector3 v2(-cos(t*PI*2.0f)*radius, height, sin(t*PI*2.0f)*radius);
    Vector3 vn(-cos(t*PI*2.0f),        0.0f,   sin(t*PI*2.0f));
    drawNormal(vn, Vector3(v1 + (Vector3)position));
    drawNormal(vn, Vector3(v2 + (Vector3)position));
    drawNormal(vn, Vector3(v2 + (Vector3)position));
    drawNormal(vn, Vector3(v1 + (Vector3)position));
  }

  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), Vector3(v1 + (Vector3)position));
  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), Vector3(v2 + (Vector3)position));
}

void drawZCylinder_drawNormals(float* position, float radius, float length, uint divisions)
{
  Vector3 v1(-radius, 0.0f,  length/2.0f);
  Vector3 v2(-radius, 0.0f, -length/2.0f);
  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v2 + (Vector3)position);
  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v1 + (Vector3)position);

  for(uint c = 1; c < divisions; c++)
  {
    float t = c / (float)divisions;
    Vector3 v1(-cos(t*PI*2.0f)*radius, sin(t*PI*2.0f)*radius, length/2.0f);
    Vector3 v2(-cos(t*PI*2.0f)*radius, sin(t*PI*2.0f)*radius, -length/2.0f);
    Vector3 vn(-cos(t*PI*2.0f),        sin(t*PI*2.0f), 0.0f);
    drawNormal(vn, Vector3(v1 + (Vector3)position));
    drawNormal(vn, Vector3(v2 + (Vector3)position));
    drawNormal(vn, Vector3(v2 + (Vector3)position));
    drawNormal(vn, Vector3(v1 + (Vector3)position));
  }

  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v1 + (Vector3)position);
  drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v2 + (Vector3)position);
}

void extrudeYCylinder_drawNormals(float* position, uint divisions, float* radia, float* heights, uint heightSegments)
{
  for(uint ch = 0; ch < heightSegments; ch++)
  {
    Vector3 v1(-radia[ch], heights[ch],   0.0f);
    Vector3 v2(-radia[ch+1], heights[ch+1], 0.0f);
    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v2 + (Vector3)position);
    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v1 + (Vector3)position);

    for(uint c = 1; c < divisions; c++)
    {
      float t = c / (float)divisions;
      Vector3 v1(-cos(t*PI*2.0f)*radia[ch],   heights[ch],   sin(t*PI*2.0f)*radia[ch]);
      Vector3 v2(-cos(t*PI*2.0f)*radia[ch+1], heights[ch+1], sin(t*PI*2.0f)*radia[ch+1]);
      Vector3 vn(-cos(t*PI*2.0f),        0.0f,          sin(t*PI*2.0f)); //todo
      drawNormal(vn, v1 + (Vector3)position);
      drawNormal(vn, v2 + (Vector3)position);
      drawNormal(vn, v2 + (Vector3)position);
      drawNormal(vn, v1 + (Vector3)position);
    }

    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v1 + (Vector3)position);
    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v2 + (Vector3)position);
  }
}

void extrudeReverseYCylinder_drawNormals(float* position, uint divisions, float* radia, float* heights, uint heightSegments)
{
  for(uint ch = 0; ch < heightSegments; ch++)
  {
    Vector3 v2(-radia[ch],   -heights[ch],   0.0f);
    Vector3 v1(-radia[ch+1], -heights[ch+1], 0.0f);
    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v2 + (Vector3)position);
    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v1 + (Vector3)position);

    for(uint c = 1; c < divisions; c++)
    {
      float t = c / (float)divisions;
      Vector3 v2(-cos(t*PI*2.0f)*radia[ch],   -heights[ch],   sin(t*PI*2.0f)*radia[ch]);
      Vector3 v1(-cos(t*PI*2.0f)*radia[ch+1], -heights[ch+1], sin(t*PI*2.0f)*radia[ch+1]);
      Vector3 vn(-cos(t*PI*2.0f),        0.0f,          sin(t*PI*2.0f)); //todo
      drawNormal(vn, v1 + (Vector3)position);
      drawNormal(vn, v2 + (Vector3)position);
      drawNormal(vn, v2 + (Vector3)position);
      drawNormal(vn, v1 + (Vector3)position);
    }

    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v1 + (Vector3)position);
    drawNormal(Vector3(-1.0f, 0.0f, 0.0f), v2 + (Vector3)position);
  }
}

void drawYBlock_drawNormals(Vector3 pos, float radius, float height)
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

void drawXBlock_drawNormals(Vector3 pos, float radius, float startX, float endX)
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

  drawQuad_drawNormals(pos + box[0], pos + box[3], pos + box[2], pos + box[1]); //front
  drawQuad_drawNormals(pos + box[1], pos + box[2], pos + box[6], pos + box[5]); //right
  drawQuad_drawNormals(pos + box[4], pos + box[5], pos + box[6], pos + box[7]); //back
  drawQuad_drawNormals(pos + box[3], pos + box[0], pos + box[4], pos + box[7]); //left
  drawQuad_drawNormals(pos + box[2], pos + box[3], pos + box[7], pos + box[6]); //top
}

void drawBlock_drawNormals(Vector3 pos, float width, float height, float depth)
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

  drawQuad_drawNormals(pos + box[0], pos + box[3], pos + box[2], pos + box[1]); //front
  drawQuad_drawNormals(pos + box[1], pos + box[2], pos + box[6], pos + box[5]); //right
  drawQuad_drawNormals(pos + box[4], pos + box[5], pos + box[6], pos + box[7]); //back
  drawQuad_drawNormals(pos + box[3], pos + box[0], pos + box[4], pos + box[7]); //left
  drawQuad_drawNormals(pos + box[2], pos + box[3], pos + box[7], pos + box[6]); //top
  drawQuad_drawNormals(pos + box[0], pos + box[1], pos + box[5], pos + box[4]); //bottom
}

void drawReverseYBlock_drawNormals(Vector3 pos, float radius, float height)
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

  drawQuad_drawNormals(pos + box[1], pos + box[2], pos + box[3], pos + box[0]); //front
  drawQuad_drawNormals(pos + box[5], pos + box[6], pos + box[2], pos + box[1]); //right
  drawQuad_drawNormals(pos + box[7], pos + box[6], pos + box[5], pos + box[4]); //back
  drawQuad_drawNormals(pos + box[7], pos + box[4], pos + box[0], pos + box[3]); //left
  drawQuad_drawNormals(pos + box[6], pos + box[7], pos + box[3], pos + box[2]); //top
}

void drawQuadList_drawNormals(float* q, uint nQuads)
{
  for(uint c = 0; c < nQuads; c++)
    drawQuad_drawNormals(q+(c*2)*3, q+(c*2+2)*3, q+(c*2+3)*3, q+(c*2+1)*3);
}


#endif
