#version 120
//Modulates colour by y height of vertex

varying float vertheight;
uniform float maxheight;

void main()
{
	float h = (vertheight / maxheight) * (100 / 1); //h is vertheight as a percentage of maxheight

	gl_FragColor = gl_Color;

	vec4 red = vec4(1, 0, 0, 1);
	vec4 orn = vec4(1, 0.5, 0, 1);
	vec4 ylo = vec4(1, 1, 0, 1);
	vec4 grn = vec4(0, 1, 0, 1);
	vec4 blu = vec4(0, 0, 1, 1);
	vec4 prp = vec4(0.5, 0, 0.5, 1);

	//80 -> 100
	if (h > 80)
	{
		gl_FragColor = red;
	}

	//60 -> 80
	else if (h > 60)
	{
		gl_FragColor = orn;
	}

	//40 -> 60
	else if (h > 40)
	{
		gl_FragColor = ylo;
	}

	//25 -> 40
	else if (h > 25)
	{
		gl_FragColor = grn;
	}

	//10 -> 25
	else if (h > 10)
	{
		gl_FragColor = blu;
	}

	//0 -> 10
	else
	{
		gl_FragColor = prp;
	}
}