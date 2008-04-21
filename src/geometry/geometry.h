#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

class Geometry
{
protected:
  Stream<float> coordinates, normals, textureCoordinates, textureTangents, textureBinormals;
  uint nVertices;
public:
  inline Geometry() : nVertices(0) {}
  inline ~Geometry() {}

  inline Stream<float>& getCoordinates()         { return coordinates; }
  inline Stream<float>& getNormals()             { return normals; }
  inline Stream<float>& getTextureCoordinates()  { return textureCoordinates; }
  inline Stream<float>& getTextureTangents()     { return textureTangents; }
  inline Stream<float>& getTextureBinormals()    { return textureBinormals; }

  inline int getNumberOfVertices() { return nVertices; }

  inline Geometry& operator << (const Geometry& arg) { coordinates << arg.coordinates; normals << arg.normals; textureCoordinates << arg.textureCoordinates; textureTangents << arg.textureTangents; textureBinormals << arg.textureBinormals; return *this; }

  //todo inline void reverse() {}

  //todo: index
};

#endif
