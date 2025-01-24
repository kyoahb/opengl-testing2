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
    projection = glm::perspective(glm::radians(manager->FOV), (float)manager->SCR_WIDTH / (float)manager->SCR_HEIGHT, 0.1f, 100.0f);

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
	updatedView = &(globalCamera->updatedView);
}

void Renderer::preRenderTest() {

	InstanceGroup* cubes = objectManager->createCubeInstanceGroup(1.0f, 1.0f, 1.0f, glm::vec3(0.0f), "cubes");
	for (int i = 0; i < 10; i++) {
		objectManager->addCube(cubes, rand_vec3(-5.0f, 5.0f), glm::vec3(0.0f), rand_vec3(0.1f, 5.0f), "cube");
	}

}

void Renderer::renderTest(float deltaTime) {
	static glm::quat rotation = glm::quat(glm::radians(glm::vec3(0.01f, 0.0f, 0.0f)));
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindBuffer(GL_UNIFORM_BUFFER, UBOmatrices); // send view and projection matrices to shader(s)
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*view));
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//for (auto& mesh : *(meshManager->getMeshes())) {
	//	mesh->drawSingleTexture();
	//}
	std::vector<InstanceGroup*> groups = *(objectManager->getInstanceGroups());
	for (InstanceGroup* group : groups) {
		group->draw();
	}
	
	//for (Instance* cube : groups[0]->instances) {
	//	cube->rotateQuat(rotation);
	//}
	//groups[0]->rotateEuler(deltaTime * glm::vec3(360.0f, 0.0f, 0.0f));
	//groups[0]->rotateEuler(glm::vec3(0.1f, 0.0f, 0.0f));
}