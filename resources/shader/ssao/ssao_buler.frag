#version 330 core 
in vec2 TexCoords;
out float FragColor;

// ssao �ļ����� 
uniform sampler2D ssaoTexture;
const int bluerSize = 4;

void main()
{
    // ��ȡ���صĴ�С  
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoTexture,0));
    float result = 0.0;
    for(int x = 0; x < bluerSize;x++)
    {
        for(int y = 0;y < bluerSize;y++)
        {
            vec2 offset = (vec2(-2.0) + vec2(float(x),float(y))) * texelSize;
            result += texture(ssaoTexture,TexCoords+offset).r;
        }
    }
    // ����ƽ��ֵ 
    FragColor = result / float(bluerSize * bluerSize);
}
