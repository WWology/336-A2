#version 330 core

in vec3 aPosition;
in vec3 aNormal;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uVMatrix;
uniform Light uLight;
uniform Material uMaterial;

out vec3 vColour;

void main()
{
	gl_Position = uMVPMatrix * vec4(aPosition, 1.0f);

	vec3 EC_VertexPos = (uMVMatrix * vec4(aPosition, 1.0)).xyz;
	vec3 EC_Normal = (uMVMatrix * vec4(aNormal, 0.0)).xyz;

	vec3 N = normalize(EC_Normal);
	vec3 L = normalize((uVMatrix * vec4(uLight.position, 1.0)).xyz - EC_VertexPos);
	vec3 E = normalize(-EC_VertexPos);
	vec3 R = reflect(-L, N);

	vec3 ambient = uLight.ambient + uMaterial.ambient;
	vec3 diffuse = uLight.diffuse * uMaterial.diffuse * max(dot(L, N), 0.0);
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	if(dot(L, N) > 0.0f)
		specular = uLight.specular * uMaterial.specular * pow(max(dot(E, R), 0.0), uMaterial.shininess);


	vColour = diffuse + specular + ambient;
}