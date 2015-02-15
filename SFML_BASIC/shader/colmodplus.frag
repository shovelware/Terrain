#version 120
//Modulates colour by y height of vertex

varying float vertheight;
uniform float maxheight;

void main()
{
	float h = (vertheight / maxheight) * (100 / 1); //h is vertheight as a percentage of maxheight

	gl_FragColor = gl_Color;

	vec4 sno = vec4(1, 1, 1, 1);
	vec4 rck = vec4(0.392, 0.196, 0, 1);
	vec4 grs = vec4(0, 0.5, 0, 1);
	vec4 snd = vec4(1, 0.785, 0, 1);
	vec4 wtr = vec4(0, 0.125, 0.37, 1);

	//90 -> 100 : Snow
	if (h > 90)
	{
		gl_FragColor = sno;
	}

	//75 -> 90 : rocks -> snow
	else if (h > 75)
	{
		gl_FragColor = rck;
	}

	//60 -> 75 : grass -> rocks
	else if (h > 60)
	{
		float i = ((h - 60) / 15) * (1000 / 1);
		gl_FragColor = grs + rck; //This breaks stuff
	}

	//50 -> 60 : grass
	else if (h > 50)
	{
		gl_FragColor = grs;
	}

	//25 -> 50 : sand -> grass
	else if (h > 25)
	{
		float i = ((h - 25) / 25) * (100 / 1);
		gl_FragColor = (snd * (i)) + (grs * (1 - i)); // Something like this?
	}

	//10 -> 25 : sand
	else if (h > 10)
	{
		gl_FragColor = snd;
	}

	//0 -> 10 : water
	else
	{
		gl_FragColor = wtr;
	}

}