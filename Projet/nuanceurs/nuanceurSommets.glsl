
uniform bool affichageNormal;
uniform bool utiliserTextures;

uniform int pointLightOn;
uniform int spotLightOn;
uniform int dirLightOn;

varying vec3 vPointLight;
varying vec3 vDirLight;
varying vec3 vSpotLight;
varying vec3 ecPosition;
varying vec3 normal;

float decalage = 0.3;

void main(void)
{
    // Position in Visualisation Coords.
    ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);

    // Compute Lights
    vPointLight = vec3(gl_LightSource[0].position);
    vDirLight = vec3(gl_LightSource[1].position);
    vSpotLight = vec3(gl_LightSource[2].position);

    // Apply transformations
    gl_Position = ftransform();
    normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_TexCoord[0] = gl_MultiTexCoord0;

    if(affichageNormal)
    {
        // Try and reduce the amount of faces that have to be drawn
        if (dot(normal, normalize(ecPosition)) >= -0.5)
        {
            gl_Position += vec4(decalage * normal,0);
        }
    }
}