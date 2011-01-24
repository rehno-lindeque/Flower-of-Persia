#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "../tga.h"

extern class Textures
{
protected:
  static const int N_TEXTURES;
  static const char* FILENAMES[/*Textures::N_TEXTURES*/];
  GLuint textures[/*Textures::N_TEXTURES*/10];

public:
  bool loadTexture(const char* filename, int textureIndex)
  {
	  TGAImg tgaImage;
    if(tgaImage.Load(filename) != IMG_OK)
      return false;

	  glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);

	  /*if(tgaImage.GetBPP() == 24)
      glTexImage2D(GL_TEXTURE_2D, 0, 3, tgaImage.GetWidth(), tgaImage.GetHeight(), 0, GL_RGB,GL_UNSIGNED_BYTE, tgaImage.GetImg());
	  else if(tgaImage.GetBPP() == 32)
      glTexImage2D(GL_TEXTURE_2D, 0, 4, tgaImage.GetWidth(), tgaImage.GetHeight(), 0, GL_RGBA,GL_UNSIGNED_BYTE, tgaImage.GetImg());
	  else
		  return false;*/

    //build mip-maps
    // todo: might want to look at GENERATE_MIPMAP instead (to avoid glu uage)
    if(tgaImage.GetBPP() == 24)
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tgaImage.GetWidth(), tgaImage.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, tgaImage.GetImg());
    else if(tgaImage.GetBPP() == 32)
      gluBuild2DMipmaps(GL_TEXTURE_2D, 4, tgaImage.GetWidth(), tgaImage.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, tgaImage.GetImg());
    else
      return false;

	  // Specify filtering and edge actions
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	  return true;
  }

  void load()
  {
	  glGenTextures(N_TEXTURES, textures);

	  for(int c = 0; c < N_TEXTURES; c++)
		  if(!loadTexture(FILENAMES[c], c))
        cout << "Failed to load texture " << FILENAMES[c] << std::endl;
  }

  inline GLuint get(int index) const { return textures[index]; }

} textures;

#endif
