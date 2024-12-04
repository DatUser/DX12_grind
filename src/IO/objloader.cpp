#include "IO/objloader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tiny_obj_loader.h"

#include <iostream>
#include "Core/asserts.h"
#include "Engine/Mesh.h"

void load_obj(const std::string& path, std::vector<Mesh*>& vMeshes)
{
	/*
	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "../resources/";

	if (!reader.ParseFromFile(path, reader_config))
	{
	  if (!reader.Error().empty()) std::cerr << "TinyObjReader: " << reader.Error();

	  return nullptr;
	}

	if (!reader.Warning().empty()) std::cout << "TinyObjReader: " << reader.Warning();
	*/

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	// std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

	//std::vector<Mesh *> meshes{};

	if (!err.empty())
	{
		std::cerr << err << std::endl;
	}

	if (!ret)
		std::cerr << path << ": Could not be parsed !" << std::endl;
	else
		std::cout << path << ": Successfully loaded !" << std::endl;

	Mesh* pCurrMesh = new Mesh{};
	CHECK(pCurrMesh != nullptr)

	pCurrMesh->m_vVertices = attrib.vertices;

	// Here we load all meshes
	for (auto&& shape : shapes)
	{
		for (auto&& index : shape.mesh.indices)
			pCurrMesh->m_vIndices.push_back(index.vertex_index);
	}

	vMeshes.push_back(pCurrMesh);

	std::cout << "Shape size: " << shapes.size() << std::endl;
	std::cout << "Face size: " << shapes[0].mesh.num_face_vertices.size() << std::endl;
	std::cout << "Pos size: " << attrib.vertices.size() << std::endl;
	std::cout << "Norm size: " << attrib.normals.size() << std::endl;
	std::cout << "Tex size: " << attrib.texcoords.size() << std::endl;

	//return meshes;
}