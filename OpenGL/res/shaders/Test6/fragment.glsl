#version 330 core
out vec4 FragColor; 

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform int shininess;

// Phong lighting model
void main()
{
	// Ambiant
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);

	float diffuseImpact = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseImpact * lightColor;

	// Specular
	vec3 viewDir = normalize(-FragPos); // Because in view space viewPos : 0,0,0
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}