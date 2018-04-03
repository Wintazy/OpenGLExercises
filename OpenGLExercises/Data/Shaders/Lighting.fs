#version 330

in vec3 NormalVec;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 sourcePos;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 ambientColor = light.ambient * material.ambient;

	vec3 fragNorm = normalize(NormalVec);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(fragNorm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, NormalVec);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = (material.specular * spec) * light.specular;

	FragColor = vec4((ambientColor + diffuse + specularColor), 1.0f);
}