#ifndef __ATTENUATEDNORMALSPECULARMASK_H__
#define __ATTENUATEDNORMALSPECULARMASK_H__

//NOTE: This shader is for double-sided surfaces
//      The normal is calculated based on which side of the polygon the viewer is

const char* qattenuatedNormalSpecularMaskVertexProgram2Lights = \
"attribute vec3 tangent;\
attribute vec3 binormal;"

"varying vec3 normal;\
varying vec2 texcoord;\
varying vec4 color;\
varying vec3 tanLight1;\
varying vec3 tanLight2;\
varying vec3 tanView;\
uniform vec3 cameraPosition;"

"void main()\
{\
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
  color = gl_Color;\
  vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;\
  vec3 viewDirection = normalize(vec3(vertex_in_modelview_space) - cameraPosition);\
  normal = /*gl_NormalMatrix */ gl_Normal;\
  normal = faceforward(normal, viewDirection, normal);\
  /*?tangent = faceforward(tangent, viewDirection, normal);*/\
  /*?binormal = faceforward(binormal, viewDirection, normal);*/"
  //"normal = gl_Normal;"
  /*"vec3 vertex_to_light1 = normalize((gl_LightSource[0].position-vertex_in_modelview_space).xyz);\
  vec3 vertex_to_light2 = normalize((gl_LightSource[1].position-vertex_in_modelview_space).xyz);"*/
  "vec3 vertex_to_light1 = vec3(gl_LightSource[0].position-vertex_in_modelview_space);\
  vec3 vertex_to_light2 = vec3(gl_LightSource[1].position-vertex_in_modelview_space);"
  
  //transform (xform) lights & camera into tangentspace
  "tanLight1.x = dot(tangent, vertex_to_light1);\
  tanLight1.y = dot(binormal, vertex_to_light1);\
  tanLight1.z = dot(normal, vertex_to_light1);" // using gl_Normal, NOT normal ???? is this correct? (Shouldn't tangent space vectors be transformed into world space??)

  "tanLight2.x = dot(tangent, vertex_to_light2);\
  tanLight2.y = dot(binormal, vertex_to_light2);\
  tanLight2.z = dot(normal, vertex_to_light2);"

  "tanView.x = dot(tangent, viewDirection);\
  tanView.y = dot(binormal, viewDirection);\
  tanView.z = dot(normal, viewDirection);"

  "texcoord = vec2(gl_MultiTexCoord0);\
}";

// Note: added phong shading
const char* qattenuatedNormalSpecularMaskFragmentProgram2Lights = \
"varying vec3 normal;\
varying vec3 tanLight1;\
varying vec3 tanLight2;\
varying vec3 tanView;\
varying vec2 texcoord;\
varying vec4 color;"

"uniform sampler2D diffuseTexture;\
uniform sampler2D normalTexture;"

"void main()\
{\
  vec4 diffuseColor = texture2D(diffuseTexture, texcoord) * color;\
  if(diffuseColor.a < 0.2)\
    discard;"
	
  //vec3 normalized_normal = normalize(normal);
  "vec3 tanNormal = normalize(vec3(texture2D(normalTexture, texcoord)*2.0) - vec3(1.0));"
  
  "vec3 normalized_vertex_to_light1 = normalize(tanLight1);\
  vec3 normalized_vertex_to_light2 = normalize(tanLight2);\
  vec3 normalized_viewDirection = normalize(tanView);\
  float light1_distance = length(tanLight1);\
  float light2_distance = length(tanLight2);"
  
  // compute diffuse component
  "vec4 light1_diffuse = clamp(dot(normalized_vertex_to_light1, tanNormal), 0.0, 1.0) * gl_LightSource[0].diffuse;\
  vec4 light2_diffuse = clamp(dot(normalized_vertex_to_light2, tanNormal), 0.0, 1.0) * gl_LightSource[1].diffuse;"
  "light1_diffuse /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\
  light2_diffuse /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;"
  "diffuseColor.rgb = diffuseColor.rgb * (light1_diffuse.rgb + light2_diffuse.rgb);"
  
  // compute specular component
  "vec3 light1_reflect_direction = -normalize(2.0*dot(normalized_vertex_to_light1, tanNormal)*tanNormal - normalized_vertex_to_light1);\
  vec3 light2_reflect_direction = -normalize(2.0*dot(normalized_vertex_to_light2, tanNormal)*tanNormal - normalized_vertex_to_light2);\
  vec4 light1_specular = vec4(0.9, 1.0, 1.0, 1.0) * pow(max(dot(light1_reflect_direction, normalized_viewDirection), 0.0), 2.0);\
  vec4 light2_specular = vec4(1.3, 1.1, 0.9, 1.0) * pow(max(dot(light2_reflect_direction, normalized_viewDirection), 0.0), 2.0);\
  light1_specular /= gl_LightSource[0].quadraticAttenuation * light1_distance * light1_distance + 1.0;\
  light2_specular /= gl_LightSource[1].quadraticAttenuation * light2_distance * light2_distance + 1.0;\
  vec4 specularColor = light1_specular + light2_specular;"
  
  // compute fragment color
  "gl_FragColor = 1.0*diffuseColor + 0.4*specularColor;"
  //"gl_FragColor = texture2D(normalTexture, texcoord);"
  //"gl_FragColor = vec4(clamp(dot(tanNormal, normalized_vertex_to_light1), 0.0, 1.0));"
  //"gl_FragColor = light2_diffuse;"
  //"gl_FragColor = vec4(tanLight1, 1.0);"
"}";

template<int lights = 2, Attenuation attenuation = QUADRATIC>
class AttenuatedNormalSpecularMaskShader : public Shader
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
  int tangentLocation;
  int binormalLocation;

  AttenuatedNormalSpecularMaskShader() : Shader(qattenuatedNormalSpecularMaskVertexProgram2Lights, qattenuatedNormalSpecularMaskFragmentProgram2Lights)
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

    tangentLocation = glGetAttribLocation(programObject, "tangent");
    binormalLocation = glGetAttribLocation(programObject, "binormal");

    activate();
    //glUniform1iARB(normalTexture_sampler_uniform_location, 1); // texture unit #2
    glUniform1i(normalTexture_sampler_uniform_location, 1); // texture unit #2
  }
};

#endif
