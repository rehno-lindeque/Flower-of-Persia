#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

/* At the moment a frame buffer render targets are restricted to cube maps
   Note: Can (and should) also use pbuffers. FBO's seem to be a better version of pbuffers. Unfortunately, it is not yet
   an ARB extension, so be carefull of support.

   Should probably create a general-purpose RenderTarget class that can be instansiated as either a pbuffer or a framebufferobject
*/
class FrameBuffer
{
protected:
  GLuint frameBufferObject;
public:
  FrameBuffer() : frameBufferObject(0) {}
  ~FrameBuffer()
  {
    if(frameBufferObject)
      glDeleteFramebuffersEXT(1, &frameBufferObject);
    frameBufferObject = 0;
  }

  /* Currently, the attachTexture must be a cubemap
  */
  void create(GLuint attachTexture)
  {
    glGenFramebuffersEXT(1, &frameBufferObject);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);

    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, attachTexture, 0);
    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, attachTexture, 0);
    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, attachTexture, 0);
    
    //for view cube:    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, attachTexture, 0);
    
    // restore original frame buffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }

  void bind()
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
  }

  void unbind()
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }
};

#endif
