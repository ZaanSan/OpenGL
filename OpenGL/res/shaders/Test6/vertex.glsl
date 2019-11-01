#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
  
out vec3 FragPos;
out vec3 LightPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Light calculation variables in view space
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	LightPos = vec3(view * vec4(lightPos, 1.0));
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
}