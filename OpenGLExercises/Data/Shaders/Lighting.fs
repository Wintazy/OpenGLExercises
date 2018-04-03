#version 330

in vec3 NormalVec;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D materialDiffuse;
uniform sampler2D materialSpecular;
uniform float materialShininess;

struct Light
{
	vec3 sourcePos;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	vec3 ambientColor = light.ambient * vec3(texture(materialDiffuse, TexCoords));

	vec3 fragNorm = normalize(NormalVec);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(fragNorm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(materialDiffuse, TexCoords));
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, NormalVec);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
	vec3 specularColor = light.specular * spec * vec3(texture(materialSpecular, TexCoords));

	FragColor = vec4(ambientColor + diffuse + specularColor, 1.0f);
}