#ifndef VARIABLE_H
#define VARIABLE_H

#include "Singleton.h"
#include <GL/glew.h>
#include "NuanceurProg.h"

///////////////////////////////////////////////
// Énumérations globales                     //
///////////////////////////////////////////////
enum DrawnShape {
    SHAPE_NONE,
    SHAPE_TEAPOT,
    SHAPE_CUBE,
    SHAPE_SPHERE,
};

enum ShaderStyle {
    SHADER_NONE,
    SHADER_CLASSIC,
    SHADER_CEL_SHADING,
    SHADER_GOOCH_SHADING,
};

enum OutlineAlgorithm {
    OUTLINE_NONE,
    OUTLINE_HALO,
    OUTLINE_NORMALS,
    OUTLINE_BF_CULLING,
};


class Variable : public Singleton<Variable>
{
    SINGLETON_DECLARATION_CLASSE(Variable);
public:
    static DrawnShape drawnShape;
    static ShaderStyle shaderStyle;
    static OutlineAlgorithm outlineAlgorithm;

    // ID de la fenêtre graphique GLUT
    static int g_nWindowID;

    // Largeur et hauteur courantes de la fenêtre graphique (viewport)
    static int currentW;
    static int currentH;

    // Indique si on est en mode Perspective (true) ou glOrtho (false)
    static bool isPerspective;

    // Rotation automatique du modèle ou non
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

    // Indique si on desire activer la lumière Spot/Directionnelle/Ponctuelle
    static bool spotLightAllumee;
    static bool dirLightAllumee;
    static bool pointLightAllumee;

    // Noms de textures
    static GLuint texture0;
    static GLuint texture1;
    static GLuint texture2;

    // Nuanceurs de Sommets/Fragments
    static CNuanceurProg progNuanceurOpenGL;
    static CNuanceurProg progNuanceurCustom;
};

#endif //VARIABLE_H