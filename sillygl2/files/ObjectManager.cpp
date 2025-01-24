#include "ObjectManager.h"

ObjectManager::ObjectManager(TextureManager* _textureManager) : textureManager(_textureManager) {

}

void ObjectManager::addInstanceGroup(InstanceGroup* instanceGroup) {
	instanceGroup->setId(maxID++);
	instanceGroups.push_back(instanceGroup);
}

std::vector<InstanceGroup*>* ObjectManager::getInstanceGroups() {
	return &instanceGroups;
}

InstanceGroup* ObjectManager::addMesh(Mesh* mesh) {
	std::vector<Texture*> textures = mesh->getTextures();
	if (textures.size() == 0) {
		Texture* texture = textureManager->createTexture(TextureType::Diffuse, "textures/hlbox.jpg");
		textures = { texture };
	}

	Shader* shader = mesh->getShader();
	if (shader == nullptr) {
		shader = new Shader("shaders/instance_shader.vert", "shaders/shader.frag");
	}

	InstanceGroup* instanceGroup = new InstanceGroup(mesh->getName(), mesh->getVertices(), mesh->getIndices(), textures, shader, mesh->getPosition(), mesh->getQuatRotation(), mesh->getScale());

	Instance* instance = new Instance("instance", mesh->getPosition(), mesh->getQuatRotation(), mesh->getScale());
	instanceGroup->addInstance(instance);

	addInstanceGroup(instanceGroup);
	return instanceGroup;
}

InstanceGroup* ObjectManager::createCubeInstanceGroup(float width, float height, float depth, glm::vec3 centre, std::string name) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

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
	Texture* texture = textureManager->createTexture(TextureType::Diffuse, "textures/hlbox.jpg");
	textures = { texture };

	InstanceGroup* cubes = new InstanceGroup(name, vertices, indices, textures, new Shader("shaders/instance_shader.vert", "shaders/shader.frag"), centre);
	addInstanceGroup(cubes);

	return cubes;
}

void ObjectManager::addCube(InstanceGroup* instanceGroup, const glm::vec3& centre, const glm::vec3& rotation, const glm::vec3& scale, const std::string& name) {
	Instance* cube = new Instance("cube", centre, rotation, scale);
	instanceGroup->addInstance(cube);
}