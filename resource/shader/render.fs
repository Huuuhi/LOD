#version 330 core


in vec3 Normal;  
in vec3 FragPos;  
in float fragScalarData;

uniform sampler2D textureMap;
uniform float minScalar;
uniform float maxScalar;

out vec4 FragColor;

void main()
{
	float normalScalar = (fragScalarData - minScalar) / (maxScalar - minScalar);
	vec4 color = texture(textureMap, vec2(normalScalar, 0.0));;
	FragColor = color;
} 