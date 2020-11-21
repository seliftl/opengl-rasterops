#version 430

in vec2 TexCoord;

layout(binding=0) uniform sampler2D BaseTex;
layout(binding=1) uniform sampler2D AlphaTex;

layout( location = 0 ) out vec4 FragColor;


void main() {
	vec4 alphaMap = texture( AlphaTex, TexCoord );

	FragColor = alphaMap;

}
