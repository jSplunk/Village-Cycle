#version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS hdrBuffer;
uniform bool hdr;
uniform bool greyScale;
uniform float exposure;
uniform int vpWidth;
uniform int vpHeight;


void main()
{             
     const float gamma = 2.2;
	 
	 //Setting the view point coordinates as integers
	 ivec2 vpCoords = ivec2(vpWidth, vpHeight);
	 vpCoords.x = int(vpCoords.x * TexCoords.x);
	 vpCoords.y = int(vpCoords.y * TexCoords.y);
	 
	 //Samples the texture with 4 samples
	 vec4 samples;
	 for(int i = 0; i < 4; i++)
	 {
		vec4 sample = texelFetch(hdrBuffer, vpCoords, i);
		samples += sample;
	 }
	 vec3 hdrColor = (samples/ 4.0f).rgb;

	//Are we using HDR?
    if(hdr)
    {
		//Are we using greyscale?
		if(greyScale)
		{
			//Grey scale conversion
			float grayscale = 0.2126 * hdrColor.r + 0.7152 * hdrColor.g + 0.0722 * hdrColor.b;

			FragColor = vec4(vec3(grayscale), 1.0);
		}
		else
		{
			// exposure
			vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
			// also gamma correct while we're at it       
			result = pow(result, vec3(1.0 / gamma));

			FragColor = vec4(result, 1.0);
		}
    }
    else
    {
		//Are we using grey scale?
		if(greyScale)
		{
		    //Grey scale conversion
			float grayscale = 0.2126 * hdrColor.r + 0.7152 * hdrColor.g + 0.0722 * hdrColor.b;

			FragColor = vec4(vec3(grayscale), 1.0);
		}
		else
		{
			//Setting gamma
			vec3 result = pow(hdrColor, vec3(1.0 / gamma));
			FragColor = vec4(result, 1.0);
		}
    }

	
}