#version 330 core

struct Material
{
	sampler2D diffuseTex;
	sampler2D specularTex;
	float shininess;
};

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fFragPos;
in vec2 fTexCoord;
in vec3 fNormal;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform float dissolve;

uniform vec3 cameraPos;

void main()
{
	vec3 difTexRGB = texture(material.diffuseTex, fTexCoord).rgb;

	// アンビエント
	vec3 ambientColor = light.ambient * difTexRGB;

	// ディフューズ
	vec3 norm = normalize(fNormal);
	vec3 frag2Light = normalize(-light.direction);

	float diff = max(dot(norm, frag2Light), 0.0);
	vec3 diffuseColor = light.diffuse * diff * difTexRGB;

	// スペキュラー
	vec3 frag2Cam = normalize(cameraPos - fFragPos);
	vec3 reflectDir = reflect(light.direction, norm);

	float spec = pow(max(dot(frag2Cam, reflectDir), 0.0), material.shininess);
	vec3 specularColor = light.specular * spec * vec3(texture(material.specularTex, fTexCoord));

	vec3 result = ambientColor + diffuseColor + specularColor;
	FragColor = vec4(result, dissolve);
}
