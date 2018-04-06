#version 330
in vec2 TexCoords;
in vec3 FragPos;
in vec3 NormVec;

out vec4 FragColor;

uniform sampler2D material_texture_diffuse1;
uniform sampler2D material_texture_specular1;
uniform vec3 viewPos;

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

void main()
{
	// ambient
    vec3 ambient = light.ambient * texture(material_texture_diffuse1, TexCoords).rgb;

	// diffuse 
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 norm = normalize(-lightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material_texture_diffuse1, TexCoords).rgb;

	// specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),1.0);
    vec3 specular = light.specular * spec * texture(material_texture_specular1, TexCoords).rgb;

	// attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient  *= attenuation;  
    //diffuse   *= attenuation;
    //specular *= attenuation;  
	
	vec3 result = ambient + specular;
    FragColor = vec4(result, 1.0);
}