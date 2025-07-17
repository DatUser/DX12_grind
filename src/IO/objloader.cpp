#include "IO/objloader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <filesystem>

#include "external/tiny_obj_loader.h"

#include <iostream>
#include "Core/asserts.h"
#include "Engine/mesh.h"

#define MESHES_PATH			"models\\meshes\\"
#define MATERIALS_PATH		"models\\materials\\"
#define TEXTURES_PATH		"models\\textures\\"

bool parse_obj(
	const std::string& path,
	tinyobj::attrib_t& attrib,
	std::vector<tinyobj::shape_t>& shapes,
	std::vector<tinyobj::material_t>& materials
)
{
	std::string err;

	bool ret = tinyobj::LoadObj(
		&attrib,
		&shapes,
		&materials,
		&err,
		path.c_str(),
		MATERIALS_PATH
		);

	if (!err.empty())
	{
		std::cout << err << std::endl;
	}


	if (!ret)
		std::cerr << path << ": Could not be parsed !" << std::endl;
	else
		std::cout << path << ": Successfully loaded !" << std::endl;

	return ret;

}

void extract_vertices(
	tinyobj::attrib_t attrib,
	std::vector<float>& vVertices
)
{
	for (int i = 0; i < attrib.vertices.size(); i += 3)
	{
		vVertices.push_back(attrib.vertices[i]);
		vVertices.push_back(attrib.vertices[i+1]);
		vVertices.push_back(attrib.vertices[i+2]);
	}

	for (int i = 0; i < attrib.vertices.size(); i += 3)
	{
		vVertices.push_back(attrib.normals[i]);
		vVertices.push_back(attrib.normals[i+1]);
		vVertices.push_back(attrib.normals[i+2]);
	}
}

void extract_indices(
	const std::vector<struct tinyobj::shape_t> &shapes,
	std::vector<int> &vIndices
)
{
	for (auto&& shape : shapes)
	{
		for (auto&& index : shape.mesh.indices)
			vIndices.push_back(index.vertex_index);
	}
}

void extract_materials(
	std::vector<tinyobj::material_t>& materials
)
{
	for (auto mat : materials)
	{
	}
}

void load_obj(const std::string& sMeshFilename, std::vector<Mesh*>& vMeshes)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::filesystem::path oObjPath{MESHES_PATH};
	oObjPath /= sMeshFilename;
	if (!parse_obj(oObjPath.string(), attrib, shapes, materials))
		return;

	// Extract vertex and indices from parsed data
	std::vector<int> vIndices{};
	extract_indices(shapes, vIndices);
	std::vector<float> vVertices{};
	extract_vertices(attrib, vVertices);

	// Fills vector with read meshes
	Mesh* pCurrMesh = new Mesh{ vVertices, vIndices };
	CHECK(pCurrMesh != nullptr)
	vMeshes.push_back(pCurrMesh);

	std::cout << "Shape size: " << shapes.size() << std::endl;
	std::cout << "Face size: " << shapes[0].mesh.num_face_vertices.size() << std::endl;
	std::cout << "Pos size: " << attrib.vertices.size() << std::endl;
	std::cout << "Norm size: " << attrib.normals.size() << std::endl;
	std::cout << "Tex size: " << attrib.texcoords.size() << std::endl;
}
