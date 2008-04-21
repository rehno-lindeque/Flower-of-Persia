#ifndef __CLOTH_H__
#define __CLOTH_H__

#include "quadmap.h"

//class Cloth : public Model
class Cloth : public QuadMap
{
public:
  class Particle
  {
  public:
    Vector3d force;
    Vector3d position;
    Vector3d acceleration;
    Vector3d velocity;
    double mass;
    bool movable;
  };

  class Spring
  {
  public:
    bool diagonal;
    double restLength;
    static double tensionTensileConst;
    static double shearTensileConst;
    static double dampFactor;
    Particle* top,* bottom;

    void applyForce()
    {
      Vector3d L = top->position - bottom->position;
      Vector3d V = top->velocity - bottom->velocity;
      double S = L.getLength();
      double k;
      if(!diagonal)
        k = tensionTensileConst;
      else
        k = shearTensileConst;
      double r = restLength;
      double d = dampFactor;
      Vector3d springForce =  -(k*(S - r) + d*(Vector3d::dot(V, L))/S)*L/S;

      top->force += springForce;
      bottom->force -= springForce;
    }
  };

  Particle* particles;
  Spring* springs;

  /*float* horisontalStressMap; // this is a map of the lengths between horisontally connected vertices (in reality it's just the calculated lengths between vertices)
  float* verticalStressMap;   // this is a map of the lengths between vertically connected vertices*/
  float width, height;
  int nVerticalVertices, nHorisontalVertices, nVertices, nHorisontalEdges, nVerticalEdges, nDiagonalEdges;
  float clothMass, particleMass;
  Vector3 position;

  Cloth(Vector3 pos, int xSubdivisions, int ySubdivisions, float width, float height, float clothMass) : width(width), height(height), clothMass(clothMass), position(pos)
  {
    particleMass = clothMass / (xSubdivisions*ySubdivisions); //actually, this is only really true for non-border and non-corner vertices, but it's close enough and the more subdivisions, the closer it gets...
    nHorisontalVertices = (xSubdivisions+1);
    nVerticalVertices = (ySubdivisions+1);
    nVertices = nVerticalVertices * nHorisontalVertices;
    nHorisontalEdges = xSubdivisions   * (ySubdivisions+1);
    nVerticalEdges = (xSubdivisions+1) * ySubdivisions;
    nDiagonalEdges = (xSubdivisions * ySubdivisions)*2;

    float xSegmentWidth = width / xSubdivisions;
    float ySegmentWidth = height / ySubdivisions;

    // create particles
    //Particle::mass = particleMass;
    particles = new Particle[nVertices];
    // create top row
    for(int cx = 0; cx < nHorisontalVertices; cx++)
    {
      particles[cx].acceleration = Vector3(0.0f, 0.0f, 0.0f);
      particles[cx].force = Vector3(0.0f, 0.0f, 0.0f);
      particles[cx].velocity = Vector3(0.0f, 0.0f, 0.0f);
      particles[cx].position = pos+Vector3(Random::getFloat(-xSegmentWidth/2.0f, xSegmentWidth/2.0f), 0.0f, Random::getFloat(-xSegmentWidth/2.0f, xSegmentWidth/2.0f)+cx*xSegmentWidth-width/2.0f);
      particles[cx].movable = (cx&3) != 0;
      //particles[cx].movable = true;
      particles[cx].mass = particleMass/2.0f;;
      if(cx == 0 || cx == xSubdivisions)
        particles[cx].mass /= 2.0f;
    }

    // create the rest
    for(int cy = 1; cy < nVerticalVertices; cy++)
      for(int cx = 0; cx < nHorisontalVertices; cx++)
      {
        particles[cy*(xSubdivisions+1)+cx].acceleration = Vector3(0.0f, 0.0f, 0.0f);
        particles[cy*(xSubdivisions+1)+cx].force = Vector3(0.0f, 0.0f, 0.0f);
        particles[cy*(xSubdivisions+1)+cx].velocity = Vector3(0.0f, 0.0f, 0.0f);
        particles[cy*(xSubdivisions+1)+cx].position = pos+Vector3(0.0f, -cy*ySegmentWidth/*-height/2.0f*/, cx*xSegmentWidth-width/2.0f);
        particles[cy*(xSubdivisions+1)+cx].mass = particleMass;
        particles[cy*(xSubdivisions+1)+cx].movable = true;
        if(cx == 0 || cx == xSubdivisions)
          particles[cy*(xSubdivisions+1)+cx].mass /= 2.0f;
        if(cy == 0 || cy == ySubdivisions)
          particles[cy*(xSubdivisions+1)+cx].mass /= 2.0f;
      }

    // create springs
    float scaleSprings = 1.0f;
    //Spring::tensionTensileConst = 280.0f;
    Spring::tensionTensileConst = 350.0f;
    //Spring::shearTensileConst = 220.0f;
    Spring::shearTensileConst = 280.0f;
    Spring::dampFactor = 2.0f;
    springs = new Spring[nHorisontalEdges + nVerticalEdges + nDiagonalEdges];
    // set horisontal springs
    for(int c = 0; c < nHorisontalEdges; c++)
    {
      springs[c].diagonal = false;
      springs[c].restLength = xSegmentWidth*scaleSprings;
      springs[c].top = &particles[c + c/xSubdivisions];
      springs[c].bottom = &particles[c + c/xSubdivisions + 1];
    }
    // set vertical springs
    for(int c = 0; c < nVerticalEdges; c++)
    {
      springs[nHorisontalEdges+c].diagonal = false;
      springs[nHorisontalEdges+c].restLength = ySegmentWidth*scaleSprings;
      springs[nHorisontalEdges+c].top    = &particles[c];
      springs[nHorisontalEdges+c].bottom = &particles[c+nHorisontalVertices];
    }
    // set diagonal springs
    float diagonalSegmentWidth = sqrt(xSegmentWidth*xSegmentWidth + ySegmentWidth*ySegmentWidth);
    for(int c = 0; c < nDiagonalEdges/2; c++)
    {
      int offset = nHorisontalEdges + nVerticalEdges;
      springs[offset+c].diagonal = true;
      springs[offset+c].restLength = diagonalSegmentWidth*scaleSprings;
      springs[offset+c].top    = &particles[c + c/xSubdivisions];
      springs[offset+c].bottom = &particles[c + c/xSubdivisions + nHorisontalVertices+1];
    }
    for(int c = 0; c < nDiagonalEdges/2; c++)
    {
      int offset = nHorisontalEdges + nVerticalEdges + nDiagonalEdges/2;
      springs[offset+c].diagonal = true;
      springs[offset+c].restLength = diagonalSegmentWidth*scaleSprings;
      springs[offset+c].top    = &particles[c + c/xSubdivisions + 1];
      springs[offset+c].bottom = &particles[c + c/xSubdivisions + nHorisontalVertices];
    }
  }

