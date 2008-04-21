#ifndef __ATTENUATEDFOG_H__
#define __ATTENUATEDFOG_H__

const char* qattenuated2LightsFogDiffuseVertexProgram = "\
varying vec3 normal;\n\
varying vec3 vertex_to_light_vector;\n\
varying vec2 texcoord;\n\
void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  vertex_to_light_vector = (vec3)(gl_LightSource[0].position-vertex_in_modelview_space);\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
  gl_FogFragCoord = gl_FogCoord;\n\
}";

// Note that this does not take the gl_Color into account
// Fog is attenuated quadratically
const char* qattenuated2LightsFogDiffuseFragmentProgram = \
"varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
uniform sampler2D diffuseTexture;\n\
varying vec3 realcoord;\n\
void main()\n\
{\n\
  const vec4 DiffuseColor = texture2D(diffuseTexture, texcoord);\n\
	vec3 normalized_normal = normalize(normal);\n\
	vec3 normalized_vertex_to_light1 = normalize(vertex_to_light1);\n\
  vec3 normalized_vertex_to_light2 = normalize(vertex_to_light2);\n\
  float light1_distance = length(vertex_to_light1); \n\
  float light2_distance = length(vertex_to_light2); \n\
  vec4 light1_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) * gl_LightSource[0].diffuse;\n\
  vec4 light2_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0) * gl_LightSource[1].diffuse;\n\
  light1_diffuse /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\n\
  light2_diffuse /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\n\
  gl_FragColor = mix((DiffuseColor * (light1_diffuse + light2_diffuse)), gl_Fog.color, gl_FogFragCoord*gl_FogFragCoord);\n\
  //gl_FragColor = DiffuseColor * (light1_diffuse + light2_diffuse);\n\
  //gl_FragColor = gl_Fog.color;\n\
}";

template<int lights = 2, Attenuation attenuation = QUADRATIC>
class AttenuatedFogShader : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
public:
  AttenuatedFogShader() : Shader(qattenuated2LightsFogDiffuseVertexProgram, qattenuated2LightsFogDiffuseFragmentProgram)
  {}

  void setTexture(int textureID)
  {
    diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();    
  }
};

#endif
