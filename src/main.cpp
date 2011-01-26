#include <stdlib.h>

#include <math.h>
#include <iostream>
#include <list>
#include <time.h>
#include <string>
#include <sstream>

#include <common/time.h>
#include "main.h"

#define ESCAPE 27				// Define ASCII value for special

//#define ACCURATE_FPS
//#define ANTIALIAS

// constants
GLint windowWidth = 1280, windowHeight = 1024; // Window dimensions
const int RANGE_MULTIPLIER = 1;
const float WORLD_RANGE = 20.0 * RANGE_MULTIPLIER;
const int Textures::N_TEXTURES = 10;
const char* Textures::FILENAMES[] = { "data/sandstone12.tga", "data/fountainfloor03.tga", "data/fountainsides02.tga", "data/water02.tga", "data/cloth.tga", "data/rail.tga", "data/railtop.tga", "data/tile4.tga", "data/lightmap.tga", "data/railnormal.tga" };

double Cloth::Spring::tensionTensileConst;
double Cloth::Spring::shearTensileConst;
double Cloth::Spring::dampFactor;

bool showFPS = false;
bool enablePhysics = true;
bool showWireframe = false;
bool showNormals = false;
bool enableTextures = true;
bool highDetailCloth = false;
bool enableMouse = false;

// globals
//AttenuatedDiffuseShader<2, QUADRATIC> floorShader;
DiffuseShadowdepthShader<2, QUADRATIC> floorShader;
AttenuatedFogShader<2, QUADRATIC> fogShader;
AttenuatedShadowmapShader<2, QUADRATIC> shadowShader;
//AttenuatedMaskShader<2, QUADRATIC> maskShader;
//AttenuatedSpecularMaskShader<2, QUADRATIC> railShader;
AttenuatedNormalSpecularMaskShader<2, QUADRATIC> railShader;
//AttenuatedDoublesidedDiffuseShader<2, QUADRATIC> clothShader;
AttenuatedDoublesidedTranslucentShader<2, QUADRATIC> clothShader;
ShadowdepthShader shadowdepthShader;

UVGenerator* uvMap0;
UVGenerator* uvMap1;

//* removed4debug:
Wind wind;
Gravity gravity;
Avatar avatar;
Camera camera;
Textures textures;
Light light0(Vector4(0.7f*2.0f, 0.7f*2.0f, 0.65f*2.0f, 1.0f), Vector4(0.0f, 5.0f, -20.0f, 1.0f), Vector4(1.0f, 1.0f, 0.9f, 1.0f), Vector4(0.0006f,   0.0006f, 0.00055f, 1.0f), 0);
Light light1(Vector4(0.2f*2.0f, 0.5f*2.0f,  0.7f*2.0f, 1.0f), Vector4(0.0f,  9.5f,   0.0f, 1.0f), Vector4(0.5f, 0.9f, 1.0f, 1.0f), Vector4(0.00002f, 0.00007f, 0.00009f, 1.0f), 1);
Pool pool;
Floor roomFloor;
Railing railing;
Portal portal;
Pillar pillar;
Walls walls;
Roof roof;
Steps steps;
DoubleDoor doubleDoor;
SingleDoor singleDoor;
Arch arch;
Pot pot;
Cloth cloth1(Vector3(-4.1f, 8.5f, -12.5f), 12, 12, 6.0f, 6.0f, 10.0f);
Cloth cloth2(Vector3(-4.1f, 8.5f, -20.5f), 12, 12, 6.0f, 6.0f, 10.0f);
Cloth cloth3(Vector3(-4.1f, 8.5f, -28.5f), 12, 12, 6.0f, 6.0f, 10.0f);
Cloth cloth4(Vector3( 4.1f, 8.5f, -12.5f), 12, 12, 6.0f, 6.0f, 10.0f);
Cloth cloth5(Vector3( 4.1f, 8.5f, -20.5f), 12, 12, 6.0f, 6.0f, 10.0f);
Cloth cloth6(Vector3( 4.1f, 8.5f, -28.5f), 12, 12, 6.0f, 6.0f, 10.0f);
Keyboard keyboard;
Mouse mouse;

//Cubemap light0Shadowmap;
DepthCubemap light0Shadowmap;
DepthCubemap light1Shadowmap;
FrameBuffer light0renderFrame;

// test view-cube
float tempH = 0.0f;
CubemapProjectShader viewCubeShader;
ViewCube viewCube;
Cubemap viewCubeCubemap;
//DepthCubemap viewCubeDepthmap;
CubeFrameBuffer viewCubeRenderFrame;//*/


#include "scene.h"
Scene scene;