  virtual void build()
  {
    // nothing here since no display list or vertex array is needed

  }

  Vector3 calcFaceNormal(int x, int y)
  {
    Vector3 p1 = particles[x+1+y*nHorisontalVertices].position - particles[x+y*nHorisontalVertices].position;
    Vector3 p2 = particles[x+(y+1)*nHorisontalVertices].position - particles[x+y*nHorisontalVertices].position;
    //return Vector3::cross(p1, p2).normalize();
    return Vector3::cross(p1, p2);
  }

  Vector3 calcNormal(int x, int y)
  {
    if(x == 0)
    {
      if(y == 0)
        return calcFaceNormal(x, y).normalize();
      else if(y == nVerticalVertices-1)
        return calcFaceNormal(x, y-1).normalize();
      else
        return (calcFaceNormal(x,y) + calcFaceNormal(x,y-1)).normalize();
    }
    else if(x == nHorisontalVertices-1)
    {
      if(y == 0)
        return calcFaceNormal(x-1, y).normalize();
      else if(y == nVerticalVertices-1)
        return calcFaceNormal(x-1, y-1).normalize();
      else
        return (calcFaceNormal(x-1, y) + calcFaceNormal(x-1, y-1)).normalize();
    }
    else
    {
      if(y == 0)
        return (calcFaceNormal(x, y) + calcFaceNormal(x-1, y)).normalize();
      else if(y == nVerticalVertices-1)
        return (calcFaceNormal(x, y-1) + calcFaceNormal(x-1, y-1)).normalize();
      else
        return (calcFaceNormal(x, y-1) + calcFaceNormal(x-1, y-1) + calcFaceNormal(x, y) + calcFaceNormal(x-1, y)).normalize();
    }
  }

