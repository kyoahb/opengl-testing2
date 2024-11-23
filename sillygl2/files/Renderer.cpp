#include "Renderer.h"

Renderer::Renderer(ObjectManager* objManager, unsigned int scr_width, unsigned int scr_height) :
    shader(Shader("shaders/shader.vs", "shaders/shader.fs")),
    projection(glm::mat4(1.0f)),
    model(glm::mat4(1.0f)),
    objectManager(objManager),
    SCR_WIDTH(scr_width),
    SCR_HEIGHT(scr_height)
{
    objects = objectManager->getObjects();
    verticesUpdated = objectManager->haveObjectsUpdated();

    // Setup globally applied matrices
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

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
    shader.setMat4("model", model);
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("projection", projection);
}

void Renderer::setCamera(Camera* camera) {
    globalCamera = camera;
    view = &(globalCamera->view);
}

void Renderer::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (view) {
        shader.setMat4("view", *view);
    }

    verticesUpdated = objectManager->haveObjectsUpdated();
    if (verticesUpdated) {
        std::vector<glm::vec3> combined_vertices;
        std::vector<unsigned int> combined_indices;
        firsts.clear();
        counts.clear();

        GLsizei vertex_offset = 0;
        for (size_t i = 0; i < objects->size(); ++i) {
            GameObject* obj = (*objects)[i];
            const auto& verts = obj->vertices;
            const auto& inds = obj->indices;

            firsts.push_back(static_cast<GLint>(combined_indices.size()));
            counts.push_back(static_cast<GLsizei>(inds.size()));

            combined_vertices.insert(combined_vertices.end(), verts.begin(), verts.end());
            for (const auto& ind : inds) {
                combined_indices.push_back(ind + vertex_offset);
            }

            vertex_offset += static_cast<GLsizei>(verts.size());
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, combined_vertices.size() * sizeof(glm::vec3), combined_vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, combined_indices.size() * sizeof(unsigned int), combined_indices.data(), GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(VAO);
    for (size_t i = 0; i < counts.size(); ++i) {
        glDrawElements(GL_TRIANGLES, counts[i], GL_UNSIGNED_INT, (void*)(firsts[i] * sizeof(unsigned int)));
    }
    glBindVertexArray(0);
}