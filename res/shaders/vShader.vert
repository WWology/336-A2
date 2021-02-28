#version 330 core

in vec3 aPosition;
in vec3 aColour;
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

uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix;
uniform mat4 uViewMatrix;
uniform Light uLight;
uniform Material uMaterial;

out vec3 vColour;

void main()
{
	gl_Position = uModelMatrix * vec4(aPosition, 1.0f);

	vColour = aColour;
}