#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;  // 纹理坐标 
out vec3 FragPos;    // 片元的位置
out vec3 Normal;     // 片元的法线

void main()
{
    FragPos = vec3(model * vec4(aPos,1.0));
    TexCoords = aTexCoords;
    Normal = aNormal;

    gl_Position = projection * view * model * vec4(aPos,1.0);
}
