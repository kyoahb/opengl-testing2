#include "Material.h"

Material::Material(
	const std::string& _name,
	const glm::vec3& _diffuseColor,
	const glm::vec3& _specularColor,
	const glm::vec3& _ambientColor,
	const glm::vec3& _emissiveColor,
	float _shininess,
	Texture* _diffuse,
	Texture* _specular,
	Texture* _normal)
	: name(_name),
	diffuseColor(_diffuseColor),
	specularColor(_specularColor),
	ambientColor(_ambientColor),
	emissiveColor(_emissiveColor),
	shininess(_shininess),
	diffuse(_diffuse),
	specular(_specular),
	normal(_normal)
{
	if (diffuse == nullptr) {
		diffuse = TextureManager::defaultTexture(TextureType::Diffuse);
	}
	if (specular == nullptr) {
		specular = TextureManager::defaultTexture(TextureType::Specular);
	}
	if (normal == nullptr) {
		normal = TextureManager::defaultTexture(TextureType::Normal);
	}
}

void Material::apply(Shader* _shader) const {

	_shader->use();
	//shader->setVec3("material.diffuse", diffuseColor);
	//shader->setVec3("material.specular", specularColor);
	//shader->setVec3("material.ambient", ambientColor);
	//shader->setVec3("material.emissive", emissiveColor);
	//shader->setFloat("material.shininess", shininess);

	diffuse->use(0);
	_shader->setInt("material.texture_diffuse", 0);

	//specular->use(1);	
	//shader->setInt("material.texture_specular", 1);

	//normal->use(2);
	//shader->setInt("material.texture_normal", 2);
}

void Material::use() const {
	diffuse->use(0);
	specular->use(1);
	normal->use(2);
}