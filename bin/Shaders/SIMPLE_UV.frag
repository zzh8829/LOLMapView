#version 120

varying vec2 UV;

uniform sampler2D Z_TEX0;

void main()
{
	gl_FragColor = texture2D(Z_TEX0,UV);
}