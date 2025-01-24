#include "Mesh.h"

Mesh::Mesh(const std::string& _name,
    const std::vector<Vertex>& _vertices,
    const std::vector<unsigned int>& _indices,
    const std::vector<Texture*>& _textures,
    Shader* _shader,
    const glm::vec3& _position,
    const glm::quat& _rotation,
    const glm::vec3& _scale)
    : VertexObject(_name, _vertices, _indices, _textures, _shader, _position, _rotation, _scale), children({}), attachedCamera(nullptr)
{
	setupMesh();
    formModelMatrix();
    resendModelMatrix();
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

void Mesh::attachCamera(Camera* camera) {
	attachedCamera = camera;
}

void Mesh::move(const glm::vec3& change) {
    VertexObject::move(change);
    for (Mesh* child : children) {
        child->move(change);
    }
    if (attachedCamera) {
        attachedCamera->move(change);
    }
}

void Mesh::rotateEuler(const glm::vec3& _rotation) {
    VertexObject::rotateEuler(_rotation);
    for (Mesh* child : children) {
        child->rotateEuler(_rotation);
    }
    if (attachedCamera) {
        attachedCamera->rotateEuler(_rotation);
    }
}

void Mesh::rotateQuat(const glm::quat& _rotation) {
    VertexObject::rotateQuat(_rotation);
    for (Mesh* child : children) {
        child->rotateQuat(_rotation);
    }
    if (attachedCamera) {
        attachedCamera->rotateEuler(glm::degrees(glm::eulerAngles(_rotation)));
    }
}

void Mesh::setRotation(glm::quat _rotation) {
	VertexObject::setRotation(_rotation);
}

void Mesh::addScale(const glm::vec3& _scale) {
    VertexObject::addScale(_scale);
    for (Mesh* child : children) {
        child->addScale(_scale);
    }
}


void Mesh::setId(unsigned int _id) {
    id = _id;
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
		Texture* currentTexture = textures[i];
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
        glBindTexture(GL_TEXTURE_2D, currentTexture->id);
    }
    glActiveTexture(GL_TEXTURE0);


	//glBindTexture(GL_TEXTURE_2D, textures[0].id);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Draws but only with a maximum of one texture for simplicity.
void Mesh::drawSingleTexture() {
    if (!textures.empty()) {
        glBindTexture(GL_TEXTURE_2D, textures[0]->id);
	}

	if (modifiedThisFrame) {
        modifiedThisFrame = false;
		formModelMatrix();
		resendModelMatrix();
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
    model = VertexObject::calculateModelMatrix();
}