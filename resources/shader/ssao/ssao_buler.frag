#version 330 core 
in vec2 TexCoords;
out float FragColor;

// ssao 的计算结果 
uniform sampler2D ssaoTexture;
const int bluerSize = 4;

void main()
{
    // 获取纹素的大小  
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
    // 计算平均值 
    FragColor = result / float(bluerSize * bluerSize);
}
