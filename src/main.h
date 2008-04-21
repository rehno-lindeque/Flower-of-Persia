#ifndef __MAIN_H__
#define __MAIN_H__

#include <common/types.h>
#include <common/math/math.h>
#include <GL/glew.h>
#include <GL/gl.h>
//#include <GL/ext/glext.h>

#ifdef _WIN32
  #include "GL/glut.h"
#else
  #include <GL/glut.h>
#endif

extern bool showNormals;
extern bool enableTextures;

#include "stream.h"
#include "random.h"
#include "globals.h"
#include "font.h"
#include "camera.h"
#include "light.h"
#include "wind.h"
#include "gravity.h"

#include "texture/textures.h"
#include "texture/cubemap.h"
#include "texture/shadowmap.h"

#include "uvgenerator.h"
#include "shapes.h"
#include "shapesnormals.h"

#include "geometry/geometry.h"
#include "geometry/tri.h"
#include "geometry/quad.h"
#include "geometry/box.h"

#include "buffers/vertexbuffer.h"
#include "buffers/framebuffer.h"
#include "buffers/cubeframebuffer.h"

#include "models/model.h"
#include "models/heightmap.h"
#include "models/water.h"
#include "models/pool.h"
#include "models/portal.h"
#include "models/pillar.h"
#include "models/cloth.h"
#include "models/floor.h"
#include "models/railing.h"
#include "models/walls.h"
#include "models/roof.h"
#include "models/steps.h"
#include "models/pot.h"
#include "models/singledoor.h"
#include "models/doubledoor.h"
#include "models/arch.h"
#include "models/viewcube.h" // temporary

#include "input/mouse.h"
#include "input/keyboard.h"

#include "shaders/shaders.h"
#include "shaders/standarddiffuse.h"
#include "shaders/attenuateddiffuse.h"
#include "shaders/attenuateddoublesideddiffuse.h"
#include "shaders/attenuateddoublesidedtranslucent.h"
#include "shaders/attenuatedfog.h"
#include "shaders/attenuatedshadowmap.h"
#include "shaders/maskedattenuated.h"
#include "shaders/attenuatedspecularmask.h"
#include "shaders/attenuatednormalspecularmask.h"
#include "shaders/shadowdepth.h"
#include "shaders/diffuseshadowdepth.h"
#include "shaders/cubemapproject.h"

#endif
