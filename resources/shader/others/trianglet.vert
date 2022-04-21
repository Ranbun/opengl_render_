#version 330 core
layout (location = 0) in vec3 Posa;

void main()
{
    gl_Position = vec4(Posa.x,Posa.y,Posa.z,1.0f);
}