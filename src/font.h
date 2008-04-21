#ifndef __FONT_H__
#define __FONT_H__

class Font
{
protected:
  void* font;
public:
  Font() : font(GLUT_BITMAP_HELVETICA_18) {}
  Font(void* font) : font(font) {}
  
  void drawText(float xPos, float yPos, const_cstring str)
  {    
    glRasterPos2f(xPos, yPos);
	  const char* character = str;
	  while(*character != '\0')
	  {
		  glutBitmapCharacter(font, *character);
		  ++character;
	  }
  }
};

#endif
