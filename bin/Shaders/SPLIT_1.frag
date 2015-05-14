#version 120

varying vec2 UV;

uniform sampler2D Z_TEX0;
uniform sampler2D Z_TEX1;
uniform int MODE;

void main()
{
	if(MODE == 0) 
	{
		gl_FragColor = texture2D(Z_TEX0,UV);
	}
	else if(MODE == 1) 
	{
		gl_FragColor = texture2D(Z_TEX1,UV);
	} 
	else if(MODE == 2) 
	{
		if(UV.x <= 0.5) {
			gl_FragColor = texture2D(Z_TEX0,UV);	
		} else {
			gl_FragColor = texture2D(Z_TEX1,UV);
		}
	} 
	else if(MODE == 3)
	{
		if(UV.x <= 0.5) {
			gl_FragColor = texture2D(Z_TEX1,UV);	
		} else {
			gl_FragColor = texture2D(Z_TEX0,UV);
		}
	}
	else if(MODE == 4)
	{
		if(UV.x <= 0.5) {
			gl_FragColor = texture2D(Z_TEX0,vec2(UV.x+0.25,UV.y));	
		} else {
			gl_FragColor = texture2D(Z_TEX1,vec2(UV.x-0.25,UV.y));
		}
	}
}