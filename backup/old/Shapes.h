#ifndef __SHAPES_H__
#define __SHAPES_H__

#define SHAPE_ACCURACY 1.0f

enum ShapeFacing { INWARDS, OUTWARDS };
enum ShapeShading { SMOOTH_SHADING, FLAT_SHADING };

void drawQuad(float* point1, float* point2, float* point3, float* point4, float uStart = 0.0f, float vStart = 0.0f, float uEnd = 1.0f, float vEnd = 1.0f)
{
  //calculate normal
  Vector3 v1 = (Vector3)point2-(Vector3)point1;
  Vector3 v2 = (Vector3)point4-(Vector3)point1;
  Vector3 nv = Vector3::cross(v1.normalize(), v2.normalize());

  //draw
  glNormal3fv(nv);
  glTexCoord2f(uStart, vStart); glVertex3fv(point1);
  glTexCoord2f(uStart, vEnd);   glVertex3fv(point2);
  glTexCoord2f(uEnd,   vEnd);   glVertex3fv(point3);
  glTexCoord2f(uEnd,   vStart); glVertex3fv(point4);
}

void extrudeYLine(float* point1, float* point2, float height, float uStart = 0.0f, float vStart = 0.0f, float uEnd = 1.0f, float vEnd = 1.0f)
{
  //note: point1[1] should == point2[1] (i.e. same y-values)

  //calculate normal vector
  Vector3 v1 = Vector3(point1[0], point1[1]+height, point1[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 v2 = Vector3(point2[0], point2[1],        point2[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 nv = Vector3::cross(v1.normalize(), v2.normalize());
  
  //draw
  glNormal3fv(nv);
  glTexCoord2f(uStart, vStart); glVertex3f(point1[0], point1[1],        point1[2]);
  glTexCoord2f(uStart, vEnd);   glVertex3f(point1[0], point1[1]+height, point1[2]);
  glTexCoord2f(uEnd,   vEnd);   glVertex3f(point2[0], point2[1]+height, point2[2]);
  glTexCoord2f(uEnd,   vStart); glVertex3f(point2[0], point2[1],        point2[2]);
}


void extrudeYSegments(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments; ++cSegment)
    extrudeYLine(&shapeVertices[3*(cSegment*2)], &shapeVertices[3*(cSegment*2+1)], height);
}

void extrudeYShape(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeYLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], height);
}

void extrudeClosedYShape(float* shapeVertices, int nSegments, float height)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
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

void drawZTrapesium(float basisRadius, float topRadius, float startZ, float endZ, float yHeight)
{  
  if(startZ < endZ)
    glNormal3f(0.0f, 1.0f, 0.0f);   // it points upwards
  else
    glNormal3f(0.0f, -1.0f, 0.0f);  // it points downwards

  //draw
  glVertex3f(-basisRadius, yHeight, startZ);
  glVertex3f(-topRadius,   yHeight, endZ);
  glVertex3f( topRadius,   yHeight, endZ);
  glVertex3f( basisRadius, yHeight, startZ);
}

void drawXTrapesium(float basisRadius, float topRadius, float startX, float endX, float yHeight)
{
  if(startX < endX)
    glNormal3f(0.0f, 1.0f, 0.0f);   // it points upwards
  else
    glNormal3f(0.0f, -1.0f, 0.0f);  // it points downwards

  glVertex3f(startX, yHeight,  basisRadius);
  glVertex3f(endX,   yHeight,  topRadius);
  glVertex3f(endX,   yHeight, -topRadius);
  glVertex3f(startX, yHeight, -basisRadius);
}

void extrudeXZLine(float* point1, float* point2, float scale, float uStart = 0.0f, float vStart = 0.0f, float uEnd = 1.0f, float vEnd = 1.0f)
{
  //calculate normal vector
  /*Vector3 v1 = Vector3(point1[0], point1[1]-xzHeight, point1[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 v2 = Vector3(point2[0], point2[1],          point2[2]) - Vector3(point1[0], point1[1], point1[2]);
  Vector3 nv = Vector3::cross(v1.normalize(), v2.normalize());*/
  Vector3 nv(0.0f, 1.0f, 0.0f);
  
  //draw
  glNormal3fv(nv);
  /*glTexCoord2f(uStart, vStart); glVertex3f(point1[0],       point1[1], point1[2]);
  glTexCoord2f(uStart, vEnd);   glVertex3f(point1[0]*scale, point1[1], point1[2]*scale);
  glTexCoord2f(uEnd,   vEnd);   glVertex3f(point2[0]*scale, point2[1], point2[2]*scale);
  glTexCoord2f(uEnd,   vStart); glVertex3f(point2[0],       point2[1], point2[2]);*/

  glTexCoord2f(uEnd,   vStart); glVertex3f(point2[0],       point2[1], point2[2]);
  glTexCoord2f(uEnd,   vEnd);   glVertex3f(point2[0]*scale, point2[1], point2[2]*scale);
  glTexCoord2f(uStart, vEnd);   glVertex3f(point1[0]*scale, point1[1], point1[2]*scale);
  glTexCoord2f(uStart, vStart); glVertex3f(point1[0],       point1[1], point1[2]);  
}

void extrudeXZShape(float* shapeVertices, int nSegments, float scale)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
    extrudeXZLine(&shapeVertices[3*cSegment], &shapeVertices[3*(cSegment+1)], scale);
}

void extrudeClosedXZShape(float* shapeVertices, int nSegments, float scale)
{
  for(int cSegment = 0; cSegment < nSegments-1; ++cSegment)
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

void drawYCylinder(float* position, float radius, uint divisions)
{
  
}

void extrudeYCylinder(float* position, uint divisions, float* radia, float* heights, uint heightSegments)
{
}

#endif