Font font;

// Renders output to screen
float fps;
void display(void)
{
	//*removed4debug: 
  camera.update();//*/

#ifdef ACCURATE_FPS
  glClear(GL_DEPTH_BUFFER_BIT);
#else
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif


  /* testing:
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(33.0, 800.f/600.f, 0.01, 500.0);//*/

  /*
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f(1.0,1.0,1.0);		// White colour
	glNormal3f(0.0, 1.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glBegin(GL_TRIANGLES);
    glVertex3f(-0.5f, 0.5f, -10.f);
    glVertex3f(0.5f,  0.5f, -10.f);
    glVertex3f(0.f,  -0.5f, -10.f);
  glEnd();//*/

  scene.render();

  // draw fps
  if(showFPS)
  {
    //* removed4debug: 
    camera.orthoMode();//*/
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //glColor3f(1.0,1.0,1.0);		// White colour
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    //char fpsStr[15];

    std::stringstream stringStream;
    stringStream << "FPS: " << fps;
    std::string fpsText;
    stringStream >> fpsText;

    font.drawText(20.0, 20.0, fpsText.c_str());
    if(enableTextures)
      glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    //* removed4debug: 
    camera.projectionMode();//*/
  }

#ifdef ACCURATE_FPS
	glFlush();
#else
	glutSwapBuffers();
#endif
}


// Handles basic initialisation and setup of OpenGL parameters.
uint64 oldTime;// = Time::getCounter();
void myinit()
{
  time_t tmp;
	srand(time(&tmp));

  //misc
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	//glClearDepth(0.0f);
	glEnable(GL_DEPTH_TEST);
  glLineWidth(2.0);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

#ifdef ANTIALIAS
  glEnable(GL_MULTISAMPLE);
  //glEnable(GL_MULTISAMPLE_SGIS);
  //?? glSampleCoverage(1.0f, false);
#endif

  // lighting
  //glShadeModel(GL_FLAT);
  glShadeModel(GL_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


  //*removed4debug:

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);// set scene ambient color
  glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
  glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
  glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0);
  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  light0.setRange(0.001f);
  //light0.setRange(1.0f);
  light1.setRange(0.008f);
  light0.init();
  light1.init();

  // fog
  GLfloat fogColor[4] = {0.1f, 0.7f, 0.7f, 1.0f};
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_MODE, GL_EXP);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glFogi(GL_FOG_COORDINATE_SOURCE, GL_FOG_COORDINATE);

  // texturing
  glEnable(GL_TEXTURE_2D);
  textures.load();

  //glClampColorARB(GL_CLAMP_READ_COLOR_ARB, GL_FALSE); //GL_FIXED_ONLY_ARB

  // environment maps
  //light0Shadowmap.build();
  //light1Shadowmap.build();

  // set up render targets
  //light0renderFrame.create(light0Shadowmap.getTexture());

  //* removed4debug:
  // set up (temp) view-cube
  viewCube.build();
  //viewCubeCubemap.build(GL_RGBA, GL_RGBA, GL_FLOAT); //  WRONG!!
  //viewCubeCubemap.build(GL_RGB16\32F_ARB, GL_FLOAT); // requires GL_ARB_texture_float extension
  viewCubeCubemap.build(GL_RGBA16F_ARB, GL_RGBA, GL_FLOAT); // requires GL_ARB_texture_float extension
  viewCubeRenderFrame.create(viewCubeCubemap.getTexture()/*, viewCubeDepthmap.getTexture()*/);
  viewCubeShader.build();//*/

  // set up shaders
  cout << "Floor shader" << endl << "------------" <<endl; 
    floorShader.build(); 
  cout << "Done" << endl;
  cout << "Fog shader" << endl << "----------" <<endl; 
    fogShader.build();
  cout << "Done" << endl;
  cout << "Shadow shader" << endl << "-------------" <<endl; 
    shadowShader.build();
  cout << "Done" << endl;
  //cout << "Mask shader" << endl << "-----------" <<endl; 
  //  maskShader.build();
  //cout << "Done" << endl;
  cout << "Rail shader" << endl << "-----------" <<endl; 
    railShader.build();
  cout << "Done" << endl;
  cout << "Cloth shader" << endl << "------------" <<endl; 
    clothShader.build();
  cout << "Done" << endl;
  cout << "Cloth shader" << endl << "------------" <<endl; 
    shadowdepthShader.build();
  cout << "Done" << endl;
  //*/

  /*
  floorShader.setTexture(textures.get(0));
  fogShader.setTexture(textures.get(2));
  shadowShader.setTexture(textures.get(7));
  shadowShader.setShadowTexture(textures.get(8));
  maskShader.set*/
  //Shader::restoreFixedFunction();

  // set up display lists
  //*removed4debug: 
  scene.build();//*/
  oldTime = Time::getCounter();
}

