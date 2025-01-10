#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include <algorithm>

class MeshManager {
public:
	std::vector<Mesh*> meshes; // List of meshes rendered every frame

	MeshManager();

	void addMesh(Mesh* mesh); // Adds mesh to list of meshes rendered every frame
	void removeMesh(Mesh* mesh); // Removes mesh from list of meshes rendered every frame

	Mesh* getMeshById(unsigned int _id); // Returns the mesh with the given id
	std::vector<Mesh*> getMeshesByName(std::string name); // Returns vector of meshes with the given name
	std::vector<Mesh*>* getMeshes(); // Returns the list of meshes

	void addCube(float width, float height, float depth, glm::vec3 centre, std::string name); // Adds a cube to the list of meshes

private:
	unsigned int maxID = 0;
};