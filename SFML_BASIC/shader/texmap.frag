#version 120
//Colour based on texture map

uniform float maxheight;
varying vec2 texv;

void main()
{
	float h = maxheight;

	//Set colour rgb to (u, v, 0) from texture coord
	gl_FragColor = vec4(texv, 0, 1);
}