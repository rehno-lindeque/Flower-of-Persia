#ifndef __ROOF_H__
#define __ROOF_H__

class Roof : public Model
{
protected:
  // Warning: this is actually a reverse cylinder
  void extrudeHalfZCylinder(float* position, uint divisions, float* radia, float* heights, uint heightSegments)
  {
    float a = PI / divisions; // half cylinder    
    for(uint cz = 0; cz < heightSegments; cz++)
    {
      float angle = 0;
      for(uint cx = 0; cx < divisions; cx++)
      {
        glNormal3f( cos(angle),-sin(angle), 0.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle)*radia[cz+1],   sin(angle)*radia[cz+1],   heights[cz+1])); // tl
        glNormal3f( cos(angle),-sin(angle), 0.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle)*radia[cz],     sin(angle)*radia[cz],     heights[cz])); // bl
        glNormal3f( cos(angle+a),-sin(angle+a), 0.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle+a)*radia[cz],   sin(angle+a)*radia[cz],   heights[cz])); // br
        glNormal3f( cos(angle+a),-sin(angle+a), 0.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle+a)*radia[cz+1], sin(angle+a)*radia[cz+1], heights[cz+1])); // tr        
        
        angle += a;
      }      
    }
  }

  // precondition: divisions must be even
  void drawXYCap(float* position, uint divisions, float radius)
  {
    float a = PI / divisions; // half cylinder    
    float angle = 0.0f;
    for(uint cx = 0; cx < divisions/2; cx++)
    {
      glNormal3f( 0.0f, 0.0f, -1.0f); drawVertex3fv((Vector3)position); // origin
      glNormal3f( 0.0f, 0.0f, -1.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle)*radius,     sin(angle)*radius,     0.0f)); // left
      glNormal3f( 0.0f, 0.0f, -1.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle+a)*radius,   sin(angle+a)*radius,   0.0f)); // middle
      glNormal3f( 0.0f, 0.0f, -1.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle+a+a)*radius, sin(angle+a+a)*radius, 0.0f)); // right
      
      angle += 2*a;
    }      
  }

  void drawReverseXYCap(float* position, uint divisions, float radius)
  {
    float a = PI / divisions; // half cylinder    
    float angle = 0.0f;
    for(uint cx = 0; cx < divisions/2; cx++)
    {
      glNormal3f( 0.0f, 0.0f, 1.0f); drawVertex3fv((Vector3)position); // origin
      glNormal3f( 0.0f, 0.0f, 1.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle+a+a)*radius, sin(angle+a+a)*radius, 0.0f)); // right
      glNormal3f( 0.0f, 0.0f, 1.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle+a)*radius,   sin(angle+a)*radius,   0.0f)); // middle
      glNormal3f( 0.0f, 0.0f, 1.0f); drawVertex3fv((Vector3)position + Vector3(-cos(angle)*radius,     sin(angle)*radius,     0.0f)); // left      
      angle += 2*a;
    }      
  }

public:
  GLuint displayList;

  virtual void build()
  {
    uint roofSegments = 12;
    float height = 16.0f;

    float radia[] = { 24.0f, 24.0f, 20.0f };
    float heights[] = { 0.0f, 48.0f, 52.0f };

    float quad[][3] =
    { { 24.0f, 32.0f,-36.0f}, // tl
      { 24.0f, 32.0f,-24.0f}, // bl
      {-24.0f, 32.0f,-24.0f}, // br
      {-24.0f, 32.0f,-36.0f}, // tr
    };

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);    
      glPushMatrix();
      glBegin(GL_QUADS);
        extrudeHalfZCylinder(Vector3(0.0f, 32.0f, -24.0f), roofSegments, radia, heights, 2);
        drawXYCap(Vector3(0.0f, 32.0f, 28.0f), roofSegments, 20.0f);
        drawReverseXYCap(Vector3(0.0f, 32.0f, -24.0f), roofSegments, 24.0f);
        
        drawQuad(quad[0], quad[1], quad[2], quad[3]);
      glEnd();
    glEndList();
  }

  virtual void render()
  {
    glCallList(displayList);
  }

  virtual void renderNormals()
  {
  }
};

#endif
