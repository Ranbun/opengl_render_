#version 330 core

// 延迟渲染 光照阶段

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


struct Light
{
    vec3 Position;
    vec3 Color;

    float liner;
    float Quadratic;

    float Radius; // 光体积的半径 
};

// 光源的个数
const int NR_LIGHT = 32;
uniform Light lights[NR_LIGHT];
uniform vec3 viewPos;

void main()
{
    vec3 FragPos = texture(gPosition,TexCoords).rgb;
    vec3 Normal = texture(gNormal,TexCoords).rgb;
    vec4 temp = texture(gAlbedoSpec,TexCoords);
    vec3 Diffuse = temp.rgb;
    float Specular = temp.r;

    //计算光照
    vec3 lighting = Diffuse * 0.1;  // 环境光
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i =0; i < NR_LIGHT;i++)
    {
        // 光照方向
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal,lightDir),0.0) * Diffuse * lights[i].Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal,halfwayDir),0.0),16.0);
        vec3 specular = lights[i].Color * spec * Specular;

        // 衰减
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0/(1.0 + lights[i].liner * distance +lights[i].Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }
    FragColor = vec4(lighting,1.0);
}


#if 0

// 光照阶段 伪体积光 
void main()
{
    vec3 FragPos = texture(gPosition,TexCoords).rgb;
    vec3 Normal = texture(gNormal,TexCoords).rgb;
    vec4 temp = texture(gAlbedoSpec,TexCoords);
    vec3 Diffuse = temp.rgb;
    float Specular = temp.r;

    //计算光照
    vec3 lighting = Diffuse * 0.1;  // 环境光
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i =0; i < NR_LIGHT;i++)
    {
        float dis_of_light = length(lights[i].Position - FragPos);
        if(dis_of_light < lights[i].Radius)
        {
            // 光照方向
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal,lightDir),0.0) * Diffuse * lights[i].Color;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal,halfwayDir),0.0),16.0);
            vec3 specular = lights[i].Color * spec * Specular;

            // 衰减
            float distance = length(lights[i].Position - FragPos);
            float attenuation = 1.0/(1.0 + lights[i].liner * distance +lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }

    FragColor = vec4(lighting,1.0);
}

#endif 



