#pragma once
#include <string>
#include "graphics_headers.h"

class Texture {
	GLint m_texture_id;
public:
	Texture();
	Texture(const std::string filename);
	bool load_texture(const std::string filename);
	GLint get_texture_id();
};




