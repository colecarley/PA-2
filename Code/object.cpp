#include "object.h"

Object::Object() {}
Object::~Object() {
  Vertices.clear();
  Indices.clear();
}

// void Object::Update(unsigned int dt) {

//   model = glm::translate(model, m_speed);

//   // To Do: Extra credit / Grad Only
//   //	Make the object move in a circle around the origin
// }

// glm::mat4 Object::GetModel() { return model; }

// void Object::Render(GLint posAttribLoc, GLint colAttribLoc) {
//   // Bind VAO
//   glBindVertexArray(vao);

//   // Bind VBO(s)
//   glBindBuffer(GL_ARRAY_BUFFER, VB);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

//   // enable the vertex attribute arrays
//   // this is the poistion attrib in the vertex shader
//   glEnableVertexAttribArray(posAttribLoc);
//   // this is the color attribe in the vertex shader
//   glEnableVertexAttribArray(colAttribLoc);

//   // Draw call to OpenGL
//   glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

//   // disable the vertex attributes
//   glDisableVertexAttribArray(posAttribLoc);
//   glDisableVertexAttribArray(colAttribLoc);

//   // unbind VBO(s) and ElementBuffer(s)
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// }
