#ifndef __CUBEMAPPROJEC_H__
#define __CUBEMAPRENDER_H__

/*
  This is for mapping an object directly to a cube map (Projects cube map straight onto object using
  local untransformed coordinates)

  NOTES:
    Cubemap texture
    No shading \ blending etc

  INPUT:
    Vertex
    1 color cube map
*/

const char* cubemapProjectVertexProgram = 
"varying vec3 localCoord;"
"void main()\
{\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
  //"localCoord = gl_Vertex;"/* - vec3(1.0, 0.0, 0.0);*/
  "localCoord = vec3(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z);"
"}";

const char* cubemapProjectFragmentProgram =
"varying vec3 localCoord;\
uniform samplerCube envmap;"

"void main()\
{\
  gl_FragColor = textureCube(envmap, localCoord);\
}";

class CubemapProjectShader : public Shader
{
protected:
  int envmap_sampler_uniform_location;

public:
  CubemapProjectShader() : Shader(cubemapProjectVertexProgram, cubemapProjectFragmentProgram)
  {}

  void setEnvmap(int textureID)
  {        
    glUniform1i(envmap_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();    
    envmap_sampler_uniform_location = glGetUniformLocation(programObject, "envmap");
  }
};

#endif
