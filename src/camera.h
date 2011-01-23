#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <common/math/unitquaternion.h>

extern class Avatar
{
public:
	Vector3 position;
	float angle;

	Avatar() : position(0.0f, 17.0f, -72.0f), angle((float)PI/4)
	{}

	void draw()
	{
		glPushMatrix();
		//glLoadIdentity();
		glTranslatef(position(0), position(1), position(2));
		glRotatef(angle/PI*180.0f, 0.0f, 1.0f, 0.0f);
		glutWireSphere(0.5f, 10, 10);
		glPopMatrix();
	}
} avatar;

extern class Camera
{
public:
  // Camera physical stats
  //todo: float maxSpeed;
  //todo: float maxAngularSpeed;

	enum Type	{ THIRD_PERSON, FIRST_PERSON, MOUSE_LOOK } type;
	Vector3 position;
	Vector3 lookAt;
	int width, height;
  float followDistance;

  //MOUSE_LOOK variables
  float pitch;
  float yaw;
  UnitQuaternion cameraRotation;

  Camera() : cameraRotation(Vector3(1.0f, 0.0f, 0.0f), Angle(0.0f))
	{
    type = MOUSE_LOOK;

		position(0) = 0.0;
		position(1) = 2.0;
		position(2) = -4.0;

		lookAt(0) = 0.0;
		//lookAt(1) = -0.5;
		lookAt(1) = 0.0;
		//lookAt(2) = 1.0;
		lookAt(2) = 0.0;
		//lookAt.normalize();

    followDistance = sqrt(33.0);
    //followDistance = 100.0f;

    //MOUSE_LOOK variables
    //rotX = rotY = 0.0f;
    yaw = 180.0f;
    pitch = 7.0f;
    //avatar.position = Vector3(0.0f, 8.0f, 50.0f);
    //pitch = 180.0f;
    //pitch = yaw = 180.0f;
    cameraRotation = UnitQuaternion(Vector3(1.0f, 0.0f, 0.0f), Angle(pitch/180.0f*PI)) * UnitQuaternion(Vector3(0.0f, 1.0f, 0.0f), Angle(yaw/180.0f*PI));
	}

  void pitchUpDown(float degrees)
  {
    // rotate in the direction of the camera's up vector
    // = rotation around the the camera's rotation axis
    // = simply add to the axis-angle rotation

    /*Vector3 rotationAxis = lookDirection.getAARotationAxis().getNormalized();
    Angle angle = lookDirection.getAARotationAngle();
    angle += degrees;
    lookDirection = UnitQuaternion(rotationAxis, angle);*/
    //lookDirection *= UnitQuaternion(lookDirection.getAARotationAxis().getNormalized(), Angle(degrees/180.0f*PI));
    pitch += degrees;
  }

  void yawLeftRight(float degrees)
  {
    //lookDirection *= UnitQuaternion(Vector3(0.0f, 1.0f, 0.0f), Angle(degrees/180.0f*PI));
    yaw += degrees;
  }

	void update()
	{
    glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
    //gluPerspective(33.0, width/(GLfloat)height, 0.01, 500.0);
    gluPerspective(33.0, 800.f/600.f, 0.01, 500.0);
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
		switch(type)
		{
		case THIRD_PERSON:
		  {
			  lookAt = avatar.position;

			  Vector3 relativePos(-sin(avatar.angle), 0.8f, -cos(avatar.angle));
			  relativePos.normalize();
			  relativePos *= followDistance;
			  position = avatar.position + relativePos;
        gluLookAt(position(0), position(1), position(2), lookAt(0), lookAt(1), lookAt(2), 0.0, 1.0, 0.0);
			  break;
		  }
		case FIRST_PERSON:
      {
        position = avatar.position;
        glRotatef(-avatar.angle/PI*180.0+180.0, 0.0, 1.0, 0.0);
        glTranslatef(-position(0), -position(1), -position(2));
			  break;
      }

    case MOUSE_LOOK:
      {
        position = avatar.position + Vector3(0.0f, 0.0f, 0.0f);
        /*glRotatef(rotX, 0.0, 1.0, 0.0);
        glRotatef(rotY, 0.0, 0.0, 1.0);*/

        //*
        cameraRotation = UnitQuaternion(Vector3(1.0f, 0.0f, 0.0f), Angle(pitch/180.0f*PI)) * UnitQuaternion(Vector3(0.0f, 1.0f, 0.0f), Angle(yaw/180.0f*PI));
        Matrix4 m;
        cameraRotation.getTransformation(m);
        glMultMatrixf(m);
        std::cout << '[' << m(0,0) << ' ' << m(0,1) << ' ' << m(0,2) << ' ' << m(0,3) << std::endl
                  << ' ' << m(1,0) << ' ' << m(1,1) << ' ' << m(1,2) << ' ' << m(1,3) << std::endl
                  << ' ' << m(2,0) << ' ' << m(2,1) << ' ' << m(2,2) << ' ' << m(2,3) << std::endl
                  << ' ' << m(3,0) << ' ' << m(3,1) << ' ' << m(3,2) << ' ' << m(3,3) << ']' << std::endl;
        /*glMultMatrixf((Matrix4)cameraRotation);
        //std::cout << '[' << position(0) << ' ' << position(1) << ' ' << position(2) << ']';
        glTranslatef(-position(0), -position(1), -position(2));*/
			  break;
      }
		}

		glMatrixMode(GL_MODELVIEW);
	}

  void orthoMode()
  {
    glMatrixMode(GL_PROJECTION);
	  glPushMatrix();
	  glLoadIdentity();
	  gluOrtho2D(0, width, 0, height);
	  glScalef(1, -1, 1);
	  glTranslatef(0, -1.0*height, 0);
	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();
  }

  void projectionMode()
  {
    update();
  }

	void setBounds(int width, int height)
	{
		Camera::width = width;
		Camera::height = height;
		update();
	}

  Vector3 getLookDirection() const
  {
    Matrix4 m;
    (-cameraRotation).getTransformation(m);
    return Vector3(0.0f, 0.0f, -1.0f).getTransform(m);
  }
} camera;

#endif
