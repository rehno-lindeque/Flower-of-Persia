#ifndef __LIGHT_H__
#define __LIGHT_H__

class Light
{
public:
  float diffuse[4];
  float position[4];
  float specular[4];
  float ambient[4];
  float range;
  uint lightNum;
  bool enabled;

  Light(const float* diffuse, const float* position, const float* specular, const float* ambient, uint lightNum) : enabled(true), lightNum(lightNum)
  {
    std::memcpy(Light::diffuse, diffuse, sizeof(float)*4);
    std::memcpy(Light::position, position, sizeof(float)*4);
    std::memcpy(Light::specular, specular, sizeof(float)*4);
    std::memcpy(Light::ambient, ambient, sizeof(float)*4);
    range = 0.0f;
  }

  inline void setRange(float range) { Light::range = range; }

  void init()
  {
    if(enabled)
      glEnable(GL_LIGHT0+lightNum);
    glLightfv(GL_LIGHT0+lightNum, GL_POSITION, position);
		glLightfv(GL_LIGHT0+lightNum, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0+lightNum, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0+lightNum, GL_SPECULAR, specular);

    //glLightf(GL_LIGHT0+lightNum, GL_CONSTANT_ATTENUATION, 0.1f);
    //glLightf(GL_LIGHT0+lightNum, GL_LINEAR_ATTENUATION, 0.01f);
    glLightf(GL_LIGHT0+lightNum, GL_QUADRATIC_ATTENUATION, range);
  }

  void onOff()
  {
    enabled = !enabled;
    if(enabled)
      glEnable(GL_LIGHT0+lightNum);
    else
      glDisable(GL_LIGHT0+lightNum);
  }
};

#endif
