#ifndef __ATTENUATED_DIFFUSE_H__
#define __ATTENUATED_DIFFUSE_H__

const char* qattenuatedDiffuseVertexProgram = "\
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
}";

const char* qattenuatedDiffuseFragmentProgram = \
"varying vec3 normal;\n\
varying vec3 vertex_to_light_vector;\n\
varying vec2 texcoord;\n\
uniform sampler2D diffuseTexture;\n\
varying vec3 realcoord;\n\
void main()\n\
{\n\
  const vec4 DiffuseColor = texture2D(diffuseTexture, texcoord);\n\
	vec3 normalized_normal = normalize(normal);\n\
	vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);\n\
  vec4 light_color = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0) * gl_LightSource[0].diffuse;\n\
  float light_distance = length(vertex_to_light_vector); \n\
  light_color /= max(gl_LightSource[0].quadraticAttenuation * light_distance * light_distance, 1.0);\n\
  gl_FragColor = DiffuseColor * light_color;\n\
}";


const char* qattenuatedDiffuseVertexProgram2Lights = "\
varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
varying vec4 color;\n\
void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  color = gl_Color;\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  vertex_to_light1 = (vec3)(gl_LightSource[0].position-vertex_in_modelview_space);\n\
  vertex_to_light2 = (vec3)(gl_LightSource[1].position-vertex_in_modelview_space);\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
}";

/*const char* qattenuatedDiffuseVertexProgram2Lights = \
"attribute vec3 rm_Tangent;\
attribute vec3 rm_Binormal;"

"varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
varying vec4 color;\n"

"void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  color = gl_Color;\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  vertex_to_light1 = (vec3)(gl_LightSource[0].position-vertex_in_modelview_space);\n\
  vertex_to_light2 = (vec3)(gl_LightSource[1].position-vertex_in_modelview_space);\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
}";*/

/*//This is the diffuse shader with ambient light
const char* qattenuatedDiffuseFragmentProgram2Lights = \
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
  vec4 light1_ambient = (1.0-clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0)) * gl_LightSource[0].ambient;\n\
  vec4 light2_ambient = (1.0-clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0)) * gl_LightSource[1].ambient;\n\
  light1_diffuse /= max(gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance, 1.0);\n\
  light2_diffuse /= max(gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance, 1.0);\n\
  light1_ambient /= max(gl_LightSource[1].quadraticAttenuation * light1_distance * light1_distance, 1.0);\n\
  light2_ambient /= max(gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance, 1.0);\n\
  gl_FragColor = DiffuseColor * (light1_ambient + light2_ambient) + DiffuseColor * (light1_diffuse + light2_diffuse);\n\
}";*/
/*const char* qattenuatedDiffuseFragmentProgram2Lights = \
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
  light1_diffuse /= max(gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance, 1.0);\n\
  light2_diffuse /= max(gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance, 1.0);\n\
  gl_FragColor = DiffuseColor * (light1_diffuse + light2_diffuse);\n\
}";*/
// this takes the diffuse color into account (multiplies it with the tex)
const char* qattenuatedDiffuseFragmentProgram2Lights = \
"varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
uniform sampler2D diffuseTexture;\n\
varying vec4 color;\n\
void main()\n\
{\n\
  const vec4 DiffuseColor = texture2D(diffuseTexture, texcoord) * color;\n\
	vec3 normalized_normal = normalize(normal);\n\
  float light1_distance = length(vertex_to_light1);\n\
  float light2_distance = length(vertex_to_light2);\n\
  /*vec3 normalized_vertex_to_light1 = normalize(vertex_to_light1);\n\
  vec3 normalized_vertex_to_light2 = normalize(vertex_to_light2);\n*/\
  vec3 normalized_vertex_to_light1 = vertex_to_light1/light1_distance;\n\
  vec3 normalized_vertex_to_light2 = vertex_to_light2/light2_distance;\n\
  vec3 light1_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) * gl_LightSource[0].diffuse;\n\
  vec3 light2_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0) * gl_LightSource[1].diffuse;\n\
  /*This used to cause a major performance drop when the camera was very close to the surface:\
  light1_diffuse /= max(gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance, 1.0);\n\
  light2_diffuse /= max(gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance, 1.0);\n*/\
  light1_diffuse /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\n\
  light2_diffuse /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\n\
  /*light1_diffuse.a = 0.5;\n\
  light2_diffuse.a = 0.5;\n*/\
  /*gl_FragColor = DiffuseColor * clamp(vec4(light1_diffuse + light2_diffuse, 1.0), 0.0, 1.0);*/\n\
  gl_FragColor = DiffuseColor * vec4(light1_diffuse + light2_diffuse, 1.0);\n\
}";

enum Attenuation
{
  LINEAR,
  QUADRATIC
};

template<int lights = 1, Attenuation attenuation = QUADRATIC>
class AttenuatedDiffuseShader : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
public:
  AttenuatedDiffuseShader() : Shader(qattenuatedDiffuseVertexProgram, qattenuatedDiffuseFragmentProgram)
  {}

  void setTexture(int textureID)
  {    
    //glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
    glUniform1i(diffuseTexture_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();
    //diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    diffuseTexture_sampler_uniform_location = glGetUniformLocation(programObject, "diffuseTexture");
  }
};

template<>
class AttenuatedDiffuseShader<2, QUADRATIC> : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
public:
  AttenuatedDiffuseShader() : Shader(qattenuatedDiffuseVertexProgram2Lights, qattenuatedDiffuseFragmentProgram2Lights)
  {}

  void setTexture(int textureID)
  {    
    //glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
    glUniform1i(diffuseTexture_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();    
    //diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    diffuseTexture_sampler_uniform_location = glGetUniformLocation(programObject, "diffuseTexture");
  }
};

#endif
