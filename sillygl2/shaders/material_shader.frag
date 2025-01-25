#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 emissive;
    float shininess;
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
};

uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse, TexCoord);
}