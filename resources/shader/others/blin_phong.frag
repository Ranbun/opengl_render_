#version 330 core

in LightData
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
}data;

out vec4  FragColor;

// �����λ�� 
uniform vec3 viewPos;

//��Դ��λ��
uniform vec3 lightPos;

uniform sampler2D  floorTexture;


void main()
{
	vec3 color = texture(floorTexture,data.Texcoords).rgb;

	// ��ɫ���½�׼
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
		vec3 reflectDir = reflect(-lightDir,normal);  //������� 
		float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);
		vec3 specular = vec3(0.3) * spec;
	#else
		// �����������ķ��� 
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal,halfwayDir),0.0),8.0);
		vec3 specular = vec3(0.3) * spec;

	#endif 


	FragColor = vec4(ambient + diffuse + specular, 1.0);

 
	// gamma ���� 
	FragColor.rgb = pow(FragColor.rgb,vec3(1.0/2.2));

}