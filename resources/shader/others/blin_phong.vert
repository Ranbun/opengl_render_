#version 330 core
layout(location=2) in vec2 aTexCoords;
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;


out LightData
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
}data;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	data.FragPos = vec3(model * vec4(aPos,1.0));
	data.Normal = aNormal;
	data.Texcoords = aTexCoords;
	gl_Position = projection * view * vec4(data.FragPos,1.0);
}


