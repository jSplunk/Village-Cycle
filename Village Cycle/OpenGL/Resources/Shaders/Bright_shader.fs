#version 330
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec4		lightDiffuseColour;
uniform vec4		lightAmbientColour;

uniform vec3        cameraPos; // to calculate specular lighting in world coordinate space, we need the location of the camera since the specular light

in vec4 posWorldCoord;
in vec4 colour;
in vec3 normalWorldCoord;
in vec2 texCoord;
in vec3 normal; 

void main()
{  

    vec3 diffuseColour = lightDiffuseColour.rgb; // input colour actually diffuse colour

    vec3 rgbColour = (lightAmbientColour.rgb) + diffuseColour;
    FragColor = vec4(rgbColour, 1.0);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}