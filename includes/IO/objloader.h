#pragma once


#include <vector>
#include <string>

class mesh;

void load_obj(const std::string& path, std::vector<mesh*>& arrMeshes);