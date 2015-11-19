#include "Var.h"
#include "Cst.h"

SINGLETON_DECLARATION_CPP(Variable);

DrawnShape          Variable::drawnShape        = SHAPE_TEAPOT;
ShaderStyle         Variable::shaderStyle       = SHADER_NONE;
OutlineAlgorithm    Variable::outlineAlgorithm  = OUTLINE_NONE;

int Variable::g_nWindowID = 0;

int Variable::currentW = 0;
int Variable::currentH = 0;

bool Variable::isPerspective = true;

bool Variable::rotAutoModele = false;

double Variable::theta  = DEG2RAD(270.0f);
double Variable::phi    = DEG2RAD(90.0f);
double Variable::rho    = 30.0;

float Variable::x = 0.0f;
float Variable::y = 0.0f;
float Variable::z = 0.0f;

float Variable::angleRotX = 0.0f;
float Variable::angleRotY = 0.0f;
float Variable::angleRotZ = 0.0f;

bool Variable::premierMouvement = true;

int Variable::dernierX = 0;
int Variable::dernierY = 0;

bool Variable::spotLightAllumee     = true;
bool Variable::dirLightAllumee      = true;
bool Variable::pointLightAllumee    = true;

GLuint Variable::texture0 = 0;
GLuint Variable::texture1 = 0;
GLuint Variable::texture2 = 0;

CNuanceurProg Variable::progNuanceurOpenGL;
CNuanceurProg Variable::progNuanceurCustom("nuanceurs/nuanceurSommets.glsl", "nuanceurs/nuanceurFragments.glsl", false);