#version 330 core

layout(location = 0) out vec4 gPositionDepth;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;  // 纹理坐标 
in vec3 FragPos;    // 片元的位置
in vec3 Normal;     // 片元的法线

const float NEAR = 0.1;
const float FAR = 50.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    // 计算一个线性的深度 
    return (2.0 * NEAR *FAR) / (FAR * NEAR - z * (FAR - NEAR));
}

void main()
{
    // 存储片段的位置到G缓冲
    gPositionDepth.xyz = FragPos;
    // 记录当前片段的深度到纹理的w分量
    gPositionDepth.a = LinearizeDepth(FragPos.z);
    // 记录法线的信息
    gNormal = normalize(Normal);

    // 漫反射的颜色的 -- 此处近似白色 显示SSAO 的效果
    gAlbedoSpec.rgb = vec3(0.95);
}


















