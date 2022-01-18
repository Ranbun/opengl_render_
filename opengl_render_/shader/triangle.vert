#version 450 core
layout(location = 0) in vec3 aPos;
// out vec4 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	VertexColor = vec4(0.5,0.0,0.0,1.0);
}