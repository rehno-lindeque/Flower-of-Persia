#ifndef __SCENEGRAPH_H__
#define __SCENEGRAPH_H__

class SceneGraph
{
public:
  vector<SceneNode*> nodes;

  virtual void draw()
  {
    //for()
  }

  ~SceneGraph()
  {
    delete nodes;
  }
};

class SceneNode
{
public:
  virtual void apply() = 0;
};

class SceneTransformation : public SceneNode
{
public:
  void apply();
};

#endif
