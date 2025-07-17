#pragma once


#include <vector>
#include <string>

class Mesh;

void load_obj(const std::string& sMeshFilename, std::vector<Mesh*>& arrMeshes);