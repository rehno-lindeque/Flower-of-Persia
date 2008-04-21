#ifndef __SHADERS_H__
#define __SHADERS_H__

/*const char* standardDiffuseVertexShader = "\
/* a simple vertex shader * \
const vec4 blue = vec4(0.0, 0.0, 1.0, 1.0); /*c++ style constructor* \
\
void main(void) \
{ \
  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex; \
  gl_FrontColor = blue; \
}";*/

class ShaderProgram
{
  template<int length, class Type> friend class Uniform;
protected:
  GLuint programObject;
public:
  inline void activate()
  {
    //glUseProgramObjectARB(programObject);
    glUseProgram(programObject);
  }
  inline static void restoreFixedFunction()
  {
    //glUseProgramObjectARB(0);
    glUseProgram(0);
  }
};

class VertexShader : public ShaderProgram
{
protected:
  const char* shaderProgram;
  GLuint shaderObject;
public:
  VertexShader(const char* shaderProgram) : shaderProgram(shaderProgram)
  {}

  void build()
  {
     //initialize shader
    programObject = glCreateProgramObjectARB(); //in OpenGL 2.0, this is glCreateProgram()
    shaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); //in OpenGL 2.0, this is glCreateShader()

    glShaderSourceARB(shaderObject, 1, (const GLcharARB**)&shaderProgram, null);
    glCompileShaderARB(shaderObject);
    glAttachObjectARB(programObject, shaderObject);

    glUseProgramObjectARB(programObject);  //in OpenGL 2.0: glUseProgram
    glLinkProgramARB(programObject); //todo: put before useProgram ??
  }
};

class FragmentShader : public ShaderProgram
{
protected:
  const char* shaderProgram;
  GLuint shaderObject;
public:
  FragmentShader(const char* shaderProgram) : shaderProgram(shaderProgram)
  {}

  void build()
  {
     //initialize shader
    programObject = glCreateProgramObjectARB(); //in OpenGL 2.0, this is glCreateProgram()
    shaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB); //in OpenGL 2.0, this is glCreateShader()

    glShaderSourceARB(shaderObject, 1, (const GLcharARB**)&shaderProgram, null);
    glCompileShaderARB(shaderObject);
    glAttachObjectARB(programObject, shaderObject);

    glUseProgramObjectARB(programObject);  //in OpenGL 2.0: glUseProgram
    glLinkProgramARB(programObject); //todo: put before useProgram ??
  }
};

char shaderLog[256*256];

class Shader : public ShaderProgram
{
protected:
  const char* vertexShaderProgram,* fragmentShaderProgram;
  GLuint vertexShaderObject, fragmentShaderObject;
public:
  Shader(const char* vertexProgram, const char* fragmentProgram) : vertexShaderProgram(vertexProgram), fragmentShaderProgram(fragmentProgram)
  {}

  /*void build()
  {
    //initialize shader
    programObject = glCreateProgramObjectARB();

    vertexShaderObject   = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    fragmentShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    glShaderSourceARB(vertexShaderObject, 1, (const GLcharARB**)&vertexShaderProgram, null);
    glShaderSourceARB(fragmentShaderObject, 1, (const GLcharARB**)&fragmentShaderProgram, null);

    glCompileShaderARB(vertexShaderObject);
    glCompileShaderARB(fragmentShaderObject);

    glAttachObjectARB(programObject, vertexShaderObject);
    glAttachObjectARB(programObject, fragmentShaderObject);

    glLinkProgramARB(programObject);

    GLsizei len;
    glGetInfoLogARB(vertexShaderObject, 256*256, &len, shaderLog);
    glGetInfoLogARB(fragmentShaderObject, 256*256, &len, shaderLog);
    glGetInfoLogARB(programObject, 256*256, &len, shaderLog);
  }*/

  void build()
  {
    //initialize shader
    programObject = glCreateProgram();

    vertexShaderObject   = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderProgram, null);
    glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderProgram, null);

    glCompileShader(vertexShaderObject);
    glCompileShader(fragmentShaderObject);

    glAttachShader(programObject, vertexShaderObject);
    glAttachShader(programObject, fragmentShaderObject);

    glLinkProgram(programObject);

    GLsizei len;
    glGetShaderInfoLog(vertexShaderObject, 256*8, &len, shaderLog);
    glGetShaderInfoLog(fragmentShaderObject, 256*8, &len, shaderLog);
  }
};

template<int length, class Type> class Uniform
{
protected:
  int uniformLocation;
  Shader* parent;
};
template<> class Uniform<1, int>
{
protected:
  int uniformLocation;
  //ShaderProgram* parent;
public:
  //inline Uniform(ShaderProgram* parent);
  inline void init(ShaderProgram* parent, const char* name)
  {
    uniformLocation = glGetUniformLocationARB(parent->programObject, "diffuseTexture");
  }

  inline Uniform& operator = (int value)
  {
    //glUniform1iARB(uniformLocation, value);
    glUniform1i(uniformLocation, value);
    return *this;
  }
};

#endif
