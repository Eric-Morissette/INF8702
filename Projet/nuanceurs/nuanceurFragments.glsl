


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

// Calcul pour une lumière ponctuelle
void pointLight(in int i)
{
    float nDotVP;       // Produit scalaire entre VP et la normale
    float attenuation;  // facteur d'atténuation calculé
    float d;            // distance entre lumière et fragment
    vec3  VP;           // Vecteur lumière

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

    nDotVP = max(0.0, dot(normal, VP));

    // Calculer les contributions ambiantes et diffuses
    Ambient += gl_LightSource[i].ambient * attenuation;
    Diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
}

// Calcul pour une lumière directionnelle
void directionalLight(in int i)
{
    float nDotVP;         // normal . light direction
    float nDotHV;         // normal . light half vector
    float pf;             // power factor

    nDotVP = max(0.0, dot(normal, normalize(vDirLight)));

    // Calculer les contributions ambiantes et diffuses
    Ambient += gl_LightSource[i].ambient;
    Diffuse += gl_LightSource[i].diffuse * nDotVP;
}

// Calcul pour une lumière "spot"
void spotLight(in int i)
{
    float nDotVP;            // Produit scalaire entre VP et la normale
    float spotDot;           // Cosinus angle entre VP et direction spot
    float spotAttenuation;   // Facteur d'atténuation du spot
    float attenuation;       // Facteur d'atténuation du à la distance
    float d;                 // Distance à la lumière
    vec3  VP;                // Vecteur lumière

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

    nDotVP = max(0.0, dot(normal, VP));

    // Calculer les contributions ambiantes et diffuses
    Ambient += 3.0 * gl_LightSource[i].ambient * attenuation;
    Diffuse += 3.0 * gl_LightSource[i].diffuse * nDotVP * attenuation;
}

vec4 flight()
{
    vec4 color;

    // Réinitialiser les accumulateurs
    Ambient = vec4(0.0);
    Diffuse = vec4(0.0);

    if (pointLightOn == 1) {
        pointLight(0);
    }

    if (dirLightOn == 1) {
        directionalLight(1);
    }

    if (spotLightOn == 1) {
        spotLight(2);
    }

    color = gl_FrontLightModelProduct.sceneColor +
        Ambient * gl_FrontMaterial.ambient +
        Diffuse * gl_FrontMaterial.diffuse;
    color = clamp(color, 0.0, 1.0);
    return color;
}

// Render Style:
// 0 - No Render
// 1 - Normal Rendering
// 2 - Cel Shading
// 3 - Gooch Shading

uniform int renderStyle;

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
    vec4 lightCoeff = flight();

    color *= lightCoeff;
    gl_FragColor = renderColor(color, lightCoeff);
}
