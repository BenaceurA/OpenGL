#version 330 core

in vec2 Tex;
uniform sampler2D screenTexture;
out vec4 fragColor;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * 0.1 * 5000.0) / (5000.0 + 0.1f - z * (5000.0 - 0.1));	
}

void main(){

	fragColor = texture(screenTexture, Tex);
//	float depthValue = texture(screenTexture, Tex).r;
//	fragColor = vec4(vec3(LinearizeDepth(depthValue) / 5000.0), 1.0); // perspective
}