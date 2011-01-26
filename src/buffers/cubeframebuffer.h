#ifndef __CUBEFRAMEBUFFER_H__
#define __CUBEFRAMEBUFFER_H__

/* This frame buffer allows you to bind different faces of a cubemap
*/
class CubeFrameBuffer : public FrameBuffer
{
protected:
  GLuint attachTexture;
  GLuint attachDepthTexture;
public:
  inline CubeFrameBuffer() : attachTexture(0) {}
  void create(GLuint attachTexture, GLuint attachDepthTexture)
  {
    CubeFrameBuffer::attachTexture = attachTexture;
    CubeFrameBuffer::attachDepthTexture = attachDepthTexture;
    glGenFramebuffersEXT(1, &frameBufferObject);
  }

  void bind(GLuint textargetCubeFace)
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textargetCubeFace, attachTexture, 0);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, textargetCubeFace, attachDepthTexture, 0);
  }
};

#endif
