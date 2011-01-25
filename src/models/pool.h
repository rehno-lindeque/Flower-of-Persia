#ifndef __POOL_H__
#define __POOL_H__

#define USE_DISPLAYLISTS

class Pool : public Model
{
protected:

  void drawOpenXZBlock(float innerRadius, float outerRadius, float startHeight, float endHeight)
  {
    extrudeYQuad(outerRadius, startHeight, endHeight); //outer quads
    drawOpenXZQuad(innerRadius, outerRadius, endHeight); //top quads
  }

  void drawBottom(float innerRadius, float outerRadius, float startHeight, float endHeight, float fog)
  {
    if (GLEW_VERSION_1_4)
    //if(GLEW_EXT_fog_coord)
    {
      glBindTexture(GL_TEXTURE_2D, textures.get(2));
      glEnable(GL_FOG);
      glBegin(GL_QUADS);
        //outer
        //top
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-outerRadius, startHeight, outerRadius); // tl
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f( outerRadius, startHeight, outerRadius); // tr
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f( outerRadius, endHeight,   outerRadius); // br
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-outerRadius, endHeight,   outerRadius); // bl

        //right
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f( outerRadius, startHeight,  outerRadius); // tl
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f( outerRadius, startHeight, -outerRadius); // tr
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f( outerRadius, endHeight,   -outerRadius); // br
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f( outerRadius, endHeight,    outerRadius); // bl

        //bottom
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f( outerRadius, startHeight, -outerRadius); // tl
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-outerRadius, startHeight, -outerRadius); // tr
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-outerRadius, endHeight,   -outerRadius); // br
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f( outerRadius, endHeight,   -outerRadius); // bl

        //left
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-outerRadius, startHeight, -outerRadius); // tl
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-outerRadius, startHeight,  outerRadius); // tr
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-outerRadius, endHeight,    outerRadius); // br
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-outerRadius, endHeight,   -outerRadius); // bl

        //inner
        //top
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f( innerRadius, startHeight, innerRadius); // tl
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-innerRadius, startHeight, innerRadius); // tr
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-innerRadius, endHeight,   innerRadius); // br
        glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f( innerRadius, endHeight,   innerRadius); // bl

        //right
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(innerRadius, startHeight, -innerRadius); // tl
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(innerRadius, startHeight,  innerRadius); // tr
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f(innerRadius, endHeight,    innerRadius); // br
        glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f(innerRadius, endHeight,   -innerRadius); // bl

        //bottom
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-innerRadius, startHeight, -innerRadius); // tl
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f( innerRadius, startHeight, -innerRadius); // tr
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f( innerRadius, endHeight,   -innerRadius); // br
        glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-innerRadius, endHeight,   -innerRadius); // bl

        //left
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-innerRadius, startHeight,  innerRadius); // tl
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(0.0f); glVertex3f(-innerRadius, startHeight, -innerRadius); // tr
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-innerRadius, endHeight,   -innerRadius); // br
        glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog);  glVertex3f(-innerRadius, endHeight,    innerRadius); // bl
      glEnd();

      glBindTexture(GL_TEXTURE_2D, textures.get(1));
      glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 1.0f); glFogCoordf(fog); glVertex3f(-outerRadius, endHeight,  outerRadius); // tl
        glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 1.0f); glFogCoordf(fog); glVertex3f( outerRadius, endHeight,  outerRadius); // tr
        glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f); glFogCoordf(fog); glVertex3f( outerRadius, endHeight, -outerRadius); // br
        glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glFogCoordf(fog); glVertex3f(-outerRadius, endHeight, -outerRadius); // bl
      glEnd();
      glDisable(GL_FOG);
    }
    else
    {
      glBindTexture(GL_TEXTURE_2D, textures.get(1));
      glBegin(GL_QUADS);
        drawYBlock(Vector3(0.0f, 0.0f, 0.0f), 3.9f, -1.0f);
      glEnd();
    }
  }

  /*void drawWater(float x0, float z0, float x1, float z1, float h0, float h1, uint subdivisions, float* heightmap)
  {
    float zPos = z0;
		float xPos = x0;
    float diameter = z1-z0;
    float step = diameter/subdivisions;
    float* map = heightmap;

    for (int cx = 0; cx < subdivisions+1 - 1; cx++)
    {
      zPos = z0;
			for (int cy=0; cy < subdivisions+1 - 1; cy++)
      {
				glColor3f(1.0, 1.0, 1.0);

				// draw the water surface
				glTexCoord2f(xPos/diameter, zPos/diameter);          glVertex3f(xPos, map[cx + cy*(subdivisions+1)], zPos);
				glTexCoord2f((xPos + step)/diameter, zPos/diameter); glVertex3f(xPos + step, map[cx+1 + cy*(subdivisions+1)], zPos);
				glTexCoord2f(xPos/diameter, (zPos + step)/diameter); glVertex3f(xPos, map[cx + (cy+1)*(subdivisions+1)], zPos + step);

				glTexCoord2f((xPos + step)/diameter, zPos/diameter);	         glVertex3f(xPos + step, map[cx+1 + cy*(subdivisions+1)], zPos);
				glTexCoord2f((xPos + step)/diameter, (zPos + step)/diameter); glVertex3f(xPos + step, map[cx+1 + (cy+1)*(subdivisions+1)], zPos + step);
				glTexCoord2f(xPos/diameter, (zPos + step)/diameter);          glVertex3f(xPos, map[cx + (cy+1)*(subdivisions+1)], zPos + step);

        zPos += step;
			}
			xPos += step;
		}
  }*/

