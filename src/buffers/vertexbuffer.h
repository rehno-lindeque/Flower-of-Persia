#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <vector>
using namespace std;

class VertexBuffer
{
protected:
 	GLuint vertexBufferObject;
  uint nVertices;
  vector<float> vertexData;

public:
  VertexBuffer() : vertexBufferObject(0), nVertices(0) {}

  ~VertexBuffer()
  {
    if(vertexBufferObject != 0)
      glDeleteBuffers(1, &vertexBufferObject);
    vertexBufferObject = 0;
  }

  inline VertexBuffer& operator<<(float value)
  {
    vertexData.push_back(value);
    return *this;
  }

  inline VertexBuffer& operator<<(Stream<float> stream)
  {
    for(int c = 0; c < stream.getLength(); ++c)
      vertexData.push_back(((float*)stream)[c]);
    return *this;
  }

  void build()
  {
    glGenBuffersARB(1, &vertexBufferObject);
	  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBufferObject);

    //Load vertexData into the buffer
    /*OLD:
    //Todo: This is very inefficient, but it doesn't matter too much since it only happens at load time
    float* tmpBuffer = new float[vertexData.size()];
    for(int c = 0; c < vertexData.size(); ++c)
      tmpBuffer[c] = vertexData[c];
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexData.size()*sizeof(float), tmpBuffer, GL_STATIC_DRAW_ARB);
    delete tmpBuffer;
    nVertices = vertexData.size() / 3;
    vertexData.clear();*/

    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexData.size()*sizeof(float), null, GL_STATIC_DRAW_ARB);
    float* tmpBuffer = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
    for(int c = 0; c < vertexData.size(); ++c)
      tmpBuffer[c] = vertexData[c];
    glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    tmpBuffer = null;
    nVertices = vertexData.size() / 3;
    vertexData.clear();
  }

  void bind()
  {
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBufferObject);
  }

  inline GLuint getObject() { return vertexBufferObject; }

  inline uint getNVertices() const { return nVertices; }
};

#endif
