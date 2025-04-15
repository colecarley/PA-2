#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere : public Object {
public:
  Sphere();

private:
  void createVertices() override;
};

#endif /* SPHERE_H */