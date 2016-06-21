#ifndef VARIABLE_H
#define VARIABLE_H

#include <GL/glew.h>
#include "NuanceurProg.h"
#include "Cst.h"

class Variable
{
public:
    static DrawnShape drawnShape;
    static ShaderStyle shaderStyle;
    static OutlineAlgorithm outlineAlgorithm;

    // ID de la fen�tre graphique GLUT
    static int g_nWindowID;

    // Largeur et hauteur courantes de la fen�tre graphique (viewport)
    static int currentW;
    static int currentH;

    // Indique si on est en mode Perspective (true) ou glOrtho (false)
    static bool isPerspective;

    // Rotation automatique du mod�le ou non
    static bool rotAutoModele;

    // Angle de rotation de la camera (coordonnees spheriques)
    static double theta;
    static double phi;
    static double rho;

    // Position de la camera
    static float x;
    static float y;
    static float z;

    // Angle courant de rotation du modele
    static float angleRotX;
    static float angleRotY;
    static float angleRotZ;

    // Indique si la souris a deja ete bougee
    static bool premierMouvement;

    // Dernieres valeurs de XY de la souris
    static int dernierX;
    static int dernierY;

    // Indique si on desire activer la lumi�re Spot/Directionnelle/Ponctuelle
    static bool spotLightAllumee;
    static bool dirLightAllumee;
    static bool pointLightAllumee;

    // Textures
    static bool utiliserTextures;
    static GLuint texture0;
    static GLuint texture1;
    static GLuint texture2;

    // Nuanceurs de Sommets/Fragments
    static CNuanceurProg progNuanceurOpenGL;
    static CNuanceurProg progNuanceurCustom;

    static int sceneRenderStyle;
    static int objectRenderStyle;
};

#endif //VARIABLE_H