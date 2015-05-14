#version 120

attribute vec2 Z_UV0;
attribute vec3 Z_POSITION;

uniform mat4 Z_MODEL_VIEW_PROJECTION;

varying vec2 UV;

void main()
{
	gl_Position = Z_MODEL_VIEW_PROJECTION * vec4(Z_POSITION,1);
	UV = Z_UV0;
}