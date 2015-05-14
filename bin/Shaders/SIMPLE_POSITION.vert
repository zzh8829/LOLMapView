#version 120

attribute vec3 Z_POSITION;

uniform mat4 Z_MODEL_VIEW_PROJECTION;

void main()
{
	gl_Position = Z_MODEL_VIEW_PROJECTION * vec4(Z_POSITION,1);
}