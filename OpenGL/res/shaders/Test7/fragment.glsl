#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor; 

in vec3 FragPos;
in vec3 Normal;
in Light LightVS;

uniform Material material;

// Phong lighting model
void main()
{
	// Ambiant
	vec3 ambient = LightVS.ambient * material.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightVS.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = LightVS.diffuse * (diff * material.diffuse);

	// Specular
	vec3 viewDir = normalize(-FragPos); // Because in view space viewPos : 0,0,0
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = LightVS.specular * (spec * material . specular);

	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}