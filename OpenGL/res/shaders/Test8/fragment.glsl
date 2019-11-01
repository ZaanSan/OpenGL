#version 330 core

struct Material {
    sampler2D diffuse; // Ambient and diffuse are almost always the same
    sampler2D specular;
	sampler2D emission;
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
in vec2 TexCoords;

uniform Material material;

// Phong lighting model
void main()
{
	// Ambiant
	vec3 ambient = LightVS.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightVS.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = LightVS.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// Specular
	vec3 viewDir = normalize(-FragPos); // Because in view space viewPos : 0,0,0
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = LightVS.specular * spec * texture(material.specular, TexCoords).rgb;

	vec3 emission = texture(material.emission, TexCoords).rgb;

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0f);
}