#include "camera.h"

Camera::Camera() {}

Camera::~Camera() {}

bool Camera::Initialize(int w, int h) {
  view = glm::lookAt(this->camera_pos,   // Eye Position
                     this->camera_front, // Focus point
                     this->camera_up);   // Positive Y is up

  projection = glm::perspective(
      glm::radians(this->fov), // the FoV typically 90 degrees is good which is
                               // what this is set to
      float(w) / float(h),     // Aspect Ratio, so Circles stay Circular
      0.01f,   // Distance to the near plane, normally a small value like this
      100.0f); // Distance to the far plane,

  return true;
}

void Camera::update_look_at(const glm::vec3 &delta, float pitch, float yaw) {
  this->camera_pos += delta;
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = -sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  this->camera_front = glm::normalize(direction);

  this->view = glm::lookAt(
      this->camera_pos, this->camera_pos + this->camera_front, this->camera_up);
}

void Camera::update_perspective(float fov_delta, int w, int h) {
  this->fov -= fov_delta;
  if (this->fov < 1.0f) {
    this->fov = 1.0f;
  } else if (this->fov > 60.0f) {
    this->fov = 60.0f;
  }

  projection = glm::perspective(glm::radians((float)this->fov),
                                float(w) / float(h), 0.01f, 100.0f);
}

glm::mat4 Camera::GetProjection() { return projection; }

glm::mat4 Camera::GetView() { return view; }

glm::vec3 &Camera::get_camera_front() { return this->camera_front; }

glm::vec3 &Camera::get_camera_up() { return this->camera_up; }

glm::vec3 &Camera::get_position() { return this->camera_pos; }
