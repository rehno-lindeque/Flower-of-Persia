#ifndef __HEIGHTMAP_H__
#define __HEIGHTMAP_H__

class Heightmap : public Model
{
protected:
  unsigned int divisions, randDivisions;
  float textureDivisions;
  float* heightMap,* nextMap;
  float height;
  float radius;
  float zPos;
  uint step, timeSteps, totalSteps;

protected:
  inline Vector2 getUVCoordinate(int xIndex, int yIndex)
  {
    return Vector2(xIndex/(float)divisions * textureDivisions, yIndex/(float)divisions * textureDivisions);
  }

  inline Vector3 getVertex(int xIndex, int yIndex)
  {
    //return Vector3(-radius/divisions+(xIndex-divisions/2.0)*2.0*radius/divisions, heightMap[yIndex*divisions+xIndex], -radius/divisions + (yIndex-divisions/2.0)*2.0*radius/divisions);
    //return Vector3(-radius/divisions+(xIndex-0.5-divisions/2.0)*2.0*radius/divisions, heightMap[yIndex*divisions+xIndex], -radius/divisions + (yIndex-0.5-divisions/2.0)*2.0*radius/divisions);
  
    //float f0 = 1.0f - step / (float)timeSteps;
    //float f1 = step / (float)timeSteps;
    float f0 = (1.0f - step / (float)timeSteps);
    float f1 = (step / (float)timeSteps);


    float x = -radius/divisions+(xIndex-0.5-divisions/2.0)*2.0*radius/divisions;
    float z = -radius/divisions + (yIndex-0.5-divisions/2.0)*2.0*radius/divisions;


    /*return Vector3(-radius/divisions+(xIndex-0.5-divisions/2.0)*2.0*radius/divisions,
                   f0*heightMap[yIndex*divisions+xIndex] + f1*nextMap[yIndex*divisions+xIndex]
                              + sin((sqrt(x*x+z*z)*1.0f/radius + totalSteps/(float)timeSteps)*2.0f)*15.5f*height,
                   -radius/divisions + (yIndex-0.5-divisions/2.0)*2.0*radius/divisions);*/
    return Vector3(-radius/divisions+(xIndex-0.5-divisions/2.0)*2.0*radius/divisions,
                   f0*heightMap[yIndex*divisions+xIndex] + f1*nextMap[yIndex*divisions+xIndex]
                              + sin((sqrt(x*x+z*z)*1.0f/radius + totalSteps/80.0f)*15.0f)*0.3*height,
                   -radius/divisions + (yIndex-0.5-divisions/2.0)*2.0*radius/divisions);
  }

  inline Vector3 getFaceNormal(int xFaceIndex, int yFaceIndex)
  {
    Vector3 first = getVertex(xFaceIndex, yFaceIndex) - getVertex(xFaceIndex+1, yFaceIndex);
    Vector3 second = getVertex(xFaceIndex, yFaceIndex) - getVertex(xFaceIndex, yFaceIndex+1);
    return Vector3::cross(second, first);    
  }

  inline Vector3 getNormal(int xIndex, int yIndex)
  {
    Vector3 compositeNormal = getFaceNormal(xIndex-1, yIndex-1) + getFaceNormal(xIndex, yIndex-1) + getFaceNormal(xIndex, yIndex) + getFaceNormal(xIndex-1, yIndex);
    return compositeNormal.normalize();
  }

public:  
  virtual void build()
  {
  }

  virtual void render()
  {
    if(camera.position(1) > zPos)
      for(uint cx = 1; cx < divisions+1; cx++)
      {
        for(uint cz = 1; cz < divisions+1; cz++)
        {		      
		      // topleft
          glTexCoord2fv(getUVCoordinate(cx, cz));
          glNormal3fv(getNormal(cx, cz));
          glVertex3fv(getVertex(cx, cz));
		      // bottomleft
          glTexCoord2fv(getUVCoordinate(cx, cz+1));
          glNormal3fv(getNormal(cx, cz+1));
		      glVertex3fv(getVertex(cx, cz+1));
		      // bottomright
          glTexCoord2fv(getUVCoordinate(cx+1, cz+1));
          glNormal3fv(getNormal(cx+1, cz+1));
		      glVertex3fv(getVertex(cx+1, cz+1));
		      // topright
          glTexCoord2fv(getUVCoordinate(cx+1, cz));
          glNormal3fv(getNormal(cx+1, cz));
		      glVertex3fv(getVertex(cx+1, cz));
        }
      }

    if(camera.position(1) < zPos+height)
      for(uint cx = 1; cx < divisions+1; cx++)
      {
        for(uint cz = 1; cz < divisions+1; cz++)
        {		      
		      // topleft
          glTexCoord2fv(getUVCoordinate(cx, cz));
          glNormal3fv(-getNormal(cx, cz));
          glVertex3fv(getVertex(cx, cz));
		      // topright
          glTexCoord2fv(getUVCoordinate(cx+1, cz));
          glNormal3fv(-getNormal(cx+1, cz));
		      glVertex3fv(getVertex(cx+1, cz));
          // bottomright
          glTexCoord2fv(getUVCoordinate(cx+1, cz+1));
          glNormal3fv(-getNormal(cx+1, cz+1));
		      glVertex3fv(getVertex(cx+1, cz+1));
          // bottomleft
          glTexCoord2fv(getUVCoordinate(cx, cz+1));
          glNormal3fv(-getNormal(cx, cz+1));
		      glVertex3fv(getVertex(cx, cz+1));
        }
      }
  }

  virtual void renderNormals()
  {
  }

  void init(uint divisions, uint randDivisions, float textureDivisions, float radius, float height, float zPos = 0.0f)
  {
    timeSteps = 15;
    step = 0;
    totalSteps = 0;
    Heightmap::radius = radius;
    Heightmap::divisions = divisions;
    Heightmap::randDivisions = randDivisions;
    Heightmap::textureDivisions = textureDivisions;
    Heightmap::height = height;
    Heightmap::zPos = zPos;
    heightMap = new float[(divisions+2)*(divisions+2)];
    for(unsigned int cy = 0; cy < divisions+2; cy++)
      for(unsigned int cx = 0; cx < divisions+2; cx++)
        heightMap[cy*divisions+cx] = rand()/(double)RAND_MAX*height + zPos;

    nextMap = new float[(divisions+2)*(divisions+2)];
    for(unsigned int cy = 0; cy < divisions+2; cy++)
      for(unsigned int cx = 0; cx < divisions+2; cx++)
        nextMap[cy*divisions+cx] = rand()/(double)RAND_MAX*height + zPos;
  }

  void update()
  {
    ++totalSteps;
    if(step == timeSteps)
    {
      float* tmp = heightMap;
      heightMap = nextMap;
      nextMap = tmp;
      for(unsigned int cy = 0; cy < divisions+2; cy++)
        for(unsigned int cx = 0; cx < divisions+2; cx++)
          nextMap[cy*divisions+cx] = rand()/(double)RAND_MAX*height + zPos;
      step = 0;
    }
    else
      ++step;
  }

  ~Heightmap()
  {
    delete[] heightMap;
    delete[] nextMap;
  }
};


#endif
