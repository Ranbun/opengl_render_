#version 330 core
out vec4 FragColor;
in vec4 VertexColor;
void main()
{
	// FragColor = vec4(1.0,0.5,0.2,1.0f);
	FragColor = VertexColor;
}