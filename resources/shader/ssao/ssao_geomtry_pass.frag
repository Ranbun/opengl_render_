#version 330 core

layout(location = 0) out vec4 gPositionDepth;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;  // �������� 
in vec3 FragPos;    // ƬԪ��λ��
in vec3 Normal;     // ƬԪ�ķ���

const float NEAR = 0.1;
const float FAR = 50.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    // ����һ�����Ե���� 
    return (2.0 * NEAR *FAR) / (FAR * NEAR - z * (FAR - NEAR));
}

void main()
{
    // �洢Ƭ�ε�λ�õ�G����
    gPositionDepth.xyz = FragPos;
    // ��¼��ǰƬ�ε���ȵ������w����
    gPositionDepth.a = LinearizeDepth(FragPos.z);
    // ��¼���ߵ���Ϣ
    gNormal = normalize(Normal);

    // ���������ɫ�� -- �˴����ư�ɫ ��ʾSSAO ��Ч��
    gAlbedoSpec.rgb = vec3(0.95);
}


















