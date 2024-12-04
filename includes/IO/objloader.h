#pragma once


#include <vector>
#include <string>

class Mesh;

void load_obj(const std::string& path, std::vector<Mesh*>& arrMeshes);