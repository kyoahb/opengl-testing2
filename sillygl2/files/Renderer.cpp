#include "Renderer.h"

Renderer::Renderer(MeshManager* _meshManager, unsigned int scr_width, unsigned int scr_height) :
    shader(Shader("shaders/shader.vert", "shaders/shader.frag")),
    projection(glm::mat4(1.0f)),
    model(glm::mat4(1.0f)),
    meshManager(_meshManager),
    SCR_WIDTH(scr_width),
    SCR_HEIGHT(scr_height)
{
    // Setup globally applied matrices
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//texture = createTestTexture();

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
}

void Renderer::setCamera(Camera* camera) {
    globalCamera = camera;
    view = &(globalCamera->view);
	updatedView = &(globalCamera->updatedView);
}

void Renderer::renderTest() {
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (*updatedView) {
		*updatedView = false;
		shader.setMat4("view", *view);
	}
	for (auto& mesh : *(meshManager->getMeshes())) {
		mesh->draw(shader);
	}
}

Texture Renderer::createTestTexture() {

	char path[] = "textures/Trollface.png";
	return Texture(TextureType::Diffuse, path);
}
/*
void Renderer::render() {
    glBindVertexArray(VAO);

	if (smallRender && !largeRender) { // Smaller, vertice-only update.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, combinedVertices.size() * sizeof(glm::vec3), combinedVertices.data());
	}
	else if (largeRender) { // Larger update, resending all data.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(glm::vec3), combinedVertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, combinedIndices.size() * sizeof(unsigned int), combinedIndices.data(), GL_DYNAMIC_DRAW);
	}

    for (size_t i = 0; i < numIndices.size(); ++i) {
		GameObject* obj = (objects)[i];
        if (obj->visible) { // ONLY draw if object is visible
            glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, (void*)(firstIndices[i] * sizeof(unsigned int)));
        }
    }
    glBindVertexArray(0);
}
*/
