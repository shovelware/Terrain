#version 120
//Texturing and modulating by y height

varying float vertheight;
uniform float maxheight;

varying vec2 texv;

uniform sampler2D tex1;//water
uniform sampler2D tex2;//grass
uniform sampler2D tex3;//snowy-rocks
uniform sampler2D tex4;//rocks
uniform sampler2D tex5;//sand

float norm(float start, float value, float end)
{
	return (value - start) / (end - start);
}

void main()
{
	vec4 tex_map = texture2D( tex1, texv.st);//water
	vec4 tex_map1 = texture2D( tex2, texv.st);//grass
	vec4 tex_map2 = texture2D( tex3, texv.st);//snowy rocks
	//vec4 snd = vec4(1, 0.785, 0, 1);
	vec4 tex_map3 = texture2D(tex4, texv.st);//rocks
	vec4 tex_map4 = texture2D(tex5,texv.st);//sand
	//vec4 rck = vec4(0.392, 0.196, 0, 1);


	//modulate the colour based on vertex height
	float h = (vertheight / maxheight) * (100 / 1); //h is vertheight as a percentage of maxheight

	/*if(h < 10)
		gl_FragColor = tex_map;
	else if(h<60)
		gl_FragColor = tex_map1;
	else gl_FragColor = tex_map2;*/
	

		//90 -> 100 : Snow
	if (h > 90)
	{
		gl_FragColor = tex_map2;
	}

	//75 -> 90 : rocks	//-> snow
	else if (h > 75)
	{
			//if(h<83)
			gl_FragColor = tex_map3;
			//else gl_FragColor = (rck*0.7) + (sno*0.3);
	}

	//60 -> 75 : grass -> rocks
	else if (h > 60)
	{
		float i = norm(60, h, 75);
		gl_FragColor = mix(tex_map1, tex_map3, i);
			//if(h<70)
			//	gl_FragColor = grs;
			//else gl_FragColor = (grs*0.5)+(rck*0.7);
		//float i = ((h - 60) / 15) * (1000 / 1);
		//gl_FragColor = grs + rck; //This breaks stuff
	}

	//50 -> 60 : grass
	else if (h > 50)
	{
		gl_FragColor = tex_map1;
	}

	//25 -> 50 : sand -> grass
	else if (h > 25)
	{
		float i = norm(25, h, 50);
		gl_FragColor = mix(tex_map4, tex_map1, i);

		//float i = ((h - 25) / 25) * (100 / 1);
		//gl_FragColor = (snd * (i)) + (grs * (1 - i)); // Something like this?
			//if(h <30)
			//	gl_FragColor = gl_FragColor = snd;
			//else gl_FragColor = (snd*0.5) + (grs*0.9);
	}

	//10 -> 25 : sand
	else if (h > 10)
	{
		gl_FragColor = tex_map4;
	}

	//0 -> 10 : water
	else
	{
		gl_FragColor = tex_map;
	}



	

}