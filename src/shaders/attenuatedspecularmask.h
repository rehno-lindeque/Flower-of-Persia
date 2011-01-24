#ifndef __ATTENUATEDSPECULARMASK_H__
#define __ATTENUATEDSPECULARMASK_H__

//NOTE: This shader is for double-sided surfaces
//      The normal is calculated based on which side of the polygon the viewer is
/*const char* qattenuatedNormalSpecularMaskVertexProgram2Lights = "\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
varying vec4 color;\n\
uniform vec3 cameraPosition;\n\
varying vec3 viewDirection;\n\
void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  color = gl_Color;\n\
  /*vec3 viewDirection = vec3(gl_ModelViewMatrix*gl_Vertex - gl_ProjectionMatrixTranspose[3]);*\n\
  /*vec3 viewDirection = vec3( gl_ModelViewMatrix * gl_Vertex);*\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  viewDirection = normalize(vec3(vertex_in_modelview_space) - cameraPosition);\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  normal = faceforward(normal, viewDirection, normal);\n\
  vertex_to_light1 = normalize((gl_LightSource[0].position-vertex_in_modelview_space).xyz);\n\
  vertex_to_light2 = normalize((gl_LightSource[1].position-vertex_in_modelview_space).xyz);\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
  viewDirection = normalize(vec3(vertex_in_modelview_space) - cameraPosition);\n"
  //transform everything, so that the normal == (0.0, 1.0, 0.0) (for bump-mapping)
    //transform view direction
"   viewDirection
  }
}";*/

const char* qattenuatedSpecularMaskVertexProgram2Lights =
"attribute vec3 rm_Tangent;\
attribute vec3 rm_Binormal;"

"varying vec3 normal;\n\
varying vec3 vertex_to_light1;\n\
varying vec3 vertex_to_light2;\n\
varying vec2 texcoord;\n\
varying vec4 color;\n\
uniform vec3 cameraPosition;\n\
varying vec3 viewDirection;\n"

"void main()\n\
{\n\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n\
  color = gl_Color;\n\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\n\
  viewDirection = normalize(vec3(vertex_in_modelview_space) - cameraPosition);\n\
  normal = gl_NormalMatrix * gl_Normal;\n\
  normal = faceforward(normal, viewDirection, normal);\n\
  vertex_to_light1 = normalize((gl_LightSource[0].position-vertex_in_modelview_space).xyz);\n\
  vertex_to_light2 = normalize((gl_LightSource[1].position-vertex_in_modelview_space).xyz);\n\
  texcoord = vec2(gl_MultiTexCoord0);\n\
  viewDirection = normalize(vec3(vertex_in_modelview_space) - cameraPosition);\n\
}";

// Note: added phong shading
const char* qattenuatedSpecularMaskFragmentProgram2Lights = \
"varying vec3 normal;\
varying vec3 vertex_to_light1;\
varying vec3 vertex_to_light2;\
varying vec2 texcoord;\
varying vec4 color;\
varying vec3 viewDirection;"

"uniform sampler2D diffuseTexture;\
uniform sampler2D normalTexture;"

"void main()\
{\
  vec4 diffuseColor = texture2D(diffuseTexture, texcoord) * color;\
  if(diffuseColor.a < 0.3)\
    discard;\
	vec3 normalized_normal = normalize(normal);\
	vec3 normalized_vertex_to_light1 = normalize(vertex_to_light1);\
  vec3 normalized_vertex_to_light2 = normalize(vertex_to_light2);\
  vec3 normalized_viewDirection = normalize(viewDirection);\
  float light1_distance = length(vertex_to_light1);\
  float light2_distance = length(vertex_to_light2);"
  
  // compute diffuse component
  "vec4 light1_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light1), 0.0, 1.0) * gl_LightSource[0].diffuse;\
  vec4 light2_diffuse = clamp(dot(normalized_normal, normalized_vertex_to_light2), 0.0, 1.0) * gl_LightSource[1].diffuse;\
  light1_diffuse /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\
  light2_diffuse /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\
  diffuseColor.rgb = diffuseColor.rgb * (light1_diffuse.rgb + light2_diffuse.rgb);"\
  
  // compute specular component
  "vec3 light1_reflect_direction = -normalize(2.0*dot(normalized_vertex_to_light1, normalized_normal)*normalized_normal - normalized_vertex_to_light1);\
  vec3 light2_reflect_direction = -normalize(2.0*dot(normalized_vertex_to_light2, normalized_normal)*normalized_normal - normalized_vertex_to_light2);\
  vec4 light1_specular = vec4(0.9, 1.0, 1.0, 1.0) * pow(max(dot(light1_reflect_direction, normalized_viewDirection), 0.0), 20.0);\
  vec4 light2_specular = vec4(1.3, 1.1, 0.9, 1.0) * pow(max(dot(light2_reflect_direction, normalized_viewDirection), 0.0), 20.0);\
  light1_specular /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\
  light2_specular /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\
  vec4 specularColor = light1_specular + light2_specular;"
  
  // compute fragment color
  "gl_FragColor = 1.0*diffuseColor + 0.2*specularColor;\
}";

template<int lights = 2, Attenuation attenuation = QUADRATIC>
class AttenuatedSpecularMaskShader : public Shader
{
protected:
  int diffuseTexture_sampler_uniform_location;
  int normalTexture_sampler_uniform_location;
  int cameraPosition_uniform_location;
  
  /*todo:
  template<class Type> class Uniform
  {
  protected:
    int uniformLocation;
  public:
    //Uniform(int uniformLocation) : uniformLocation(uniformLocation){}
    void setUniformLocation(int uniformLocation) { Uniform::uniformLocation = uniformLocation; }
  };*/

public:
  AttenuatedSpecularMaskShader() : Shader(qattenuatedSpecularMaskVertexProgram2Lights, qattenuatedSpecularMaskFragmentProgram2Lights)
  {}

  void setTexture(int textureID)
  {
    //glUniform1iARB(diffuseTexture_sampler_uniform_location, textureID);
    glUniform1i(diffuseTexture_sampler_uniform_location, textureID);
  }
  void setCameraPosition(float* cameraPosition)
  {
    //glUniform3fvARB(cameraPosition_uniform_location, 1, cameraPosition);
    glUniform3fv(cameraPosition_uniform_location, 1, cameraPosition);
    //glUniform3fARB(cameraPosition_uniform_location, cameraPosition[0], cameraPosition[1], cameraPosition[2]);
  }

  void build()
  {
    Shader::build();    
    //diffuseTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "diffuseTexture");
    diffuseTexture_sampler_uniform_location = glGetUniformLocation(programObject, "diffuseTexture");
    //normalTexture_sampler_uniform_location = glGetUniformLocationARB(programObject, "normalTexture");
    normalTexture_sampler_uniform_location = glGetUniformLocation(programObject, "normalTexture");
    //cameraPosition_uniform_location = glGetUniformLocationARB(programObject, "cameraPosition");
    cameraPosition_uniform_location = glGetUniformLocation(programObject, "cameraPosition");
    activate();
    //glUniform1iARB(normalTexture_sampler_uniform_location, 1); // texture unit #2
    glUniform1i(normalTexture_sampler_uniform_location, 1); // texture unit #2
  }
};

#endif
