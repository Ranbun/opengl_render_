#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 

out vec2 TexCoords;

void main()
{
     vec4 position = projection * view * model * vec4(position, 1.0f);
     gl_Position = position;
     
    // TexCoords = texCoords;
    // 计算窗口上的纹理坐标 
    TexCoords = vec2(position.x /position.w,position.x/position.w) * 0.5 + 0.5;
}