  virtual void render()
  {
    // set uv generator
    UVGenerator* oldUVMap = uvMap0;


    int xstep = 1, xstart = 0, xend = nHorisontalVertices-1;
    if(camera.getLookDirection()(2) > 0.0f) // z looking forward?
    {
      xstep = -1;
      xstart = nHorisontalVertices-2;
      xend = -1;
      uvMap0 = new QuadGridUVGenerator(nHorisontalVertices-1, nVerticalVertices-1, -1);
    }
    else
      uvMap0 = new QuadGridUVGenerator(nHorisontalVertices-1, nVerticalVertices-1);

    for(int cy = 0; cy < nVerticalVertices-1; cy++)
      for(int cx = xstart; cx != xend; cx += xstep)
      {
        /*drawQuad((Vector3)particles[cy*nHorisontalVertices + cx].position,
                (Vector3)particles[cy*nHorisontalVertices + cx+1].position,
                (Vector3)particles[cy*nHorisontalVertices+nHorisontalVertices + cx+1].position,
                (Vector3)particles[cy*nHorisontalVertices+nHorisontalVertices + cx].position);
        drawReverseQuad((Vector3)particles[cy*nHorisontalVertices + cx].position,
                        (Vector3)particles[cy*nHorisontalVertices + cx+1].position,
                        (Vector3)particles[cy*nHorisontalVertices+nHorisontalVertices + cx+1].position,
                        (Vector3)particles[cy*nHorisontalVertices+nHorisontalVertices + cx].position);*/

        Vector3 n1 = calcNormal(cx, cy);
        Vector3 n2 = calcNormal(cx+1, cy);
        Vector3 n3 = calcNormal(cx+1, cy+1);
        Vector3 n4 = calcNormal(cx, cy+1);

        /*Vector2 uv1 = Vector2(cx/(float)nHorisontalVertices,     cy/(float)nVerticalVertices);
        Vector2 uv2 = Vector2((cx+1)/(float)nHorisontalVertices, cy/(float)nVerticalVertices);
        Vector2 uv3 = Vector2((cx+1)/(float)nHorisontalVertices, (cy+1)/(float)nVerticalVertices);
        Vector2 uv4 = Vector2(cx/(float)nHorisontalVertices,     (cy+1)/(float)nVerticalVertices);

        drawQuadNormUV( n1, uv1, (Vector3)particles[cy*nHorisontalVertices + cx].position,
                        n2, uv2, (Vector3)particles[cy*nHorisontalVertices + cx+1].position,
                        n3, uv3, (Vector3)particles[(cy+1)*nHorisontalVertices + cx+1].position,
                        n4, uv4, (Vector3)particles[(cy+1)*nHorisontalVertices + cx].position);*/
        drawQuadNorm( n1, (Vector3)particles[cy*nHorisontalVertices + cx].position,
                      n2, (Vector3)particles[cy*nHorisontalVertices + cx+1].position,
                      n3, (Vector3)particles[(cy+1)*nHorisontalVertices + cx+1].position,
                      n4, (Vector3)particles[(cy+1)*nHorisontalVertices + cx].position);
        /*drawReverseQuadNormUV(n1, uv1, (Vector3)particles[cy*nHorisontalVertices + cx].position,
                              n2, uv2, (Vector3)particles[cy*nHorisontalVertices + cx+1].position,
                              n3, uv3, (Vector3)particles[(cy+1)*nHorisontalVertices + cx+1].position,
                              n4, uv4, (Vector3)particles[(cy+1)*nHorisontalVertices + cx].position);*/
      }
    //reset uv generator
    delete uvMap0;
    uvMap0 = oldUVMap;
  }

  virtual void renderNormals()
  {
    for(int cy = 0; cy < nVerticalVertices-1; cy++)
      for(int cx = 0; cx < nHorisontalVertices-1; cx++)
      {
        Vector3 n1 = calcNormal(cx, cy);
        Vector3 n2 = calcNormal(cx+1, cy);
        Vector3 n3 = calcNormal(cx+1, cy+1);
        Vector3 n4 = calcNormal(cx, cy+1);

        drawQuadNorm_drawNormals(n1, (Vector3)particles[cy*nHorisontalVertices + cx].position,
                                 n2, (Vector3)particles[cy*nHorisontalVertices + cx+1].position,
                                 n3, (Vector3)particles[(cy+1)*nHorisontalVertices + cx+1].position,
                                 n4, (Vector3)particles[(cy+1)*nHorisontalVertices + cx].position);
      }
  }

  void update(double time)
  {
    time /= 10.0;

    // compute forces on particles
    {
      for(int c = 0; c < nVertices; c++)
      {
        // compute wind force
        particles[c].force = wind.apply(particles[c].position);
        //particles[c].force = Vector3(0.0f, 0.0f, 0.0f);

        // compute gravity force
        particles[c].force += gravity.apply(particles[c].position, particles[c].mass);

        // compute drag force
        if(particles[c].velocity.getLength() != 0.0f)
        {
          const float dragCoefficient = 0.01f;
          particles[c].force += -particles[c].velocity.getNormalized()*particles[c].velocity.getLength()*particles[c].velocity.getLength()*dragCoefficient;
        }
      }

      // compute spring force
      for(int c = 0; c < nHorisontalEdges + nVerticalEdges + nDiagonalEdges; c++)
        springs[c].applyForce();
    }

    for(int c = 0; c < nVertices; c++)
      if(particles[c].movable)
      {
        // calculate acceleration
        particles[c].acceleration = particles[c].force / particles[c].mass;

        // move particles
          // calculate velocity
          particles[c].velocity += particles[c].acceleration*time;

          // calculate position
          particles[c].position += particles[c].velocity*time;
      }
  }

  ~Cloth()
  {
    delete[] particles;
    delete[] springs;
  }

};

#endif