// Rendered window's reshape function - manages changes to size of the
// window, correctly adjusting aspect ratio.
void changeParentWindow(GLsizei width, GLsizei height)
{
	if(height == 0)
    height = 1;
	glViewport(0, 0, width, height);
	camera.setBounds(width, height);
  windowWidth = width;
  windowHeight = height;
  glLoadIdentity();
}

// Handles all normal keyboard input
void keyInput(unsigned char key, int x, int y)
{
	if (key == ESCAPE)
  {
		exit(0);
    return;
  }

  //keyboard.clear();
  //*removed4debug: 
  keyboard.keys[key] = true;//*/
  
  //cout << "Key: " << key;

  switch(key)
	{
  case ' ':
    if(camera.type == Camera::THIRD_PERSON)
      camera.type = Camera::FIRST_PERSON;
    else if(camera.type == Camera::MOUSE_LOOK)
      camera.type = Camera::THIRD_PERSON;
    else
      camera.type = Camera::MOUSE_LOOK;
    break;
  case 'f': camera.followDistance += 0.5; break;
  case 'r': if(camera.followDistance > 0.5f) camera.followDistance -= 0.5; break;
  case 'm': enableMouse = !enableMouse;
    glutWarpPointer(windowWidth/2, windowHeight/2);
    mouse.x = mouse.y = 0;
	}
}

void idle(void)
{
  //*removed4debug:
  // handle keyboard input
  if(keyboard.keys['w'])
  {
    //std::cout << '[' << avatar.position(0) << ' ' << avatar.position(1) << ' ' << avatar.position(2) << ']';

    //cout << "forward";
    // move forward
    avatar.position += camera.getLookDirection()*0.66f;
    keyboard.keys['w'] = false;

    //std::cout << '[' << avatar.position(0) << ' ' << avatar.position(1) << ' ' << avatar.position(2) << ']' << std::endl;
    //std::cout << "rotation: " << '[' << camera.cameraRotation(0) << ' ' << camera.cameraRotation(1) << ' ' << camera.cameraRotation(2) << ' ' << camera.cameraRotation(3) << ']' << std::endl;
  }

  if(keyboard.keys['s'])
  {
    // move backward
    avatar.position -= camera.getLookDirection()*0.66f;
    keyboard.keys['s'] = false;
  }

  if(keyboard.keys['a'])
  {
    // todo: strafe left
  }
  if(keyboard.keys['d'])
  {
    //todo: strafe right
  }

  // handle mouse input
  if(enableMouse)
  {
    camera.yawLeftRight(mouse.x/5.0f);
    camera.pitchUpDown(mouse.y/5.0f);
  }
  mouse.x = mouse.y = 0;
  //*removed4debug: 
  if(!enableMouse)
    glutWarpPointer(windowWidth/2, windowHeight/2);//*/

  // update time & fps
  uint64 currentTime = Time::getCounter();
  float frameTime = ((uint32)(currentTime - oldTime)) / (double)Time::getFrequency();
  oldTime = currentTime;

  //cout << "time: " << frameTime << std::endl;

  if(frameTime > 0.0f)
    fps = 1.0/frameTime;
  else
    fps = 0.0;

  if(frameTime > 0.03f)
    frameTime = 0.03f; //fixes the delay bug (e.g. right-click, startup)

  //* removed4debug:
  // update physics
  wind.update();
  gravity.update();
  if(enablePhysics)
  {
    if(highDetailCloth)
    {
      cloth1.update(frameTime/2.0); cloth1.update(frameTime/2.0);
      cloth2.update(frameTime/2.0); cloth2.update(frameTime/2.0);
      cloth3.update(frameTime/2.0); cloth3.update(frameTime/2.0);
      cloth4.update(frameTime/2.0); cloth4.update(frameTime/2.0);
      cloth5.update(frameTime/2.0); cloth5.update(frameTime/2.0);
      cloth6.update(frameTime/2.0); cloth6.update(frameTime/2.0);
    }
    else
    {
      cloth1.update(frameTime);
      cloth2.update(frameTime);
      cloth3.update(frameTime);
      cloth4.update(frameTime);
      cloth5.update(frameTime);
      cloth6.update(frameTime);
    }
  }
  pool.update();//*/

  // update display
  glutPostRedisplay();
}

