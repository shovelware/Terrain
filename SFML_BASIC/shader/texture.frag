#version 120
//Texturing and modulating by y height

varying float vertheight;
uniform float maxheight;

varying vec2 texv;

uniform sampler2D tex1;//water
uniform sampler2D tex2;//grass
uniform sampler2D tex3;//snowy-rocks

void main()
{
	vec4 tex_map = texture2D( tex1, gl_TexCoord[0].st);//water
	vec4 tex_map1 = texture2D( tex2, gl_TexCoord[0].st);//grass
	vec4 tex_map2 = texture2D( tex3, gl_TexCoord[0].st);//snowy rocks


	//modulate the colour based on vertex height
	float h = (vertheight / maxheight) * (100 / 1); //h is vertheight as a percentage of maxheight

	if(h < 10)
		gl_FragColor = tex_map;
	else if(h<60)
		gl_FragColor = tex_map1;
	else gl_FragColor = tex_map2;
	






	/* PROBABLY NEED THIS FOR BLENDING SO IT CAN STAY HERE

	gl_FragColor = gl_Color;
	
	vec4 red = vec4(1, 0, 0, 1);
	vec4 orn = vec4(1, 0.5, 0, 1);
	vec4 ylo = vec4(1, 1, 0, 1);
	vec4 grn = vec4(0, 1, 0, 1);
	vec4 blu = vec4(0, 0, 1, 1);
	vec4 prp = vec4(0.5, 0, 0.5, 1);
	
	if (h > 80)
	{
		gl_FragColor = red;
	}
	
	else if (h > 60)
	{
		gl_FragColor = orn;
	}
	
	else if (h > 40)
	{
		gl_FragColor = ylo;
	}
	
	else if (h > 25)
	{
		gl_FragColor = grn;
	}
	
	else if (h > 10)
	{
		gl_FragColor = blu;
	}
	
	else
	{
		gl_FragColor = prp;
	}
	*/

}