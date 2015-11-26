


uniform int pointLightOn;
uniform int spotLightOn;
uniform int dirLightOn;

varying vec3 vPointLight;
varying vec3 vDirLight;
varying vec3 vSpotLight;
varying vec3 ecPosition;
varying vec3 normal;

// Accumulateurs pour les facteurs d'éclairage (AMBIANT ET DIFFUS UNIQUEMENT)
// On POSE d'emblée que le matériau du gazon n'a PAS de composante spéculaire
vec4 Ambient;
vec4 Diffuse;
vec4 Specular;

// Calcul pour une lumière ponctuelle
void pointLight(in int i, in vec3 eye)
{
    float nDotVP;       // Produit scalaire entre VP et la normale
	float nDotHV;       // normal . light half vector
	float pf;           // power factor
    float attenuation;  // facteur d'atténuation calculé
    float d;            // distance entre lumière et fragment
    vec3  VP;           // Vecteur lumière
	vec3  halfVector;   // direction of maximum highlights

    // Calculer vecteur lumière
    VP = vPointLight - ecPosition;

    // Calculer distance à la lumière
    d = length(VP);

    // Normaliser VP
    VP = normalize(VP);

    // Calculer l'atténuation due à la distance
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
        gl_LightSource[i].linearAttenuation * d +
        gl_LightSource[i].quadraticAttenuation * d * d);

	halfVector = normalize(VP + eye);

	nDotVP = max(0.0, dot(normal, VP));
	nDotHV = max(0.0, dot(normal, halfVector));

	if (nDotVP == 0.0)
	{
		pf = 0.0;
	}
	else
	{
		pf = pow(nDotHV, gl_FrontMaterial.shininess);
	}

    // Calculer les contributions ambiantes diffuses et speculaires
    Ambient += gl_LightSource[i].ambient * attenuation;
    Diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
	Specular += gl_LightSource[i].specular * pf * attenuation;
}

// Calcul pour une lumière directionnelle
void directionalLight(in int i)
{
    float nDotVP;         // normal . light direction
    float nDotHV;         // normal . light half vector
    float pf;             // power factor

    nDotVP = max(0.0, dot(normal, normalize(vDirLight)));
	nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));

	if (nDotVP == 0.0)
	{
		pf = 0.0;
	}
	else
	{
		pf = pow(nDotHV, gl_FrontMaterial.shininess);
	}

    // Calculer les contributions ambiantes diffuses et speculaires
    Ambient += gl_LightSource[i].ambient;
    Diffuse += gl_LightSource[i].diffuse * nDotVP;
	Specular += gl_LightSource[i].specular * pf;
}

// Calcul pour une lumière "spot"
void spotLight(in int i, in vec3 eye)
{
	float nDotVP;            // normal . light direction
	float nDotHV;            // normal . light half vector
	float pf;                // power factor
	float spotDot;           // cosine of angle between spotlight
	float spotAttenuation;   // spotlight attenuation factor
	float attenuation;       // computed attenuation factor
	float d;                 // distance from surface to light source
	vec3  VP;                // direction from surface to light position
	vec3  halfVector;        // direction of maximum highlights

    // Calculer le vecteur Lumière
    VP = vSpotLight - ecPosition;

    // Calculer la distance à al lumière
    d = length(VP);

    // Normaliser VP
    VP = normalize(VP);

    // Calculer l'atténuation due à la distance
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
        gl_LightSource[i].linearAttenuation * d +
        gl_LightSource[i].quadraticAttenuation * d * d);

    // Le fragment est-il à l'intérieur du cône de lumière ?
    spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));

    if (spotDot < gl_LightSource[i].spotCosCutoff)
    {
        spotAttenuation = 0.0; // light adds no contribution
    }
    else
    {
        spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);
    }

    // Combine les atténuation du spot et de la distance
    attenuation *= spotAttenuation;
	halfVector = normalize(VP + eye);

    nDotVP = max(0.0, dot(normal, VP));
	nDotHV = max(0.0, dot(normal, halfVector));

	if (nDotVP == 0.0)
	{
		pf = 0.0;
	}
	else
	{
		pf = pow(nDotHV, gl_FrontMaterial.shininess);
	}

    // Calculer les contributions ambiantes diffuses et speculaires
    Ambient += gl_LightSource[i].ambient * attenuation;
    Diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
	Specular += gl_LightSource[i].specular * pf * attenuation;
}

