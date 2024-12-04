#include "Engine/Mesh.h"

Mesh::Mesh()
: m_vVertices()
, m_vIndices()
{
}

Mesh::Mesh(const std::vector<float>& vVertices, const std::vector<int>& vIndices)
: m_vVertices(m_vVertices)
, m_vIndices(m_vIndices)
{
}

Mesh::~Mesh()
{
}