#include "light.h"

Light::Light(glm::vec4 viewMatrix) {
  glm::vec4 temp = glm::vec4((viewMatrix * glm::vec4(light_position, 1.0f)));
  light_position_view_space[0] = temp.x;
  light_position_view_space[1] = temp.y;
  light_position_view_space[2] = temp.z;
}
