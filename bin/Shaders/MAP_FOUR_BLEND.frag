#version 120

varying vec2 UV0;
varying vec2 UV1;

uniform sampler2D Z_TEX0;
uniform sampler2D Z_TEX1;
uniform sampler2D Z_TEX2;
uniform sampler2D Z_TEX3;
uniform sampler2D Z_TEX4;

void main()
{
	vec3 color0;
	vec4 blend = texture2D(Z_TEX0,UV1);

	vec4 color1 = texture2D(Z_TEX1, UV0);
	color0 = color1.xyz;

	vec4 color2 = texture2D(Z_TEX2, UV0);
	color0 = blend.r * color2.xyz + (1.0f - blend.r) * color0.xyz;

	vec4 color3 = texture2D(Z_TEX3, UV0);
	color0 = blend.g * color3.xyz + (1.0f - blend.g) * color0.xyz;
	
	vec4 color4 = texture2D(Z_TEX4, UV0);
	color0 = blend.b * color4.xyz + (1.0f - blend.b) * color0.xyz;
	
	gl_FragColor = vec4(color0, 1.0f);
}
