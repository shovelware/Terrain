#version 120

varying float vertheight;
uniform float maxheight;

uniform bool normals;

//Need access to every vertex as it goes through
//Can't unless I get access to shader in Terrain.cpp
//But that doesn't work fuuuu
varying vec2 texv;

void main()
{
	vertheight = gl_Vertex[1]; //Pull y coord for fragment shader

	gl_FrontColor = gl_Color; //pass the colour along to the next phase (fragment shader)

	//Pass texture along
	texv = vec2(gl_MultiTexCoord0[0], gl_MultiTexCoord0[1]);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;//Transformation
}

/*

C++ = GL

glColor3fv() = gl_Color;

glNormal3fv() = gl_Normal;

glTexCoord2fv() = gl_MultiTexCoord0; //gl_MultiTexCoord always has 4 values so use [0] and [1] as u and v

glVertex3fv() = gl_Vertex;

*/