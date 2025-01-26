#pragma once
#include "TextureManager.h"
#include "Shader_l.h"

class Material {
public:
	Texture* diffuse;
    Texture* specular;
    Texture* normal;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 ambientColor;
    glm::vec3 emissiveColor;
    float shininess;

    std::string name;

    Material(
        const std::string& _name = "Unnamed Material",
        const glm::vec3& _diffuseColor = glm::vec3(1.0f),
        const glm::vec3& _specularColor = glm::vec3(1.0f),
        const glm::vec3& _ambientColor = glm::vec3(1.0f),
        const glm::vec3& _emissiveColor = glm::vec3(0.0f),
        float _shininess = 32.0f,
        Texture* _diffuse = nullptr,
        Texture* _specular = nullptr,
        Texture* _normal = nullptr);

	void apply(Shader* _shader) const; // Applies material to shader (setup)
    void use() const; // Activates shader and material textures

};