#version 330 core

struct Material {
    sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_reflection1;
    float shininess;
}; 

struct DirLight {
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    float constant;
    float linear;
    float quadratic;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

out vec4 FragColor; 

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 cameraPos;
uniform samplerCube skybox;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 Position, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 Position, vec3 viewDir);
vec3 CalcReflection(vec3 normal, vec3 Position, vec3 viewDir);


// Phong lighting model
void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPos - Position);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, Position, viewDir);
	
	result += CalcSpotLight(spotLight, norm, Position, viewDir);

	vec3 reflection = CalcReflection(norm, Position, cameraPos);
	
	result += reflection;
	//result = mix(result, reflection, 0.5);

    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 Position, vec3 viewDir)
{
	if (light.linear == 0 || light.quadratic == 0)
		return vec3(0.0f);
	
	vec3 lightDir = normalize(light.position - Position);

	float diff = max(dot(normal, lightDir), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position = Position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

	vec3 ambient = attenuation * light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
	vec3 diffuse = attenuation * light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
	vec3 specular = attenuation * light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 Position, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - Position);

	vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	if (theta > light.outerCutOff)
	{
		// Diffuse
		float diff = max(dot(normal, lightDir), 0.0);

		// Specular
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		vec3 diffuse = intensity * light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;
		vec3 specular = intensity * light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

		return (ambient + diffuse + specular);
	}
	return (ambient);
}

vec3 CalcReflection(vec3 normal, vec3 Position, vec3 viewDir)
{
	vec3 I = normalize(Position - viewDir);
	vec3 R = reflect(I, normalize(normal));
	
	vec3 skyboxSample = texture(skybox, R).rgb;
	vec3 refrectionMult = texture(material.texture_reflection1, TexCoords).rgb;

	return skyboxSample * refrectionMult;
}
