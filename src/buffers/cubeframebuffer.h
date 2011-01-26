#ifndef __CUBEFRAMEBUFFER_H__
#define __CUBEFRAMEBUFFER_H__

/* This frame buffer allows you to bind different faces of a cubemap
*/
class CubeFrameBuffer : public FrameBuffer
{
protected:
  GLuint attachTexture;
  //GLuint attachDepthTexture;
  GLuint renderBufferObject;
public:
  inline CubeFrameBuffer() : attachTexture(0) {}
  void create(GLuint attachTexture/*, GLuint attachDepthTexture*/)
  {
    CubeFrameBuffer::attachTexture = attachTexture;
    //CubeFrameBuffer::attachDepthTexture = attachDepthTexture;
    glGenFramebuffersEXT(1, &frameBufferObject);

    // create a renderbuffer object to store depth info
    glGenRenderbuffersEXT(1, &renderBufferObject);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferObject);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 512, 512);
  }

  void bind(GLuint textargetCubeFace)
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textargetCubeFace, attachTexture, 0);
    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, textargetCubeFace, attachDepthTexture, 0);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderBufferObject);
  }
};

#endif
