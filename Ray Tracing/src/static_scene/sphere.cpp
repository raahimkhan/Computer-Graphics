#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

//PART 4
bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  double A = dot(r.d, r.d) ;
  double B = dot(2 * r.d, (r.o - object -> o)) ;
  double C = dot((r.o - object -> o), (r.o - object -> o)) - (object -> r * object -> r) ;

  double check = (B * B) - 4 * (A * C) ;

  if (check >= 0) {
    t1 = ((-B) + sqrt(check)) / (2 * A) ;
    t1 = ((-B) - sqrt(check)) / (2 * A) ;
    if (t2 < t1) {
      swap(t1, t2) ;
    }
    return true ;
  }

  return false ;
}

//PART 5
bool Sphere::intersect(const Ray& r, Intersection *i) const {

  double t1, t2 ;
  if(test(r, t1, t2) == false) {
    return false ;
  }

  double rad = object -> r ;
  Vector3D c = object -> o ;
  Vector3D p = r.o + (t1 * r.d) ;
  Vector3D n = (p - c) / rad ;

  if(t1 >= r.min_t && t1 <= r.max_t) {
    i -> primitive = this ;
    i -> bsdf = get_bsdf() ;
    r.max_t = t1 ;
    i -> t = t1 ;
    i -> n = n ;
    return true ;
  }

  return false ;
}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

bool Sphere::intersect(const Ray& r) const {}


void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CGL
