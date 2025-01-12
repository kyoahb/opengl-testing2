#include "Renderer.h"
#include "Manager.h"
Renderer::Renderer(MeshManager* _meshManager) :
    shader(Shader("shaders/shader.vert", "shaders/shader.frag")),
    projection(glm::mat4(1.0f)),
    model(glm::mat4(1.0f)),
    meshManager(_meshManager)
{
	Manager* manager = &Manager::getInstance();
    // Setup globally applied matrices
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(manager->FOV), (float)manager->SCR_WIDTH / (float)manager->SCR_HEIGHT, 0.1f, 100.0f);

    // Using EBO buffers, so vertices can be reused
    // Indices are always needed with vertices in this approach.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    // Setup shader
    shader.use();
    //shader.setMat4("model", model);
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("projection", projection);

    preRenderTest();
}

void Renderer::setCamera(Camera* camera) {
    globalCamera = camera;
    view = &(globalCamera->view);
	updatedView = &(globalCamera->updatedView);
}

void Renderer::preRenderTest() {

	float width = 1.0f;
	float height = 1.0f;
	float depth = 1.0f;
	glm::vec3 centre = { 0.0f, 0.0f, 0.0f };

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
	Texture* texture = meshManager->textureManager->createTexture(TextureType::Diffuse, "textures/hlbox.jpg");
	textures = { texture };

    // Make a new instance group
	InstanceGroup* group = new InstanceGroup(vertices, indices, textures);
	meshManager->addInstanceGroup(group);
	for (int i = 0; i < 2250; i++) {
		group->addInstance(new Instance(rand_vec3(-10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "test"));
	}

}

void Renderer::renderTest() {
	static glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.1f, 0.0f, 0.0f)));
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (*updatedView) {
		*updatedView = false;
		shader.setMat4("view", *view);
	}
	//for (auto& mesh : *(meshManager->getMeshes())) {
	//	mesh->drawSingleTexture();
	//}
	std::vector<InstanceGroup*> groups = *(meshManager->getInstanceGroups());
	for (InstanceGroup* group : groups) {
		group->draw();
	}
	
	for (Instance* cube : groups[0]->instances) {
		cube->rotateQuat(rotation);
	}
}

Texture Renderer::createTestTexture() {

	char path[] = "textures/Trollface.png";
	return Texture(TextureType::Diffuse, path);
}