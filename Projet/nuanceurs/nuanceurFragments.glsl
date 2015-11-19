

// Render Style:
// 0 - No Render
// 1 - Normal Rendering
// 2 - Cel Shading
// 3 - Gooch Shading

uniform int renderStyle;

vec3 goochWarmColor = vec3(1.0, 1.0, 0.0);
vec3 goochColdColor = vec3(0.0, 0.0, 1.0);

void main(void)
{
	vec4 color;
	float seuil = 5.0;

	color = gl_Color;

	switch(renderStyle)
	{
	case 1: // 1 - Normal Rendering
		// Do nothing
		break;
	case 2: // Cel Shading
		color.r = (floor(color.r * seuil)) / seuil;
		color.g = (floor(color.g * seuil)) / seuil;
		color.b = (floor(color.b * seuil)) / seuil;
		break;
	case 3: // 3 - Gooch Shading
		color.r = (color.r * goochWarmColor.r) + ((1 - color.r) * goochColdColor.r);
		//prototype meilleur
		//color.r = color.r * ((intensite * goochWarmColor.r) + ((1 - intensite) * goochColdColor.r));
		color.g = (color.g * goochWarmColor.g) + ((1 - color.g) * goochColdColor.g);
		color.b = (color.b * goochWarmColor.b) + ((1 - color.b) * goochColdColor.b);
		break;
	case 0: // 0 - No Render
	default:
		break;
	}

	gl_FragColor = color;
}
