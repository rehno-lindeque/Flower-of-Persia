#ifndef __UVGENERATOR_H__
#define __UVGENERATOR_H__

class UVGenerator
{
public:
  virtual Vector2 generateUV(Vector3 vertex) = 0;
  inline Vector2 generateUV(float x, float y, float z) { return generateUV(Vector3(x, y, z)); }
};

class XZUVGenerator : public UVGenerator
{
public:
  float scale;
  XZUVGenerator(float scale) : scale(scale) {}
  virtual Vector2 generateUV(Vector3 vertex) { return Vector2(vertex(0), vertex(2)) * scale; }
};

class Y_XZUVGenerator : public UVGenerator
{
public:
  float scale;
  Y_XZUVGenerator(float scale) : scale(scale) {}
  virtual Vector2 generateUV(Vector3 vertex) { return Vector2(vertex(0)+vertex(2), vertex(1)) * scale; }
};

class X_YZUVGenerator : public UVGenerator
{
public:
  float scale;
  X_YZUVGenerator(float scale) : scale(scale) {}
  virtual Vector2 generateUV(Vector3 vertex) { return Vector2(vertex(0), vertex(1)+vertex(2)) * scale; }
};

//todo class vectorUVGenerator : public UVGenerator{}

class QuadUVGenerator : public UVGenerator
{
public:
  uint n;
  Vector2 scale;
  QuadUVGenerator(const Vector2& scale) : scale(scale), n(0) {}
  virtual Vector2 generateUV(Vector3 vertex) { n++; if(n>3) n = 0; return Vector2((float)(n&1) * scale(0), (float)(n&2) * scale(1)); }
};

class QuadGridUVGenerator : public UVGenerator
{
public:
  int xDivisions, yDivisions;
  uint cVert; // vertex index
  int cX, cY; // quad index
  int xDirection;

  QuadGridUVGenerator(int xDivisions, int yDivisions, int xDirection=1) : xDivisions(xDivisions), yDivisions(yDivisions), cX(0), cY(0), cVert(0), xDirection(xDirection)
  {    
    cX = (xDirection==1)? 0 : xDivisions-1;
  }

  virtual Vector2 generateUV(Vector3 vertex)
  { 
    // assume clockwise winding from top-left to bottom-left
    /* bin codes of cVert
       00.  .01
       11.  .10 */
    int endX = (xDirection==1)? xDivisions : -1;
    int startX = (xDirection==1)? 0 : xDivisions-1;

    Vector2 uv((cX + ((cVert&1) ^ ((cVert&2)>>1)))/(float)(xDivisions), (yDivisions-cY-1 + (((~cVert)&2)>>1))/(float)(yDivisions)); 
    if(++cVert > 3)
    {
      cVert = 0;      
      cX += xDirection;
      if(cX == endX)
      { cX = startX; ++cY; }; 
    }
    return uv;
  }
};

class BoxUVGenerator : public UVGenerator
{
public:
  uint n;
  Vector3 scale;
  Vector3 centerOfProjection;
  BoxUVGenerator(const Vector3& scale, const Vector3& centerOfProjection) : scale(scale), centerOfProjection(centerOfProjection) {}
  virtual Vector2 generateUV(Vector3 vertex) 
  { 
    //vertex
    return Vector2(0.0f, 0.0f);
  }
};

class BufferUVGenerator : public UVGenerator
{
public:
  uint c;
  float* buffer;
  BufferUVGenerator(float* buffer) : c(0), buffer(buffer) {}
  virtual Vector2 generateUV(Vector3 vertex) 
  { 
    Vector2 v(buffer[c*2], buffer[c*2+1]);
    ++c;
    return v;
  }
  void reset() { c = 0; }
};

extern UVGenerator* uvMap0; // texture unit 0
extern UVGenerator* uvMap1; // texture unit 0

#endif
