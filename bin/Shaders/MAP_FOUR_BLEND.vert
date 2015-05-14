#version 120

attribute vec3 Z_POSITION;
attribute vec2 Z_UV0;
attribute vec2 Z_UV1;

uniform mat4 Z_MODEL_VIEW_PROJECTION;

varying vec2 UV0;
varying vec2 UV1;

void main()
{
	gl_Position = Z_MODEL_VIEW_PROJECTION * vec4(Z_POSITION,1);
	UV0 = Z_UV0;
	UV1 = Z_UV1;
}