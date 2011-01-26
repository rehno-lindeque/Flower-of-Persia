#ifndef __DEPTHCUBEMAP_H__
#define __DEPTHCUBEMAP_H__

class DepthCubemap : public Cubemap
{
public:
  /*void setRenderTarget()
  {
    
  }*/
  void build()
  {
    //Cubemap::build(GL_DEPTH_COMPONENT, GL_FLOAT);
    //Cubemap::build(GL_DEPTH_COMPONENT, GL_FLOAT);
    //Cubemap::build(GL_RGBA16F_ARB, GL_FLOAT);
    //Cubemap::build(GL_LUMINANCE16F_ARB, GL_LUMINANCE, GL_FLOAT);
    Cubemap::build(GL_LUMINANCE16F_ARB, GL_DEPTH_COMPONENT, GL_FLOAT);
  }
};

#endif

