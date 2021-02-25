#version 330 core

in vec3 aPosition;
in vec3 aColour;

uniform mat4 uModelMatrix;

out vec3 vColour;

void main()
{
	gl_Position = uModelMatrix * vec4(aPosition, 1.0f);

	vColour = aColour;
}