void motion(int x, int y)
{
  //*removed4debug:
  mouse.x += x - windowWidth/2;
  mouse.y += y - windowHeight/2;//*/
}


void demo_menu(int id)
{
	//* removed4debug:
	switch(id)
	{
	case 1:
		showFPS = !showFPS;
    if(showFPS)
      glutChangeToMenuEntry(1, "Hide FPS", 1);
    else
      glutChangeToMenuEntry(1, "Show FPS", 1);
		break;
	case 2:
		enablePhysics = !enablePhysics;
    if(enablePhysics)
      glutChangeToMenuEntry(2, "Disable Physics", 2);
    else
      glutChangeToMenuEntry(2, "Enable Physics", 2);
		break;
  case 3:
		wind.enabled = !wind.enabled;
    if(wind.enabled)
      glutChangeToMenuEntry(3, "Disable Wind", 3);
    else
      glutChangeToMenuEntry(3, "Enable Wind", 3);
		break;
  case 4:
	  gravity.enabled = !gravity.enabled;
    if(gravity.enabled)
      glutChangeToMenuEntry(4, "Disable Gravity", 4);
    else
      glutChangeToMenuEntry(4, "Enable Gravity", 4);
	  break;
  /*case 5:
    if()
      glutChangeToMenuEntry(5, "Disable Volumetric Fog", 5);
    else
      glutChangeToMenuEntry(5, "Enable Volumetric Fog", 5);
    break*
  case 5:
    if(!light0.enabled)
      glutChangeToMenuEntry(5, "Disable Light1", 5);
    else
      glutChangeToMenuEntry(5, "Enable Light1", 5);
    light0.onOff();
    break;
  case 6:
    if(!light1.enabled)
      glutChangeToMenuEntry(6, "Disable Light2", 6);
    else
      glutChangeToMenuEntry(6, "Enable Light2", 6);
    light1.onOff();
    break;
  /*case 8:
    if()
      glutChangeToMenuEntry(8, "Single Cloth", 8);
    else
      glutChangeToMenuEntry(8, "Multiple Cloths", 8);
    break;*
  case 7:
    if(showWireframe)
    {
      glutChangeToMenuEntry(7, "Wireframe Drawing", 7);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
      glutChangeToMenuEntry(7, "Solid Drawing", 7);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    showWireframe = !showWireframe;
    break;
  case 8:
    if(showNormals)
      glutChangeToMenuEntry(8, "Show Normals", 8);
    else
      glutChangeToMenuEntry(8, "Hide Normals", 8);
    showNormals = !showNormals;
    break;
  case 9:
    if(enableTextures)
    {
      glDisable(GL_TEXTURE_2D);
      glutChangeToMenuEntry(9, "Enable Textures", 9);
    }
    else
    {
      glEnable(GL_TEXTURE_2D);
      glutChangeToMenuEntry(9, "Disable Textures", 9);
    }
    enableTextures = !enableTextures;
    break;
  case 10:
    if(highDetailCloth)
      glutChangeToMenuEntry(10, "High Detail Cloth", 10);
    else
      glutChangeToMenuEntry(10, "Low detail Cloth", 10);
    highDetailCloth = !highDetailCloth;
    break;//*/
	}
  oldTime = Time::getCounter();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
#ifdef ACCURATE_FPS
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
#else
  #ifdef ANTIALIAS
	  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  #else
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  #endif
#endif
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("23123576 Rehno Lindeque");
  if(glewInit() != GLEW_OK)
    return 1;

  glutReshapeFunc(changeParentWindow);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyInput);
	glutIdleFunc(idle);

  glutSetCursor(GLUT_CURSOR_NONE);
  glutPassiveMotionFunc(motion);

  glutCreateMenu(demo_menu);
	glutAddMenuEntry("Show FPS", 1);
	glutAddMenuEntry("Disable Physics", 2);
  glutAddMenuEntry("Disable Wind", 3);
  glutAddMenuEntry("Disable Gravity", 4);
  glutAddMenuEntry("Disable Light1", 5);
  glutAddMenuEntry("Disable Light2", 6);
  glutAddMenuEntry("Wireframe Drawing", 7);
  glutAddMenuEntry("Show Normals", 8);
  glutAddMenuEntry("Disable Textures", 9);
  glutAddMenuEntry("High Detail Cloth", 10);
  //glutAddMenuEntry("Disable Volumetric Fog", 11);
  //glutAddMenuEntry("Single Cloth", 12);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	myinit();
  //glutFullScreen();
  glutWarpPointer(windowWidth/2, windowHeight/2);
  oldTime = Time::getCounter();
  glutMainLoop();
  return 0;
}
