#version 330

in vec3 NormalVec;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambientColor = lightColor * ambientStrength;

	vec3 fragNorm = normalize(NormalVec);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(fragNorm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, NormalVec);
	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specularColor = specularStrength * spec * lightColor;

	FragColor = vec4(objectColor * (ambientColor + diffuse + specularColor), 1.0f);
}