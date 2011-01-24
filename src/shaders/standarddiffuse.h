#ifndef __STANDARD_DIFFUSE_H__
#define __STANDARD_DIFFUSE_H__

// This is a per-pixel diffuse shader

const char* diffuseVertexProgram = "\
varying vec3 normal;\n\
varying vec3 vertex_to_light_vector;\n\
varying vec2 texcoord;\n\
void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  vertex_to_light_vector = (gl_LightSource[0].position-vertex_in_modelview_space).xyz;\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
}";

const char* diffuseFragmentProgram = \
"varying vec3 normal;\n\
varying vec3 vertex_to_light_vector;\n\
varying vec2 texcoord;\n\
uniform sampler2D diffuseTexture;\n\
varying vec3 realcoord;\n\
void main()\n\
{\n\
	vec4 AmbientColor = vec4(0.0, 0.0, 0.0, 1.0);\n\
  vec4 DiffuseColor = texture2D(diffuseTexture, texcoord);\n\
	vec3 normalized_normal = normalize(normal);\n\
	vec3 normalized_vertex_to_light_vector = normalize(vertex_to_light_vector);\n\
  vec4 light_color = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0) * gl_LightSource[0].diffuse;\n\
  gl_FragColor = AmbientColor + DiffuseColor * light_color;\n\
}";

const char* diffuseVertexProgram2Lights = "\
varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  vertex_to_light1 = (gl_LightSource[0].position-vertex_in_modelview_space).xyz;\n\
  vertex_to_light2 = (gl_LightSource[1].position-vertex_in_modelview_space).xyz;\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
}";

const char* diffuseFragmentProgram2Lights = \
"varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
uniform sampler2D diffuseTexture;\n\
varying vec3 realcoord;\n\
void main()\n\
{\n\
	vec4 AmbientColor = vec4(0.0, 0.0, 0.0, 1.0);\n\
  vec4 DiffuseColor = texture2D(diffuseTexture, texcoord);\n\
	vec3 normalized_normal = normalize(normal);\n\
	vec3 normalized_vertex_to_light1 = normalize(vertex_to_light1);\n\
  vec3 normalized_vertex_to_light2 = normalize(vertex_to_light2);\n\
  vec4 light1_color = clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) * gl_LightSource[0].diffuse;\n\
  vec4 light2_color = clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0) * gl_LightSource[1].diffuse;\n\
  gl_FragColor = AmbientColor + DiffuseColor * (light1_color + light2_color);\n\
}";

template<int lights = 1>
class StandardDiffuseShader : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
public:
  StandardDiffuseShader() : Shader(diffuseVertexProgram, diffuseFragmentProgram)
  {}

  void setTexture(int textureID)
  {
    glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();
    diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textures.get(0));
  }
};

template<>
class StandardDiffuseShader<2> : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
public:
  StandardDiffuseShader() : Shader(diffuseVertexProgram2Lights, diffuseFragmentProgram2Lights)
  {}

  void setTexture(int textureID)
  {    
    glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
  }

  void build()
  {
    Shader::build();
    diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textures.get(0));
  }
};

#endif
