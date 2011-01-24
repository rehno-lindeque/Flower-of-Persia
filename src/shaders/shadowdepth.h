#ifndef __SHADOWDEPTH_H__
#define __SHADOWDEPTH_H__

/*

  The shadow-depth shader renders geometry to a floating point cube map (shadowmap) centered arround a light. Only
  the distance of the geometry from the light is rendered so it can be used for shadow calculation later on.

  IMPORTANT NOTES:
  1. The geometry must be transformed (via glTranslate) so that the light is at position (0,0,0). This can be done in the
     shader instead, but that would be wastefull of performance since modifying the world transformation is free.

*/

const char* shadowdepthVertexProgram = 
"varying vec4 modelViewVertex;\n"
"varying vec4 projectedVertex;\n"
"void main()\n"
"{\n"
   // project the vertex 
"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"  projectedVertex = gl_Position;\n"
  
   // calculate the vertex in model view space
   // TODO: can't you just use the z component of glPosition???
"  modelViewVertex = gl_ModelViewMatrix * gl_Vertex;\n"
"}";

const char* shadowdepthFragmentProgram =
"varying vec4 modelViewVertex;\n"
"varying vec4 projectedVertex;\n"
"void main()\n"
"{\n"
   //gl_FragColor = length(modelViewVertex);
"  gl_FragColor = vec4(projectedVertex.z);\n"
"}";

class ShadowdepthShader : public Shader
{
public:
  ShadowdepthShader() : Shader(shadowdepthVertexProgram, shadowdepthFragmentProgram)
  {}

  void build()
  {
    Shader::build();
  }
};

#endif
