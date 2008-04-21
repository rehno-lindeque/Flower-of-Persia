#ifndef __POT_H__
#define __POT_H__

class Pot : public Model
{
public:  
  GLuint displayList;


  virtual void build()
  {    
    uint segments = 20;
    float PotHeight = 7.0f;

    float Pots[][3] = 
    { // left side
      {-18.0f, 1.3f, -25.5f},
      { -3.0f, 0.0f,  7.1f},
      {-18.0f, 20.3f, 5.9f},
      {-11.0f, 0.0f, -4.1f},
      {-26.0f, 20.3f, -5.0f},
      
      //right side
      { 11.5f, 0.0f,   4.0f},
      { 34.0f, 20.3f, -20.0f},
      { 34.0f, 20.3f, -10.0f},
      { 8.0f,  0.5f,  28.0f} };

    float rads[] = 
    { 0.0f, 0.4f, 0.8f, 0.4f, 0.4f, 0.7f };
    
    float heights[] = 
    { 0.0f, 0.0f, 0.8f, 1.2f, 1.6f, 2.0f };

    float heightsReverse[] = 
    { 0.0f, 0.0f, -0.8f, -1.2f, -1.6f, -2.0f };

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);    
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D, textures.get(6));
      //glTranslatef(0.0f, 0.8f, 0.0f);
      glScalef(0.6f, 0.6f, 0.6f);
      //glColor3f(1.0f, 0.85f, 0.7f);
      glBegin(GL_QUADS);        
        for(int c = 0; c < 9; c++)
        {
          //drawYCylinder(Pots[c], 1.0f, PotHeight, segments);
          extrudeYCylinder(Pots[c], segments, rads, heights, 5);          
          extrudeReverseYCylinder(Pots[c], segments, rads, heightsReverse, 5);
        }        
      glEnd();
      glPopMatrix();
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEndList();
  }

  virtual void render()
  {
    //draw railing
    glCallList(displayList);
  }

  virtual void renderNormals()
  {
  }
};


#endif
