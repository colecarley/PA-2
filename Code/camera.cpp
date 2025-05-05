#include "camera.h"

Camera::Camera() {}

Camera::~Camera() {}

bool Camera::Initialize(int w, int h) {
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more
  //  dynamic
  //  ...Like you should update it before you render more dynamic
  //  for this project having them static will be fine

  // TO DO: Make the camera to be placed at location (10, 12, -20), point at
  // origin, and the right-handed Y-up coordinate
  view = glm::lookAt(this->camera_pos,   // Eye Position
                     this->camera_front, // Focus point
                     this->camera_up);   // Positive Y is up

  projection = glm::perspective(
      glm::radians(60.f),  // the FoV typically 90 degrees is good which is what
                           // this is set to
      float(w) / float(h), // Aspect Ratio, so Circles stay Circular
      0.01f,   // Distance to the near plane, normally a small value like this
      100.0f); // Distance to the far plane,

  // To DO:
  // Change your camera to orthographic
  // projection = glm::ortho(float(-w / 100), float(w / 100), -float(h/100),
  // float(h/100), -20.f, 20.f);

  return true;
}

void Camera::update_view(float pitch, float yaw) {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = -sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  this->camera_front = glm::normalize(direction);

}

void Camera::update_pos(const glm::vec3 &delta) { 
	this->camera_pos += delta; 
}
	
void Camera::update_look_at() {
  this->view = glm::lookAt(
      this->camera_pos, this->camera_pos + this->camera_front, this->camera_up);
}

glm::mat4 Camera::GetProjection() { return projection; }

glm::mat4 Camera::GetView() { return view; }

glm::vec3 &Camera::get_camera_front() { 
	return this->camera_front; 
}

glm::vec3 &Camera::get_camera_up() { 
	return this->camera_up; 
}


