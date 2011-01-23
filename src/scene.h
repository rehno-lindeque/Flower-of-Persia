#ifndef __SCENE_H__
#define __SCENE_H__

class Scene
{
protected:
  GLuint staticGeometry;
  list<Cloth*> cloths;
public:
  void build()
  {
    X_YZUVGenerator globalUVGenerator(1/4.0);
    uvMap0 = &globalUVGenerator;

    pillar.build();
    roomFloor.build();
    /*removed4debug:
    pool.build();
    //railing.build(railShader.tangentLocation, railShader.binormalLocation);
    railing.build(railShader.tangentLocation, railShader.binormalLocation);    
    portal.build();
    
    pot.build();
    walls.build();
    roof.build();
    steps.build();
    arch.build();
    singleDoor.build();
    doubleDoor.build();
    //pot.build();
    cloth1.build();
    cloth2.build();
    cloth3.build();
    cloth4.build();
    cloth5.build();
    cloth6.build();

    /*staticGeometry = glGenLists(1);
    glNewList(staticGeometry, GL_COMPILE);
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D, textures.get(0));
      pool.render();
      railing.render();
      portal.render();
      pillar.render();
      walls.render();
      roomFloor.render();
      roof.render();
      steps.render();
      glPopMatrix();
    glEndList();*

    cloths.push_back(&cloth1);
    cloths.push_back(&cloth2);
    cloths.push_back(&cloth3);
    cloths.push_back(&cloth4);
    cloths.push_back(&cloth5);
    cloths.push_back(&cloth6);//*/
  }

    struct comp_model_distance : public binary_function <Cloth*, Cloth*, bool>
    {
#ifdef __GNUC__
      bool operator()(Cloth*& _Left, Cloth*& _Right) const
#else
      bool operator()(const Cloth*& _Left, const Cloth*& _Right) const
#endif
      {
        float leftDistance = (camera.position - _Left->position).getLength();
        float rightDistance = (camera.position - _Right->position).getLength();
        return leftDistance > rightDistance;
        return true;
      }
    };

  void renderCloths()
  {
    cloths.sort(comp_model_distance());
    for(std::list<Cloth*>::iterator i = cloths.begin(); i != cloths.end(); i++)
      (*i)->render();
  }

  void renderEnv()
  {
    /*//removed4debug:
    //glClearColor(0.0, 0.2, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    pillar.render();
    pot.render();
    portal.render();
    arch.render();*/
  }

