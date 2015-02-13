#version 120
//Texturing and modulating by y height

varying float vertheight;
uniform float maxheight;

varying vec2 texv;

void main()
{
	vertheight = gl_Vertex[1]; //Pull y coord for fragment shader

	gl_FrontColor = gl_Color;
	texv = vec2(gl_MultiTexCoord0[0], gl_MultiTexCoord0[1]);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}