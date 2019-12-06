#version 330

uniform vec4		lightDiffuseColour;
uniform vec4		lightAmbientColour;

layout (location = 0) out vec4 fragColour;


void main(void)
{
	//Add the ambient and diffuse colours together
	vec4 lightColour = lightAmbientColour + lightDiffuseColour;

	//Output the light's colour
	fragColour = lightColour;
}
