#pragma once
#include "object.h"

class Ring : public Object {
public:
	Ring(const std::string& texture_file, const std::string& normal_map_file);
	Ring(const std::string& texture_file);

private:
	void createVertices() override;
};
