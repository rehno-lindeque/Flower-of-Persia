#ifndef __DIFFUSESHADOWDEPTH_H__
#define __DIFFUSESHADOWDEPTH_H__

/*
  NOTES:
    Standard diffuse shading
    2 Lights (takes diffuse color into account)
    Quadric attenuation (for light fall-off)
    1 Shadow "depth" map (for light 0 only)
    No specular \ alpha \ blending etc

  INPUT:
    Vertex
    Normal
    1 texture coord (for color texture)
    1 color texture
    1 shadow "depth" map
*/

const char* diffuseShadowdepthVertexProgram2Lights =
"varying vec3 normal;\n"
"varying vec3 vertex_to_light1;\n"
"varying vec3 vertex_to_light2;\n"
"varying vec2 texcoord;\n"
"varying vec4 color;\n"
"void main()\n"
"{\n"
"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"   color = gl_Color;\n"
"   normal = gl_NormalMatrix * gl_Normal;\n"
"   vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n"
"   vertex_to_light1 = vec3(gl_LightSource[0].position-vertex_in_modelview_space);\n"
"   vertex_to_light2 = vec3(gl_LightSource[1].position-vertex_in_modelview_space);\n"
"   texcoord = vec2(gl_MultiTexCoord0);\n"
"}";

const char* diffuseShadowdepthFragmentProgram2Lights =
"varying vec3 normal;\n"
"varying vec3 vertex_to_light1;\n"
"varying vec3 vertex_to_light2;\n"
"varying vec2 texcoord;\n"

"uniform sampler2D diffuseTexture;\n"
"uniform samplerCube shadowmap;\n"

"varying vec4 color;\n"
"void main()\n"
"{\n"
"  vec4 DiffuseColor = texture2D(diffuseTexture, texcoord) * color;\n"
"  vec3 normalized_normal = normalize(normal);\n"
"  float light1_distance = length(vertex_to_light1);\n"
"  float light2_distance = length(vertex_to_light2);\n"
"  vec3 normalized_vertex_to_light1 = vertex_to_light1/light1_distance;\n"
"  vec3 normalized_vertex_to_light2 = vertex_to_light2/light2_distance;\n"
"  vec3 light1_diffuse = vec3(clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) * gl_LightSource[0].diffuse);\n"
"  vec3 light2_diffuse = vec3(clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0) * gl_LightSource[1].diffuse);\n"
"  light1_diffuse /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\n"
"  light2_diffuse /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\n"
"  float shadowdist = float(textureCube(shadowmap, -vertex_to_light1));\n"
"  if(shadowdist < light1_distance)\n"
"    gl_FragColor = DiffuseColor * vec4(light1_diffuse + light2_diffuse, 1.0);\n"
"  else\n"
"    gl_FragColor = DiffuseColor * vec4(light2_diffuse, 1.0);\n"
"}";

template<int lights = 2, Attenuation attenuation = QUADRATIC>
class DiffuseShadowdepthShader : public Shader {};

template<>
class DiffuseShadowdepthShader<2, QUADRATIC> : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
  int shadowmap_sampler_uniform_location;

public:
  DiffuseShadowdepthShader() : Shader(diffuseShadowdepthVertexProgram2Lights, diffuseShadowdepthFragmentProgram2Lights)
  {}

  void setTexture(int textureID)
  {    
    glUniform1i(diffuseTexture_sampler_uniform_location, textureID);
  }

  void setShadowmap(int textureID)
  {        
    glUniform1i(shadowmap_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();    
    diffuseTexture_sampler_uniform_location = glGetUniformLocation(programObject, "diffuseTexture");
    shadowmap_sampler_uniform_location = glGetUniformLocation(programObject, "shadowmap");
  }
};

#endif
