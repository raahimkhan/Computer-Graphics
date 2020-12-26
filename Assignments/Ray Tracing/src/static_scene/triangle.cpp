#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}
bool Triangle::intersect(const Ray& r) const {

  // Task 2 Part 6: implement ray-triangle intersection

  Vector3D v0(mesh -> positions[v1]) ;
  Vector3D v1(mesh -> positions[v2]) ;
  Vector3D v2(mesh -> positions[v3]) ;

  Vector3D v0v1 = v1 - v0 ;
  Vector3D v0v2 = v2 - v0 ;
  
  Vector3D pvec = cross(r.d, v0v2) ;
  float det = dot(v0v1, pvec) ;


  float invDet = 1 / det ;   
  Vector3D tvec = r.o - v0 ;
  float u = dot(tvec, pvec) * invDet ;
  if (u < 0 || u > 1) {
    return false ;
  }

  Vector3D qvec = cross(tvec, v0v1) ;
  float v = dot(r.d, qvec) * invDet ;
  if (v < 0 || u + v > 1) {
    return false ;
  }

  float t = dot(v0v2, qvec) * invDet;
  if (t <= r.max_t && t >= r.min_t){
    r.max_t = t ;
    return true ;
  }

  return false ;
} 


bool Triangle::intersect(const Ray& r, Intersection *isect) const {

  // Task 2 Part 6:
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly

  Vector3D vv0(mesh -> positions[v1]) ;
  Vector3D vv1(mesh -> positions[v2]) ;
  Vector3D vv2(mesh -> positions[v3]) ;

  Vector3D n1(mesh->normals[v1]) ;
  Vector3D n2(mesh->normals[v2]) ;
  Vector3D n3(mesh->normals[v3]) ;

  Vector3D v0v1 = vv1 - vv0 ;
  Vector3D v0v2 = vv2 - vv0 ;
  Vector3D pvec = cross(r.d, v0v2) ;
  float det = dot(v0v1, pvec);
  float invDet = 1 / det ;   
  Vector3D tvec = r.o - vv0 ;

  float u = dot(tvec, pvec) * invDet ;
  if (u < 0 || u > 1) {
    return false ;
  }

  Vector3D qvec = cross(tvec, v0v1) ;
  float v = dot(r.d, qvec) * invDet ;
  if (v < 0 || u + v > 1) {
    return false ;
  } 

  float t = dot(v0v2, qvec) * invDet ;

  // This code taken from sphere.cpp Part 5

  if (t <= r.max_t && t >= r.min_t)
  {
    isect -> t = r.max_t = t ; 
    isect -> n = (1 - u - v) * n1 + u * n2 + v * n3 ;
    isect -> primitive = this ;
    isect -> bsdf = get_bsdf() ;
    return true ;
  }

  return false ;
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CGL
