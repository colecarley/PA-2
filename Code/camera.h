#pragma once
#include "graphics_headers.h"

class Camera {
public:
  Camera();
  ~Camera();
  bool Initialize(int w, int h);
  glm::mat4 GetProjection();
  glm::mat4 GetView();
  void update_look_at(const glm::vec3 &delta, float pitch, float yaw);
  void update_perspective(float fov, int h, int w);
  glm::vec3 &get_front();
  glm::vec3 &get_up();
  glm::vec3 &get_position();
  void set_front(glm::vec3 f);
  void set_up(glm::vec3 u);
  void set_position(glm::vec3 p);
  void set_view(glm::mat4 v);

private:
  glm::vec3 camera_pos = glm::vec3(0, 0, 8);
  glm::vec3 camera_front = glm::vec3(0, 0, 0);
  glm::vec3 camera_up = glm::vec3(0, 1, 0);
  float fov = 60.0f;
  glm::mat4 view;
  glm::mat4 projection;
};
