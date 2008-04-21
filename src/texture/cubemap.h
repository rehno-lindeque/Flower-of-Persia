#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

/* An empty cubemap for depth only
  TODO: Make generic: This cubemap is meant, exclusively, for  shadowmapping, so it only supports depth textures and no file loading
*/
class Cubemap
{
protected:
  GLuint texture;

public: 
  Cubemap() : texture(0) {}

  ~Cubemap()
  {
    if(texture != 0)
      glDeleteTextures(1, &texture);
  }

  void build(GLuint internalFormat = GL_RGBA, GLuint format = GL_RGBA, GLuint type = GL_UNSIGNED_BYTE, GLuint length = 512)
  {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    
    // generate cube map texture images
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, length, length, 0, format, type, null);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, length, length, 0, format, type, null);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, length, length, 0, format, type, null);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, length, length, 0, format, type, null);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, length, length, 0, format, type, null);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, length, length, 0, format, type, null);
    //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RED, 256, 256, 0, GL_RED, GL_FLOAT, null);
    //or? glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, 256, 256, , GL_DEPTH_COMPONENT16\24\32, GL_UNSIGNED_BYTE, null);
    
    // temp: The shadow map is used with all geometry and is involved in more than one pass. Thus, 
    // it is given a high priority (for residence).
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_PRIORITY, 1.0); //same as glPrioritizeTextures(1, & texture, 1.0f);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	  //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    // unbind - this seems to be necessary (especially for float textures)???
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }
  
  void bind()
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  }

  void unbind()
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }
  
  inline GLuint getTexture() { return texture; }
};

#endif
