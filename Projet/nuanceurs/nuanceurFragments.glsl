

// Render Style:
// 0 - No Render
// 1 - Normal Rendering
// 2 - Cel Shading
// 3 - Gooch Shading

uniform int renderStyle;

// Cel Shading
float seuil = 10.0;
// Cel Shading

// Gooch Shading
vec3 goochWarmColor = vec3(1.0, 1.0, 0.0);
vec3 goochColdColor = vec3(0.0, 0.0, 1.0);
// Gooch Shading

void main(void)
{
	vec4 color;

	color = gl_Color;

	switch(renderStyle)
	{
	case 1: // 1 - Normal Rendering
		// Do nothing
		break;
	case 2: // Cel Shading
        // prototype meilleur
        // se baser sur l'intensite, exemple: http://rbwhitaker.wikidot.com/toon-shader
		color.r = (floor(color.r * seuil)) / seuil;
		color.g = (floor(color.g * seuil)) / seuil;
		color.b = (floor(color.b * seuil)) / seuil;
		break;
	case 3: // 3 - Gooch Shading
		color.r = (color.r * goochWarmColor.r) + ((1.0 - color.r) * goochColdColor.r);
		// prototype meilleur
		// color.r = color.r * ((intensite * goochWarmColor.r) + ((1 - intensite) * goochColdColor.r));
		color.g = (color.g * goochWarmColor.g) + ((1.0 - color.g) * goochColdColor.g);
		color.b = (color.b * goochWarmColor.b) + ((1.0 - color.b) * goochColdColor.b);
		break;
	case 0: // 0 - No Render
	default:
		break;
	}

	gl_FragColor = color;
}
