#version 330 core

precision highp float;

#define kernelSize 64

out float FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

uniform mat4 projection;

// 窗口的分辨率 
uniform vec2 m_resolution;

// 这个纹理是一个4*4纹理 窗口可能是宽高不等的 我们需要让纹理填充到这个分辨率大小屏幕 需要做一个处理 
vec2 noiseScale = vec2(m_resolution.x / 4.0, m_resolution.y/4.0);

void main()
{
    vec3 fragPos = texture(gPositionDepth,TexCoords).xyz;
    vec3 normal = texture(gNormal,TexCoords).xyz;
    vec3 randomVec = texture(texNoise,TexCoords * noiseScale).xyz;

    // 计算TBN矩阵
    vec3 tangent = normalize(randomVec - normal * dot(randomVec,normal));
    vec3 bitmangent = cross(normal,tangent);
    mat3 TBN = mat3(tangent,bitmangent,normal);

    // 计算遮罩因子 
    float occlusion = 0.0;
    float radius = 1.0;
    
    for(int i=0;i < kernelSize;i++)
    {
        vec3 sampleVec = TBN * samples[i];
        sampleVec = fragPos + sampleVec * radius;

        // 将这个位置变换到裁剪坐标 
        vec4 offset = projection * vec4(sampleVec, 1.0);
        offset.xyz /= offset.w;   // 透视除法 
        offset.xyz = offset.xyz * 0.5 + 0.5;  // 变换到 0 - 1 获取一个线性深度 

        // 某个位置的片段的深度 -- 这个位置是被加了偏移的位置的深度(当前的屏幕坐标对应的最小深度 ) 
        float sampleDepth = -texture(gPositionDepth, offset.xy).w;

        // 做一个范围约束 保证深度只在取样半径内才会影响遮蔽因子 
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        // occlusion += (sampleDepth >= sampleVec.z ? 1.0 : 0.0);
        // c此时表示被遮挡 
        occlusion += (sampleDepth >= sampleVec.z ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1- occlusion/kernelSize;
    FragColor = occlusion;
}




