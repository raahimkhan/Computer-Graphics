#include <nanogui/nanogui.h>

#include "../clothMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;

void Sphere::collide(PointMass &pm) {

  Vector3D dir = pm.position - origin ;

  if(dir.norm() <= radius)
  {
      Vector3D pointOfIntersection = origin + dir.unit() * radius ;
      Vector3D correctionVector = pointOfIntersection - pm.last_position ;
      pm.position = pm.last_position + (1 - friction) * correctionVector ;
  }
}

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  Misc::draw_sphere(shader, origin, radius * 0.92);
}