vec4 flight()
{
    vec4 color;
	vec3 eye = vec3(0.0, 0.0, 1.0);

    // Réinitialiser les accumulateurs
    Ambient = vec4(0.0);
    Diffuse = vec4(0.0);
	Specular = vec4(0.0);

    if (pointLightOn == 1) {
        pointLight(0, eye);
    }

    if (dirLightOn == 1) {
        directionalLight(2);
    }

    if (spotLightOn == 1) {
        spotLight(1, eye);
    }

    color = gl_FrontLightModelProduct.sceneColor +
        Ambient * gl_FrontMaterial.ambient +
        Diffuse * gl_FrontMaterial.diffuse + 
		Specular  * gl_FrontMaterial.specular;
    color = clamp(color, 0.0, 1.0);
    return color;
}

// Render Style:
// 0 - No Render
// 1 - Normal Rendering
// 2 - Cel Shading
// 3 - Gooch Shading

uniform int renderStyle;
uniform sampler2D colorMap;
uniform bool affichageNormal;

// Cel Shading
float seuil = 4.0;
// Cel Shading

// Gooch Shading
vec3 goochWarmColor = vec3(1.0, 1.0, 0.0);
vec3 goochColdColor = vec3(0.0, 0.0, 1.0);
// Gooch Shading

vec4 renderColor(vec4 c, vec4 lightCoeff)
{
    vec4 color = c;
    color.a = 1.0;
    /*float intensity = dot(normalize(normal), normalize(lightCoeff.xyz));
    intensity = clamp(intensity, 0.0, 1.0);*/

    switch(renderStyle)
    {
    case 1: // 1 - Normal Rendering
        // Do nothing
        break;

    case 2: // Cel Shading
        // Color Based Algorithm
        color.r = (floor(color.r * seuil)) / seuil;
        color.g = (floor(color.g * seuil)) / seuil;
        color.b = (floor(color.b * seuil)) / seuil;

        // Light Intensity Based Algorithm
        /*if (intensity > 0.95)
        {
            color = vec4(1.0, 1.0, 1.0, 1.0);
        }
        else if (intensity > 0.5)
        {
            color = vec4(0.7, 0.7, 0.7, 1.0);
        }
        else if (intensity > 0.05)
        {
            color = vec4(0.35, 0.35, 0.35, 1.0);
        }
        else
        {
            color = vec4(0.1, 0.1, 0.1, 1.0);
        }*/

        break;

    case 3: // 3 - Gooch Shading
        // Color Based Algorithm
        color.r = (color.r * goochWarmColor.r) + ((1.0 - color.r) * goochColdColor.r);
        color.g = (color.g * goochWarmColor.g) + ((1.0 - color.g) * goochColdColor.g);
        color.b = (color.b * goochWarmColor.b) + ((1.0 - color.b) * goochColdColor.b);

        // Light Intensity Based Algorithm
        /*color.r *= (intensity * goochWarmColor.r) + ((1.0 - intensity) * goochColdColor.r);
        color.g *= (intensity * goochWarmColor.g) + ((1.0 - intensity) * goochColdColor.g);
        color.b *= (intensity * goochWarmColor.b) + ((1.0 - intensity) * goochColdColor.b);*/
        break;

        case 0: // 0 - No Render
        default:
        break;
    }
    color.a = 1.0;

    return color;
}

void main (void) 
{
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 baseColor = vec4(0.0, 0.0, 0.0, 0.0);

	//Affichage en mode texture 
	baseColor = texture2D(colorMap, gl_TexCoord[0].xy);

    vec4 lightCoeff = flight();

	//Affichage normal
    //color *= lightCoeff;

	//Affichage par texture
	color = baseColor + lightCoeff;
	color = clamp(color, 0.0, 1.0);

	if(affichageNormal)
	{
		gl_FragColor = gl_Color;
	}
	else
	{
		gl_FragColor = renderColor(color, lightCoeff);
	}
}
