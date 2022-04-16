#version 330 core
layout(location = 0) out vec3 gPositon;     // 片元的位置
layout(location = 1) out vec3 gNormal;      // 片元的法向量
layout(location = 2) out vec4 gAlbedSpec;   // 镜面反射系数 + 漫反射的颜色

in vec2 TexCoords; // Texcoord
in vec3 FragPos;   // 片段在世界坐标的位置
in vec3 Normal;    // 片段的法向量

uniform sampler2D texture_deffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // 片段位置
    gPositon = FragPos;
    // 法向量
    gNormal = Normal;
    // 计算 颜色 + specular系数
    gAlbedSpec.rgb = texture(texture_deffuse1,TexCoords).rgb;
    gAlbedSpec.r   = texture(texture_specular1,TexCoords).r;
}



