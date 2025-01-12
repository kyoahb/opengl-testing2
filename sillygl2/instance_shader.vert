#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;


out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 group;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * group * vec4(aPos, 1.0);
    ourColor = vec3(aPos.x, aPos.y, aPos.z);
    TexCoord = aTexCoord;
}