public:
#ifdef USE_DISPLAYLISTS
  GLuint displayList, bottomDisplayList;
#else
  VertexBuffer blocks;
#endif

  virtual void build()
  {
    UVGenerator* oldUVMap = uvMap0;

#ifdef USE_DISPLAYLISTS
    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);
      glPushMatrix();

      X_YZUVGenerator poolUV(1.0f);
      uvMap0 = &poolUV;

      //glBindTexture(GL_TEXTURE_2D, textures.get(6));
      glBegin(GL_QUADS);
        drawOpenXZBlock(3.9f, 4.4f, 0.0f, 0.1f);
      glEnd();

      //glBindTexture(GL_TEXTURE_2D, textures.get(0));
      glBegin(GL_QUADS);
        //draw pool architecture
        /*drawOpenXZBlock(1.2f, 1.7f, 0.0f, 1.0f);
        drawOpenXZBlock(0.5f, 0.8f, 1.0f, 1.7f);
        drawBox(Vector3(0.0f, 1.7f+0.2f, 0.0f), 0.2f);
        drawOpenXZBlock(3.9f, 4.4f, 0.0f, 0.1f);*/
        drawOpenXZBlock(1.2f, 1.7f, 0.1f, 1.0f);
        drawOpenXZBlock(0.5f, 0.8f, 1.0f, 1.7f);
        drawBox(Vector3(0.0f, 1.7f+0.2f, 0.0f), 0.2f);
      glEnd();

      glPopMatrix();
    glEndList();
#else
    Geometry blockGeometry;
    blockGeometry << Box(Vector3(0.0f, 1.7f+0.2f, 0.0f), 0.2f);
    blocks << blockGeometry.getCoordinates();

    //glEnableClientState(GL_VERTEX_ARRAY);
    blocks.build();
#endif

    uvMap0 = oldUVMap;

    /*bottomDisplayList = glGenLists(1);
    glNewList(bottomDisplayList, GL_COMPILE);
      glPushMatrix();
      glBegin(GL_QUADS);
        //draw pool a outer bottom
        drawBottom(1.7f, 3.9f, 0.0f, -2.0f);

      glEnd();
      glPopMatrix();
    glEndList();*/

    water1.init(10, 10, 2.0f, 3.9f,   0.2f, -0.2f);
    water2.init(5,   5, 1.0f, 1.2f,  0.08f, 0.85f);
    water3.init(2,   2, 0.5f, 0.5f, 0.035f, 1.60f);
  }

  virtual void render()
  {
    //draw pool architecture
#ifdef USE_DISPLAYLISTS
    glCallList(displayList);
#else
    //blocks.bind();
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, blocks.getObject());
    glVertexPointer(3, GL_FLOAT, 0, null);		// Set The Vertex Pointer To The Vertex Buffer
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glDrawArrays(GL_TRIANGLES, 0, blocks.getNVertices());
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
    //draw pool water

    //draw fountain (streaming) water

    //draw fountain foam particles (where the fountain water hits the water)

    //draw fountain steam?? (perhaps also where the fountain water hits the water)

  }

  void renderBottom()
  {
    //glCallList(bottomDisplayList);

    glPushMatrix();
    drawBottom(1.7f, 3.9f, 0.1f, -2.0f, 0.7f);
    drawBottom(0.8f, 1.2f, 1.0f,  0.0f, 0.5f);
    drawBottom(0.2f, 0.5f, 1.7f,  1.0f, 0.4f);
    glPopMatrix();
  }

  Heightmap water1, water2, water3;

  void renderWater()
  {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures.get(3));
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS);
    /*  drawWater(-);*/
      water3.render();
      water2.render();
      water1.render();

    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
  }

  virtual void renderNormals()
  {
  }

  void update()
  {
    water1.update();
    water2.update();
    water3.update();
  }

  ~Pool() {}
};

#endif
