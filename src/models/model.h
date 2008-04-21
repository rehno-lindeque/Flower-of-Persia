#ifndef __MODEL_H__
#define __MODEL_H__

class Model
{
public:
  virtual void build() {}
  virtual void render() = 0;
  virtual void renderNormals() = 0;
  virtual ~Model() {}
};

#endif