  void render()
  {
    /* render shadow maps: */
      // render ViewCube (temp)


      /*//removed4debug:

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(0.0, -5.0, 0.0);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glRotatef(180.0, 0.0, 0.0, 1.0);
      gluPerspective(90.0, 1.0, 0.01, 500.0);
      //glTranslatef(0.0, tempH, 0.0);
      glViewport(0, 0, 512, 512);

      //glMatrixMode(GL_MODELVIEW);
      viewCubeCubemap.bind();
      viewCubeRenderFrame.bind(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
        renderEnv();
      viewCubeRenderFrame.bind(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        renderEnv();
      viewCubeRenderFrame.bind(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        renderEnv();
      viewCubeRenderFrame.bind(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        renderEnv();
      viewCubeRenderFrame.bind(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
        /*glLoadIdentity();
        glRotatef(180.0, 0.0, 0.0, 1.0);
        gluPerspective(90.0, 1.0, 0.01, 500.0);
        glTranslatef(0.0, tempH, 0.0);
        glRotatef(180.0, 0.0, 1.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);*
        //glRotatef(180.0, 0.0, 1.0, 0.0);
        //glRotatef(90.0, 0.0, 1.0, 0.0);
        glRotatef(90.0, 0.0, 0.0, 1.0);
        glRotatef(-90.0, 0.0, 1.0, 0.0);
        renderEnv();
      viewCubeRenderFrame.unbind();
      //glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glViewport(0, 0, windowWidth, windowHeight);
      viewCubeShader.activate();
        glTranslatef(0.0, 5.0, 0.0);
        viewCube.render();
        glLoadIdentity();
      Shader::restoreFixedFunction();
      viewCubeCubemap.unbind();

      // get the light at (0,0,0)
      /*glPushMatrix();
      glLoadIdentity();
      //??? glClear(GL_DEPTH_BUFFER_BIT);
      glTranslatef(-light0.position[0], -light0.position[1], -light0.position[2]);

      //todo: set up the projection???

      // activate the shadow map render target
      light0renderFrame.bind();

      // render depths
      shadowdepthShader.activate();
        portal.render();
      Shader::restoreFixedFunction();

      // deactivate the shadow map render target
      light0renderFrame.unbind();

      // reset world view
      glPopMatrix();*/

    /* render scene */
    // set uv mapping
    XZUVGenerator globalUVGenerator(1/8.0);
    uvMap0 = &globalUVGenerator;

    // render static geometry
    //glScalef(2.0f, 2.0f, 2.0f);
    //glCallList(staticGeometry);

    // Cloth railings
    if(enableTextures)
      glBindTexture(GL_TEXTURE_2D, textures.get(0));
    glBegin(GL_QUADS);
      drawZCylinder(Vector3(-4.0f, 8.5f, -22.0f), 0.05f, 28.0f, 5);
      drawZCylinder(Vector3( 4.0f, 8.5f, -22.0f), 0.05f, 28.0f, 5);
    glEnd();

    /*removed4debug:
    pillar.render();
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    railing.render();
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
    glColor3f(0.5f, 0.5f, 0.5f);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    //pot.render();*/

    /*if(enableTextures)
      glBindTexture(GL_TEXTURE_2D, textures.get(0));*/
    //pool.render();
    //portal.render();


    /*if(enableTextures)
      glBindTexture(GL_TEXTURE_2D, textures.get(0));*
    //glDisable(GL_TEXTURE_2D);
    //perPixelDiffuseFragmentShader.activate();
    //3621072
    */
    
    floorShader.activate();

    // bind color texture to texture unit #0
    floorShader.setTexture(0);

    /*// bind shadow map to texture unit #2
    floorShader.setShadowmap(2);
    glActiveTexture(GL_TEXTURE2);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, light0Shadowmap.getTexture());
    glActiveTexture(GL_TEXTURE0);*/

    pillar.render();
    /*pot.render();
    //if(enableTextures)
      glBindTexture(GL_TEXTURE_2D, textures.get(0));
    pool.render();
    portal.render();
    walls.render();*/
    roomFloor.render();
    /*steps.render();
    roof.render();
    arch.render();
    singleDoor.render();
    doubleDoor.render();
    railShader.activate();
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures.get(9));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.get(5));

        railShader.setCameraPosition(camera.position);
        railing.render();
      //glDisable(GL_BLEND);
      fogShader.activate();
        pool.renderBottom();
      shadowShader.activate();
        shadowShader.setTexture(0);
        shadowShader.setShadowTexture(1);
        glActiveTexture(GL_TEXTURE0); //multi-texturing
        glBindTexture(GL_TEXTURE_2D, textures.get(7));
        glActiveTexture(GL_TEXTURE1); //multi-texturing
        //glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures.get(8));
        roomFloor.renderPlatforms();
      //FragmentShader::restoreFixedFunction();

      //steps.render();
      //roof.render();
      //arch.render();
      //singleDoor.render();
      //doubleDoor.render();
      //pool.renderBottom();

      if(camera.position(2) > -8.0)
      {
        // render cloths
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
        if(enableTextures)
          glBindTexture(GL_TEXTURE_2D, textures.get(4));

          clothShader.activate();
            clothShader.setCameraPosition(camera.position);
            glBegin(GL_QUADS);
              renderCloths();
            glEnd();
          FragmentShader::restoreFixedFunction();
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        pool.renderWater();
      }
      else
      {
        FragmentShader::restoreFixedFunction();
        pool.renderWater();

        // render cloths
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
        if(enableTextures)
          glBindTexture(GL_TEXTURE_2D, textures.get(4));

          clothShader.activate();
            clothShader.setCameraPosition(camera.position);
            glBegin(GL_QUADS);
              renderCloths();
            glEnd();
          FragmentShader::restoreFixedFunction();
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
      }

    /* render normals *
      if(showNormals)
      {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 0.0f, 0.0f);
        pool.renderNormals();
        railing.renderNormals();
        portal.renderNormals();
        pillar.renderNormals();
        walls.renderNormals();
        roomFloor.renderNormals();
        steps.renderNormals();
        roof.renderNormals();
        arch.renderNormals();
        singleDoor.renderNormals();
        doubleDoor.renderNormals();
        //pot.renderNormals
        glBegin(GL_LINES);
          cloth1.renderNormals();
          cloth2.renderNormals();
          cloth3.renderNormals();
          cloth4.renderNormals();
          cloth5.renderNormals();
          cloth6.renderNormals();
        glEnd();
        glEnable(GL_LIGHTING);
        if(enableTextures)
          glEnable(GL_TEXTURE_2D);
      }*/
    }
};

#endif
