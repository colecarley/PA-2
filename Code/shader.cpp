#include "shader.h"

Shader::Shader() { m_shaderProg = 0; }

Shader::~Shader() {
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin();
       it != m_shaderObjList.end(); it++) {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0) {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize() {
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

bool Shader::AttachShader(std::string s, GLenum ShaderType) {
  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar *p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = {(GLint)s.size()};

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}

// Use this method to add shaders to the program. When finished - call
// finalize()
bool Shader::AddShader(GLenum ShaderType) {
  std::string s;

  if (ShaderType == GL_VERTEX_SHADER) {
    s = "#version 410\n \
      \
      layout (location = 0) in vec3 v_position; \
      layout (location = 1) in vec3 v_normal; \
      layout (location = 2) in vec2 v_texture; \
      layout (location = 3) in vec4 instance_mat_0; \
      layout(location = 4) in vec4 instance_mat_1; \
      layout(location = 5) in vec4 instance_mat_2; \
      layout(location = 6) in vec4 instance_mat_3; \
      \
			out vec3 norm; \
			out vec2 tex_coord; \
			out vec3 frag_pos;\
      \
      uniform mat4 projection; \
      uniform mat4 view; \
      uniform mat4 model; \
      uniform bool use_instancing; \
      \
      void main(void) \
      { \
        mat4 instance_mat = mat4(instance_mat_0, instance_mat_1, instance_mat_2, instance_mat_3); \
        mat4 local_model = use_instancing ? instance_mat : model;\
        vec4 v = vec4(v_position, 1.0); \
        gl_Position = (projection * view * local_model) * v; \
		tex_coord = v_texture; \
		norm = mat3(transpose(inverse(local_model))) * v_normal; \
		frag_pos = vec3(local_model * v);\
      } \
    ";
  } else if (ShaderType == GL_FRAGMENT_SHADER) {
    s = "#version 410\n \
			struct Material {\
				vec3 ambient;\
				vec3 diffuse;\
				vec3 specular;\
				float shininess;\
			};\
      \
      layout (location = 0) in vec2 tex_coord; \
			layout (location = 1) in vec3 norm; \
			uniform bool is_emissive;\
			uniform bool has_normal_map;\
			uniform sampler2D samp;\
			uniform sampler2D samp2;\
			uniform vec3 light_pos; \
			uniform vec3 light_color;\
			uniform vec3 view_pos;\
			uniform Material material; \
      \
      out vec4 frag_color; \
			in vec3 frag_pos;\
      \
      void main(void) \
      { \
				vec4 color = texture(samp, tex_coord);\
				if (is_emissive) {\
					frag_color = color;\
				} else {\
					vec3 normal;\
					if (has_normal_map) {\
						normal = normalize(norm + texture(samp2, tex_coord).xyz * 2 - 1);\
					}\
					normal = normalize(norm);\
					vec3 light_dir = normalize(light_pos - frag_pos);\
					float diff = max(dot(normal, light_dir), 0.0);\
					vec3 diffuse = (diff * material.diffuse) * light_color;\
					vec3 view_dir = normalize(view_pos - frag_pos);\
					vec3 reflect_dir = reflect(-light_dir, normal);\
					float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);\
					vec3 specular = (material.specular * spec) * light_color;\
					vec3 ambient = material.ambient * light_color;\
					vec3 result = (ambient + diffuse + specular) * vec3(color);\
					frag_color = vec4(result, 1.0f);\
				}\
			}";
  }

  return AttachShader(s, ShaderType);
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize() {
  GLint Success = 0;
  GLchar ErrorLog[1024] = {0};

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  // glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin();
       it != m_shaderObjList.end(); it++) {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}

void Shader::Enable() { glUseProgram(m_shaderProg); }

GLint Shader::GetUniformLocation(const char *pUniformName) {
  GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

  if (Location == INVALID_UNIFORM_LOCATION) {
    fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n",
            pUniformName);
  }

  return Location;
}

GLint Shader::GetAttribLocation(const char *pAttribName) {
  GLuint Location = glGetAttribLocation(m_shaderProg, pAttribName);
  if (Location == GL_INVALID_VALUE) {
    fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n",
            pAttribName);
  }
  return Location;
}
