#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct sPlyVertexXYZ_N {
	float x, y, z;
	float nx, ny, nz;	// Normals
};

struct sPlyTriangle {
	unsigned int vert_index_1;
	unsigned int vert_index_2;
	unsigned int vert_index_3;
};

class cMesh {
public:
	std::vector<sPlyVertexXYZ_N> vecVertices;
	std::vector<sPlyTriangle> vecTriangles;
};
