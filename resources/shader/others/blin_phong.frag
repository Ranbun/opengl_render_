#version 330 core

in LightData
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
}data;

out vec4  FragColor;

// 相机的位置 
uniform vec3 viewPos;

//光源的位置
uniform vec3 lightPos;

uniform sampler2D  floorTexture;


void main()
{
	vec3 color = texture(floorTexture,data.Texcoords).rgb;

	// 颜色重新矫准
	// color = pow(color,vec3(2.2));

	vec3 ambient = 0.05 * color;
	
	// diffuse  -- phong
	vec3 lightDir = normalize(lightPos - data.FragPos);
	vec3 normal = normalize(data.Normal);

	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuse = diff * color;

	// specular 
	vec3 viewDir = normalize(viewPos - data.FragPos);

	#if 0
		vec3 reflectDir = reflect(-lightDir,normal);  //反射光线 
		float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);
		vec3 specular = vec3(0.3) * spec;
	#else
		// 计算半程向量的方向 
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal,halfwayDir),0.0),8.0);
		vec3 specular = vec3(0.3) * spec;

	#endif 


	FragColor = vec4(ambient + diffuse + specular, 1.0);

 
	// gamma 矫正 
	FragColor.rgb = pow(FragColor.rgb,vec3(1.0/2.2));

}