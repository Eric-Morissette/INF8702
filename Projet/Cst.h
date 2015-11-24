#ifndef CONSTANT_H
#define CONSTANT_H

#include "Singleton.h"
#include <GL/glew.h>


///////////////////////////////////////////////
// declaration des macros et defines globaux //
///////////////////////////////////////////////

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif
#define DEG2RAD(a) ( a / 180.0f ) * ( M_PI );

#ifndef NULL
#define NULL 0
#endif


class Constant : public Singleton<Constant>
{
    SINGLETON_DECLARATION_CLASSE(Constant);
public:
    // constante de parametre speculaire
    static const GLfloat gl_separate_specular_color;

    // Frequences de rotation
    static const float freqRotX;
    static const float freqRotY;
    static const float freqRotZ;

    // le framerate cible de l'application graphique.
    static const float frameRate;

    // la grandeur du modèle
    static const float modelSize;

    // le facteur multiplicatif des axes unitaires dessines dans la scene
    static const float axeScaleFact;

    // Taille initiale de la fenetre GLUT
    static const int g_nWidth;
    static const int g_nHeight;
};


// Définition des lumières openGL
// Les lumieres sont et restent definies dans la PARTIE OPENGL, meme lorsque 
// l'on utilise les nuanceurs. Alors ici, pour obtenir l'effet désiré dans ShaderGen,
// on doit creer les mêmes trois (3) lumieres openGL que dans ShaderGen et les 
// positionner aux mêmes endroits.

// POINT LIGHT
static GLfloat light_position0[] = { 20.0f, 0.0f, 3.0f, 1.0f };
static GLfloat light_ambient0[]  = {  0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat light_diffuse0[]  = {  1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat light_specular0[] = {  0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat light_emission0[] = {  1.0f, 1.0f, 1.0f, 1.0f };

// SPOT LIGHT
static GLfloat light_position1[] = { 10.0f, 30.0f, 90.0f, 0.0f };
static GLfloat light_ambient1[]  = {  0.0f,  0.0f,  0.0f, 1.0f };
static GLfloat light_diffuse1[]  = {  0.6f,  0.6f,  0.6f, 1.0f };
static GLfloat light_specular1[] = {  0.7f,  0.7f,  0.7f, 1.0f };
static GLfloat light_emission1[] = {  1.0f,  1.0f,  1.0f, 1.0f };

// DIRECTIONNAL LIGHT
static GLfloat light_position2[] = { -20.0f, 0.0f, 50.0f, 0.0f };
static GLfloat light_ambient2[]  = {   0.0f, 0.0f,  0.0f, 1.0f };
static GLfloat light_diffuse2[]  = {   0.8f, 0.8f,  0.8f, 1.0f };
static GLfloat light_specular2[] = {   0.8f, 0.8f,  0.8f, 1.0f };
static GLfloat light_emission2[] = {   1.0f, 1.0f,  1.0f, 1.0f };

// définition du matériau utilise.
static GLfloat mat_ambiant_model[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat mat_diffuse_model[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat mat_specular_model[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat mat_emission_model[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat mat_shininess_model[] = { 100.0f };


// Structure qui represente une image RGB
struct SImage
{
    GLuint    width;
    GLuint    height;
    GLubyte*  data;

    SImage(int Width, int Height)
        : width(Width)
        , height(Height)
        , data(NULL)
    {
        data = new GLubyte[width * height * 3];
    }

    ~SImage(void)
    {
        if (data)
            delete[] data;
    }
};

#endif //CONSTANT_H