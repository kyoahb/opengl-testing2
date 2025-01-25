#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 group = mat4(1.0f);
layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
};

void main()
{
    // Use the 'hi' uniform to prevent it from being optimized out
    gl_Position = projection * view * model * group * vec4(aPos, 1.0) ;
    TexCoord = aTexCoord;
    FragPos = aPos;
    Normal = aNormal;
    TexCoord = aTexCoord;
}