#version 330

in vec3 NormalVec;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform sampler2D materialDiffuse;
uniform sampler2D materialSpecular;
uniform float materialShininess;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normVec, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normVec, vec3 fragPos, vec3 viewDir);

void main()
{	
	vec3 norm = normalize(NormalVec);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 resultColor = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		resultColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	FragColor = vec4(resultColor, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normVec, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normVec, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normVec);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

	vec3 ambient = light.ambient * vec3(texture(materialDiffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(materialDiffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(materialDiffuse, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normVec, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normVec, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normVec);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

	float distanceToLightSource = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distanceToLightSource + light.quadratic * distanceToLightSource * distanceToLightSource);

	vec3 ambient = light.ambient * vec3(texture(materialDiffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(materialDiffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(materialDiffuse, TexCoords));
	return (ambient + diffuse + specular) * attenuation;
}