#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, std::string _name)
    : vertices(_vertices), indices(_indices), textures(_textures), position(_position), scale(_scale), rotation(_rotation), model(glm::mat4(1.0f)), name(_name), id(0), children({}), attachedCamera(nullptr)
{
	setupMesh();
    formSendModelMatrix();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
	glGenBuffers(1, &modelBuffer);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &model, GL_STATIC_DRAW);

    // model matrix
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Mesh::move(glm::vec3 change) {
	position += change;
	formSendModelMatrix();

	for (Mesh* child : children) {
		child->move(change);
	}
	if (attachedCamera != nullptr) {
		attachedCamera->move(change);
	}
}

void Mesh::rotate(glm::vec3 _rotation) {
	rotation += _rotation;
	rotation = vec3Overfill(rotation, 0.0f, 360.0f);
	formSendModelMatrix();

	for (Mesh* child : children) {
		child->rotate(_rotation);
	}
    if (attachedCamera != nullptr) {
        attachedCamera->changeDirection(_rotation);
    }
}

void Mesh::addScale(glm::vec3 _scale) {
	scale += _scale;
	formSendModelMatrix();

	for (Mesh* child : children) {
		child->addScale(_scale);
	}
}

void Mesh::attachCamera(Camera* camera) {
	attachedCamera = camera;
}

void Mesh::draw(Shader& shader)
{
    // Binds all textures
    //unsigned int diffuseNr = 1;
    //unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string typeString;
		Texture& currentTexture = textures[i];
        /*
        TextureType type = currentTexture.type;
        if (type == TextureType::Diffuse) {
            number = std::to_string(diffuseNr++);
			typeString = "texture_diffuse";
        }
        else if (type == TextureType::Specular) {
            number = std::to_string(specularNr++);
			typeString = "texture_specular";
        }*/



        //shader.setInt(("material." + typeString + number).c_str(), i);
		//shader.setInt("material.diffuse", 0);
        glBindTexture(GL_TEXTURE_2D, currentTexture.id);
    }
    glActiveTexture(GL_TEXTURE0);


	//glBindTexture(GL_TEXTURE_2D, textures[0].id);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Draws but only with a maximum of one texture for simplicity.
void Mesh::drawSingleTexture(Shader& shader) {
    if (!textures.empty()) {
        glBindTexture(GL_TEXTURE_2D, textures[0].id);
	}

    //resend model if necessary
    //if (mPosition != position || mScale != scale || mRotation != rotation) {
    //    formSendModelMatrix();
    //}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::formSendModelMatrix() {
	formModelMatrix();
	resendModelMatrix();
}

void Mesh::resendModelMatrix() {
    glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &model);
    glBindVertexArray(0);
}

void Mesh::resendVerticesIndices() {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::formModelMatrix() {
    // calculate new model
    glm::mat4 newModel = glm::mat4(1.0f);
    newModel = glm::translate(newModel, position);
    newModel = glm::scale(newModel, scale);
    newModel = glm::rotate(newModel, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    newModel = glm::rotate(newModel, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    newModel = glm::rotate(newModel, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // set rendered flags
    mPosition = position;
    mScale = scale;
    mRotation = rotation;

    // update model
    model = newModel;
}