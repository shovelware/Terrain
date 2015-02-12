#version 120

varying float vertheight;
uniform float maxheight;

uniform bool normals;

varying vec2 texv;

void main()
{
	if (!normals)
	{
		float h = (vertheight / maxheight) * (100 / 1); //h is vertheight as a percentage of maxheight
		
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

		gl_FragColor = vec4(texv, 0, 1);
	}

}