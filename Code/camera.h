#pragma once

#include "graphics_headers.h"

class Camera {
public:
  Camera();
  ~Camera();
  bool Initialize(int w, int h);
  glm::mat4 GetProjection();
  glm::mat4 GetView();
  void update_view(float pitch, float yaw);
  void update_pos(const glm::vec3 &delta) { this->camera_pos += delta; }
  glm::vec3 &get_camera_front() { return this->camera_front; }
  glm::vec3 &get_camera_up() { return this->camera_up; }

private:
  glm::vec3 camera_pos = glm::vec3(0, 0, 40.f);
  glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 view;
  glm::mat4 projection;
};
