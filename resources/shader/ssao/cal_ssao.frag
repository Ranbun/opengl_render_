#version 330 core

precision highp float;

#define kernelSize 64

out float FragColor;
in vec2 texCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;

uniform vec2 m_resolution;

vec2 noiseScale = vec2(m_resolution.x / 4.0, m_resolution.y/4.0);

void main()
{
    vec3 fragPos = texture(gPositionDepth,texCoords).xyz;
    vec3 normal = texture(gNormal,texCoords).xyz;
    vec3 randomVec = texture(texNoise,texCoords * noiseScale).xyz;

    // ����TBN����
    vec3 tangent = normalize(randomVec - normal * dot(randomVec,normal));
    vec3 bitmangent = cross(normal,tangent);
    mat3 TBN = mat3(tangent,bitmangent,normal);

    // ������������ 
    float occlusion = 0.0;
    float radius = 1.0;
    
    for(int i=0;i < kernelSize;i++)
    {
        vec3 sampleVec = TBN * samples[i];
        sampleVec = fragPos + sampleVec * radius;
        vec4 offset = projection * vec4(sampleVec, 1.0);
        offset.xyz /= offset.w;   // ͸�ӳ��� 
        offset.xyz = offset.xyz * 0.5 + 0.5;  // �任�� 0 - 1 ��ȡһ��������� 

        // ĳ��λ�õ�Ƭ�ε���� -- ���λ���Ǳ�����ƫ�Ƶ�λ�õ���� 
        float sampleDepth = -texture(gPositionDepth, offset.xy).w;

        // ��һ����ΧԼ�� ��֤���ֻ��ȡ���뾶�ڲŻ�Ӱ���ڱ����� 
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        // occlusion += (sampleDepth >= sampleVec.z ? 1.0 : 0.0);
        occlusion += (sampleDepth >= sampleVec.z ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1- occlusion/kernelSize;
    FragColor = occlusion;

}




