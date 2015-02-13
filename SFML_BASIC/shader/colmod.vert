#version 120
//Modulates colour by y height of vertex

varying float vertheight;
uniform float maxheight;

void main()
{
	vertheight = gl_Vertex[1]; //Pull y coord for fragment shader

	gl_FrontColor = gl_Color; //pass the colour along to the next phase (fragment shader)

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

}