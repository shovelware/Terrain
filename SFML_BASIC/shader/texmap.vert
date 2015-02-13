#version 120
//Colour based on texture map

uniform float maxheight;
varying vec2 texv;

void main()
{
	gl_FrontColor = gl_Color;

	texv = vec2(gl_MultiTexCoord0[0], gl_MultiTexCoord0[1]);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}