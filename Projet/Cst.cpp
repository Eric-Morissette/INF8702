#include "Cst.h"

SINGLETON_DECLARATION_CPP(Constant);

// constante de parametre speculaire
const GLfloat Constant::gl_separate_specular_color = GL_SEPARATE_SPECULAR_COLOR;

// Frequences de rotation
const float Constant::freqRotX = 0.15f;
const float Constant::freqRotY = 0.10f;
const float Constant::freqRotZ = 0.20f;

// le framerate cible de l'application graphique.
const float Constant::frameRate = 50.0f;

// la grandeur du modèle
const float Constant::modelSize = 15.0f;

// le facteur multiplicatif des axes unitaires dessinés dans la scène
const float Constant::axeScaleFact = 15.0f;

// Taille initiale de la fenetre GLUT
const int Constant::g_nWidth = 640;
const int Constant::g_nHeight = 480;