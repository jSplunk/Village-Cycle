#version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;


void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(image, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if(bloom)
        hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}