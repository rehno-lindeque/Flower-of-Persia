#ifndef __ATTENUATEDSHADOWMAP_H__
#define __ATTENUATEDSHADOWMAP_H__

/* TODO:
  Rename - This should be lightmap.
  Shadowmapping is more commonly known as a depth map rendered from a light. (See DiffuseShadowdepth.h)
  Lightmapping is a static light texture pasted over the geometry.
*/

const char* qattenuatedShadowmapVertexProgram2Lights = "\
varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
varying vec2 shadowTexcoord;\n\
varying vec4 color;\n\
void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  color = gl_Color;\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  vertex_to_light1 = (gl_LightSource[0].position-vertex_in_modelview_space).xyz;\n\
  vertex_to_light2 = (gl_LightSource[1].position-vertex_in_modelview_space).xyz;\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
  shadowTexcoord = vec2(gl_MultiTexCoord1);\n\
}";

const char* qattenuatedShadowmapFragmentProgram2Lights = \
"varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
varying vec2 shadowTexcoord;\n\
uniform sampler2D diffuseTexture;\n\
uniform sampler2D shadowTexture;\n\
varying vec4 color;\n\
void main()\n\
{\n\
  vec4 diffuseColor = texture2D(diffuseTexture, texcoord) * color;\n\
  vec4 shadowColor = texture2D(shadowTexture, shadowTexcoord);\n\
	vec3 normalized_normal = normalize(normal);\n\
	vec3 normalized_vertex_to_light1 = normalize(vertex_to_light1);\n\
  vec3 normalized_vertex_to_light2 = normalize(vertex_to_light2);\n\
  float light1_distance = length(vertex_to_light1); \n\
  float light2_distance = length(vertex_to_light2); \n\
  vec4 light1_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) * gl_LightSource[0].diffuse;\n\
  vec4 light2_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0) * gl_LightSource[1].diffuse;\n\
  /*light1_diffuse /= max(gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance, 1.0);\n\
  light2_diffuse /= max(gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance, 1.0);\n*/\
  light1_diffuse /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\n\
  light2_diffuse /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\n\
  gl_FragColor = diffuseColor * (light1_diffuse + light2_diffuse) * shadowColor;\n\
}";

template<int lights = 2, Attenuation attenuation = QUADRATIC>
class AttenuatedShadowmapShader : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
  int shadowTexture_sampler_uniform_location;
public:
  AttenuatedShadowmapShader() : Shader(qattenuatedShadowmapVertexProgram2Lights, qattenuatedShadowmapFragmentProgram2Lights)
  {}

  void setTexture(int textureID)
  {
    glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
  }
  void setShadowTexture(int textureID)
  {
    glUniform1iARB(shadowTexture_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();    
    diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    shadowTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "shadowTexture");
  }
};

#endif
