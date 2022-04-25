#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;  // �������� 
out vec3 FragPos;    // ƬԪ��λ��
out vec3 Normal;     // ƬԪ�ķ���

// �����ڹ۲�ռ������ε����� 
void main()
{
    FragPos = vec3(view * model * vec4(aPos,1.0));
    TexCoords = aTexCoords;

    // ���߾���
    mat3 normalMat = transpose(inverse(mat3(view * model)));
    Normal = normalMat * aNormal;

    gl_Position = projection * view * model * vec4(aPos,1.0);
}
