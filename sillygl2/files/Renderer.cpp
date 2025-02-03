#include "Renderer.h"
#include "Manager.h"
Renderer::Renderer(ObjectManager* _objectManager) :
    projection(glm::mat4(1.0f)),
    model(glm::mat4(1.0f)),
    objectManager(_objectManager)
{
	Manager* manager = &Manager::getInstance();
    // Setup globally applied matrices
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //glm::perspective(glm::radians(manager->FOV), (float)manager->SCR_WIDTH / (float)manager->SCR_HEIGHT, 0.1f, 100.0f);

	// ubo buffer object, separate from VAO
	glGenBuffers(1, &UBOmatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection)); // projection likely isnt changing


	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOmatrices); // bound to ubo binding point 0


    glEnable(GL_DEPTH_TEST);

	preRenderTest();
}

void Renderer::setCamera(Camera* camera) {
    globalCamera = camera;
    view = &(globalCamera->view);
	projection = (globalCamera->projection);
	updatedView = &(globalCamera->updatedView);

	// buffer over UBO
	glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices); // send view and projection matrices to shader(s)
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
}

void Renderer::preRenderTest() {

	float width = 1.0f;
	float height = 1.0f;
	float depth = 1.0f;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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

	//std::shared_ptr<InstanceGroup> cubes = objectManager->createCubeInstanceGroup(1.0f, 1.0f, 1.0f, glm::vec3(0.0f), "cubes");
	//for (int i = 0; i < 13; i++) {
	//	std::shared_ptr<InstanceObject> ig = cubes->addInstance("coolmoment");
	//	ig->setPosition(rand_vec3(-5.0f, 5.0f));
	//}

	std::shared_ptr<Object> deleteThisAfterTest = objectManager->createObject("cool");
	deleteThisAfterTest->createRenderComponent();
	deleteThisAfterTest->getRenderComponent()->addVerticesIndices(vertices, indices);
	deleteThisAfterTest->getRenderComponent()->material->diffuse = TextureManager::createTexture(TextureType::Diffuse, "textures/hlbox.jpg");
	deleteThisAfterTest->getRenderComponent()->setupMaterial();

	//std::shared_ptr<Object> blankCube = objectManager->createObject("cool NOT TEX");
	//blankCube->createRenderComponent();
	//blankCube->getRenderComponent()->addVerticesIndices(vertices, indices);
	//blankCube->getRenderComponent()->getMaterial()->diffuse = TextureManager::defaultTexture(TextureType::Diffuse);
	//blankCube->getRenderComponent()->setupMaterial();


}

void Renderer::renderTest(float deltaTime) {
	static glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.01f, 0.0f, 0.0f)));
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// UBO Buffer group
	glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices); // send view and projection matrices to shader(s)
	// View buffer
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*view));

	objectManager->draw();


}