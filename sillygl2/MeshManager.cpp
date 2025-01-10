#include "MeshManager.h"

MeshManager::MeshManager() : meshes({}) {

}

void MeshManager::addMesh(Mesh* mesh) {
	mesh->id = maxID;
	maxID += 1;
	meshes.push_back(mesh);
}

void MeshManager::removeMesh(Mesh* mesh) {
	meshes.erase(std::remove(meshes.begin(), meshes.end(), mesh), meshes.end());
}

std::vector<Mesh*>* MeshManager::getMeshes() {
	return &meshes;
}

Mesh* MeshManager::getMeshById(unsigned int _id) {
	for (Mesh* mesh : meshes) {
		if (mesh->id == _id) {
			return mesh;
		}
	}
	return nullptr;
}

std::vector<Mesh*> MeshManager::getMeshesByName(std::string name) {
	std::vector<Mesh*> foundMeshes;
	for (Mesh* mesh : meshes) {
		if (mesh->name == name) {
			foundMeshes.push_back(mesh);
		}
	}
	return foundMeshes;
}

void MeshManager::addCube(float width, float height, float depth, glm::vec3 centre, std::string name) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	vertices = {
		Vertex(glm::vec3(-width / 2, -height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, -height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-width / 2, height / 2, -depth / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
		Vertex(glm::vec3(-width / 2, -height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, -height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(width / 2, height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-width / 2, height / 2, depth / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
	};

	indices = {
		0, 1, 2, 
		2, 3, 0, 
		1, 5, 6, 
		6, 2, 1, 
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3
	};

	textures = { Texture(TextureType::Diffuse, "textures/hlbox.jpg") };

	Mesh* mesh = new Mesh(vertices, indices, textures, centre, glm::vec3(0.0f), glm::vec3(1.0f), "cube");

	addMesh(mesh);
}