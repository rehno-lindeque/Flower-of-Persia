#ifndef __STEPS_H__
#define __STEPS_H__

extern class Steps : public Model
{
protected:
  GLuint displayList;

  void drawXSteps(Vector3 start, int nSteps, float stepsHeight, float basisHeight, float length, float width)
  {
    float h = stepsHeight/nSteps; // single step height
    float l = length/(nSteps);   // step x-length (i.e. horisontal)

    float xPos = start(0); // single step height
    float yPos = basisHeight + start(1);

    for(int c = 0; c < nSteps; c++)
    {
      // draw vertical quad    
      drawQuad(Vector3(xPos, yPos, start(2)-width/2.0f), // bl
               Vector3(xPos, yPos+h, start(2)-width/2.0f), //tl
               Vector3(xPos, yPos+h, start(2)+width/2.0f), //tr
               Vector3(xPos, yPos, start(2)+width/2.0f)); //br

      // draw horisontal quad
      drawQuad(Vector3(xPos,   yPos+h, start(2)-width/2.0f), // bl
               Vector3(xPos+l, yPos+h, start(2)-width/2.0f), //tl
               Vector3(xPos+l, yPos+h, start(2)+width/2.0f), //tr
               Vector3(xPos,   yPos+h, start(2)+width/2.0f)); //br

      // draw side quad
      drawQuad(Vector3(xPos,   start(3), start(2)+width/2.0f), // bl
               Vector3(xPos,   yPos+h,   start(2)+width/2.0f), //tl
               Vector3(xPos+l, yPos+h,   start(2)+width/2.0f), //tr
               Vector3(xPos+l, start(3), start(2)+width/2.0f)); //br

      yPos += h;
      xPos += l;
    }
    
    // draw vertical quad    
    /*drawQuad(Vector3(xPos, yPos, start(2)-width/2.0f), // bl
              Vector3(xPos, yPos+h, start(2)-width/2.0f), //tl
              Vector3(xPos, yPos+h, start(2)+width/2.0f), //tr
              Vector3(xPos, yPos, start(2)+width/2.0f)); //br*/
  }

  void drawReverseXSteps(Vector3 start, int nSteps, float stepsHeight, float basisHeight, float length, float width)
  {
    float h = stepsHeight/nSteps; // single step height
    float l = length/(nSteps);   // step x-length (i.e. horisontal)

    float xPos = start(0); // single step height
    float yPos = basisHeight + start(1);

    for(int c = 0; c < nSteps; c++)
    {
      // draw vertical quad    
      drawReverseQuad(Vector3(xPos, yPos, start(2)-width/2.0f), // bl
               Vector3(xPos, yPos+h, start(2)-width/2.0f), //tl
               Vector3(xPos, yPos+h, start(2)+width/2.0f), //tr
               Vector3(xPos, yPos, start(2)+width/2.0f)); //br

      // draw horisontal quad
      drawReverseQuad(Vector3(xPos,   yPos+h, start(2)-width/2.0f), // bl
               Vector3(xPos+l, yPos+h, start(2)-width/2.0f), //tl
               Vector3(xPos+l, yPos+h, start(2)+width/2.0f), //tr
               Vector3(xPos,   yPos+h, start(2)+width/2.0f)); //br
      
      // draw side quad
      drawReverseQuad(Vector3(xPos,   start(3), start(2)+width/2.0f), // bl
               Vector3(xPos,   yPos+h,   start(2)+width/2.0f), //tl
               Vector3(xPos+l, yPos+h,   start(2)+width/2.0f), //tr
               Vector3(xPos+l, start(3), start(2)+width/2.0f)); //br

      yPos += h;
      xPos += l;
    }
    //draw vertical quad
    /*drawReverseQuad(Vector3(xPos, yPos, start(2)-width/2.0f), // bl
                    Vector3(xPos, yPos+h, start(2)-width/2.0f), //tl
                    Vector3(xPos, yPos+h, start(2)+width/2.0f), //tr
                    Vector3(xPos, yPos, start(2)+width/2.0f)); //br*/
  }

  void drawCurveSteps(Vector3 centerOfRotation, int nSteps, float stepsHeight, float basisHeight, float startAngle, float endAngle, float startRadius, float endRadius)
  {
    float a = (endAngle - startAngle) / (nSteps-1); // the angle step

    float h = stepsHeight/nSteps; // single step height    
    float yPos = basisHeight + centerOfRotation(1);
    float anglePos = startAngle;

    for(int c = 0; c < nSteps-1; c++)
    {
      Vector3 v1 = centerOfRotation + Vector3(-cos(anglePos)*startRadius, 0.0f, sin(anglePos)*startRadius);
      Vector3 v2 = centerOfRotation + Vector3(-cos(anglePos+a)*startRadius, 0.0f, sin(anglePos+a)*startRadius);

      Vector3 v3 = centerOfRotation + Vector3(-cos(anglePos)*endRadius, 0.0f, sin(anglePos)*endRadius);
      Vector3 v4 = centerOfRotation + Vector3(-cos(anglePos+a)*endRadius, 0.0f, sin(anglePos+a)*endRadius);

      Vector3 n1 = -Vector3(-cos(anglePos), 0.0f, sin(anglePos));
      Vector3 n2 = -Vector3(-cos(anglePos+a), 0.0f, sin(anglePos+a));

      // draw side quad
      drawQuadNorm( n1, v1, // bl
                    n1, v1+Vector3(0.0f, yPos+h, 0.0f), //tl
                    n2, v2+Vector3(0.0f, yPos+h, 0.0f), //tr
                    n2, v2); //br

      // draw vertical quad    
      drawQuad(v3+Vector3(0.0f, yPos, 0.0f), // bl
               v3+Vector3(0.0f, yPos+h, 0.0f), //tl
               v1+Vector3(0.0f, yPos+h, 0.0f), //tr
               v1+Vector3(0.0f, yPos,   0.0f)); //br

      // draw horisontal quad
      drawQuad(v3+Vector3(0.0f, yPos+h, 0.0f), // bl
               v4+Vector3(0.0f, yPos+h, 0.0f), // tl
               v2+Vector3(0.0f, yPos+h, 0.0f), // tr
               v1+Vector3(0.0f, yPos+h, 0.0f)); // br

      yPos += h;
      anglePos += a;
    }

    // close steps
    Vector3 v1 = centerOfRotation + Vector3(-cos(anglePos)*startRadius, 0.0f, sin(anglePos)*startRadius);
    Vector3 v3 = centerOfRotation + Vector3(-cos(anglePos)*endRadius, 0.0f, sin(anglePos)*endRadius);
    drawReverseQuad(v3+Vector3(0.0f, 0.0f, 0.0f), // bl
                    v3+Vector3(0.0f, yPos, 0.0f), //tl
                    v1+Vector3(0.0f, yPos, 0.0f), //tr
                    v1+Vector3(0.0f, 0.0f,   0.0f)); //br
  }

  void drawReverseCurveSteps(Vector3 centerOfRotation, int nSteps, float stepsHeight, float basisHeight, float startAngle, float endAngle, float startRadius, float endRadius)
  {
    float a = (endAngle - startAngle) / (nSteps-1); // the angle step

    float h = stepsHeight/nSteps; // single step height    
    float yPos = basisHeight + centerOfRotation(1);
    float anglePos = startAngle;

    for(int c = 0; c < nSteps-1; c++)
    {
      Vector3 v1 = centerOfRotation + Vector3(-cos(anglePos)*startRadius, 0.0f, sin(anglePos)*startRadius);
      Vector3 v2 = centerOfRotation + Vector3(-cos(anglePos+a)*startRadius, 0.0f, sin(anglePos+a)*startRadius);

      Vector3 v3 = centerOfRotation + Vector3(-cos(anglePos)*endRadius, 0.0f, sin(anglePos)*endRadius);
      Vector3 v4 = centerOfRotation + Vector3(-cos(anglePos+a)*endRadius, 0.0f, sin(anglePos+a)*endRadius);

      Vector3 n1 = Vector3(-cos(anglePos), 0.0f, sin(anglePos));
      Vector3 n2 = Vector3(-cos(anglePos+a), 0.0f, sin(anglePos+a));

      // draw side quad
      drawReverseQuadNorm(n1, v1, // bl
               n1, v1+Vector3(0.0f, yPos+h, 0.0f), //tl
               n2, v2+Vector3(0.0f, yPos+h, 0.0f), //tr
               n2, v2); //br

      // draw vertical quad    
      drawReverseQuad(v3+Vector3(0.0f, yPos, 0.0f), // bl
               v3+Vector3(0.0f, yPos+h, 0.0f), //tl
               v1+Vector3(0.0f, yPos+h, 0.0f), //tr
               v1+Vector3(0.0f, yPos,   0.0f)); //br

      // draw horisontal quad
      drawReverseQuad(v3+Vector3(0.0f, yPos+h, 0.0f), // bl
               v4+Vector3(0.0f, yPos+h, 0.0f), // tl
               v2+Vector3(0.0f, yPos+h, 0.0f), // tr
               v1+Vector3(0.0f, yPos+h, 0.0f)); // br

      yPos += h;
      anglePos += a;
    }

    // close steps
    Vector3 v1 = centerOfRotation + Vector3(-cos(anglePos)*startRadius, 0.0f, sin(anglePos)*startRadius);
    Vector3 v3 = centerOfRotation + Vector3(-cos(anglePos)*endRadius, 0.0f, sin(anglePos)*endRadius);
    drawQuad(v3+Vector3(0.0f, 0.0f, 0.0f), // bl
                    v3+Vector3(0.0f, yPos, 0.0f), //tl
                    v1+Vector3(0.0f, yPos, 0.0f), //tr
                    v1+Vector3(0.0f, 0.0f,   0.0f)); //br
  }

public:
  virtual void build()
  {
    float qlist[][3] = 
    {
      // back-left
      {-24.0f, 0.4f, 16.0f}, {-12.0f, 0.4f, 28.0f},      
      {-24.0f, 0.9f, 16.0f}, {-12.0f, 0.9f, 28.0f},
      {-24.0f, 0.9f, 20.0f}, {-16.0f, 0.9f, 28.0f},
      {-24.0f, 1.4f, 20.0f}, {-16.0f, 1.4f, 28.0f},
      {-24.0f, 1.4f, 24.0f}, {-20.0f, 1.4f, 28.0f},
      
      // back-right
      { 12.0f, 0.4f, 28.0f}, { 24.0f, 0.4f, 16.0f},
      { 12.0f, 0.9f, 28.0f}, { 24.0f, 0.9f, 16.0f},
      { 16.0f, 0.9f, 28.0f}, { 24.0f, 0.9f, 20.0f},
      { 16.0f, 1.4f, 28.0f}, { 24.0f, 1.4f, 20.0f},
      { 20.0f, 1.4f, 28.0f}, { 24.0f, 1.4f, 24.0f},

      //front
      /*{ 1.6f, 0.0f, -28.0f}, {-1.6f, 0.0f, -28.0f},
      { 1.6f, 0.8f, -28.0f}, {-1.6f, 0.8f, -28.0f},
      { 1.6f, 0.8f, -28.8f}, {-1.6f, 0.8f, -28.8f},
      
      { 2.0f, 0.8f, -28.8f}, {-2.0f, 0.8f, -28.8f},
      { 2.0f, 1.6f, -28.8f}, {-2.0f, 1.6f, -28.8f},
      { 2.0f, 1.6f, -29.6f}, {-2.0f, 1.6f, -29.6f} ,

      { 3.6f, 0.1f, -28.0f}, {-3.6f, 0.1f, -28.0f},
      { 3.6f, 0.9f, -28.0f}, {-3.6f, 0.9f, -28.0f},
      { 3.6f, 0.9f, -28.8f}, {-3.6f, 0.9f, -28.8f} */
      
    };
    
    float stepHeight = 0.5f;
    float stepDepth = 4.2f/3;

    displayList = glGenLists(1);
    glNewList(displayList, GL_COMPILE);    
      glBegin(GL_QUADS);
        drawQuadList((float*)qlist[0],  4);
        drawQuadList((float*)qlist[10], 4);
        /*drawQuadList((float*)qlist[20], 2); // front steps
        drawQuadList((float*)qlist[23], 2);
        drawQuadList((float*)qlist[26], 2);*/
        drawBlock(Vector3(0.0f, stepHeight/2.0f,   -27.0f - stepDepth), 6.4f, stepHeight, stepDepth*3);
        drawBlock(Vector3(0.0f, stepHeight*2/2.0f, -27.0f - stepDepth*1.5), 4.8f, stepHeight*2, stepDepth*2);
        drawBlock(Vector3(0.0f, stepHeight*3/2.0f, -27.0f - stepDepth*2), 3.2f, stepHeight*3, stepDepth);
        drawBlock(Vector3(0.0f, stepHeight*4/2.0f, -36.0f + 2.75f), 8.0f, stepHeight*4, 5.5f);
        drawXSteps(Vector3(-4.0f, 0.4f, -36.0f + 2.75f), 7, stepHeight*7, stepHeight*4-0.4f, -8.0f, 5.5f);
        drawReverseXSteps(Vector3( 4.0f, 0.4f, -36.0f + 2.75f), 8, stepHeight*7, stepHeight*4-0.4f, 8.0f, 5.5f);
        //drawCurveSteps(Vector3(-12.0f, 0.4f, -24.0f), 2, stepHeight*8, stepHeight*8-0.4f, 180.0f, 270.0f, 6.5f, 12.0f );
        drawCurveSteps(Vector3(-12.0f, 0.4f, -24.0f), 12, stepHeight*12, stepHeight*4-0.4f + stepHeight*6, (float)(PI*1.5f), (float)(2.0f*PI), 6.5f, 12.0f );
        //drawCurveSteps(Vector3( 12.0f, 0.4f, -24.0f), 10, -stepHeight*10, stepHeight*4-0.4f + stepHeight*6,-PI,    -0.5*PI, 6.5f, 12.0f );
        drawReverseCurveSteps(Vector3( 12.0f, 0.4f, -24.0f), 12, stepHeight*12, stepHeight*4-0.4f + stepHeight*6, (float)(1.5f*PI), (float)(PI*1.0f), 6.5f, 12.0f );

        //draw platforms

        //drawBlock(Vector3(-24.0f+2.75f, 9.2f+stepHeight*2, -8.0f), 5.5f, stepHeight*4, 32.0f);
        drawBlock(Vector3(-24.0f+2.75f, 10.2f+stepHeight*2, -16.0f), 5.5f, stepHeight*4, 16.0f);
        drawBlock(Vector3(+24.0f-2.75f, 10.2f+stepHeight*2, -16.0f), 5.5f, stepHeight*4, 16.0f);
        drawBlock(Vector3(+16.0f, 10.2f+stepHeight*2, 0.0f), 16.0f, stepHeight*4, 16.0f);
        drawBlock(Vector3(-16.0f, 10.2f+stepHeight*2, 0.0f), 16.0f, stepHeight*4, 16.0f);

      glEnd();
    glEndList();
  }

  virtual void render()
  {
    glCallList(displayList);
  }

  virtual void renderNormals()
  {
  }
} steps;

#endif
