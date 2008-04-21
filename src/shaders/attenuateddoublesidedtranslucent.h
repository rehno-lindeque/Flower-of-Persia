#ifndef __ATTENUATEDDOUBLESIDEDTRANSLUCENT_H__
#define __ATTENUATEDDOUBLESIDEDTRANSLUCENT_H__

const char* qattenuatedDoubleTranslucentVertexProgram2Lights = 

"uniform vec3 cameraPosition;\n"

"varying vec3 normal;\n"
"varying vec3 vertex_to_light1;\n"
"varying vec3 vertex_to_light2;\n"
"varying vec2 texcoord;\n"
"varying vec4 color;\n"

"void main()\n"
"{\n"
"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"  color = gl_Color;\n"

"  const vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n"
"  const vec3 viewDirection = normalize(vec3(vertex_in_modelview_space) - cameraPosition);\n"

/*"  normal = gl_NormalMatrix * gl_Normal;\n"
"  normal = faceforward(normal, viewDirection, normal);\n"
"  vertex_to_light1 = vec3(gl_LightSource[0].position-vertex_in_modelview_space);\n"
"  vertex_to_light2 = vec3(gl_LightSource[1].position-vertex_in_modelview_space);\n"
"  texcoord = vec2(gl_MultiTexCoord0);\n"*/
"}";

// this takes the diffuse color into account (multiplies it with the tex)
const char* qattenuatedDoubleTranslucentFragmentProgram2Lights = 
"varying vec3 normal;\n"
"varying vec3 vertex_to_light1;\n"
"varying vec3 vertex_to_light2;\n"
"varying vec2 texcoord;\n"
"uniform sampler2D diffuseTexture;\n"
"varying vec4 color;\n"

"void main()\n"
"{\n"
"   const vec4 diffuseColor = texture2D(diffuseTexture, texcoord) * color;\n"
" 	vec3 normalized_normal = normalize(normal);\n"
" 	vec3 normalized_vertex_to_light1 = normalize(vertex_to_light1);\n"
"   float light1_distance = length(vertex_to_light1);\n"
  
"   vec4 light1_diffuse = (clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) + (1.0-diffuseColor.a) * clamp(dot(-normalized_normal, normalized_vertex_to_light1), 0.0, 1.0)) * gl_LightSource[0].diffuse;\n"
"   light1_diffuse /= max(gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance, 1.0);\n"
"   light1_diffuse.a = 1.0;\n"
"   gl_FragColor = diffuseColor * (light1_diffuse);\n"
"}";

template<int lights = 2, Attenuation attenuation = QUADRATIC>
class AttenuatedDoublesidedTranslucentShader : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
  int cameraPosition_uniform_location;

public:
  AttenuatedDoublesidedTranslucentShader() : Shader(qattenuatedDoubleTranslucentVertexProgram2Lights, qattenuatedDoubleTranslucentFragmentProgram2Lights)
  {}

  void setTexture(int textureID)
  {    
    glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
  }
  
  void setCameraPosition(float* cameraPosition)
  {
    glUniform3fv(cameraPosition_uniform_location, 1, cameraPosition);
  }

  void build()
  {
    Shader::build();
    diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    cameraPosition_uniform_location = glGetUniformLocation(programObject, "cameraPosition");
  }
};

#endif
