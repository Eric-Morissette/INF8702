
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "textfile.h"
#include "Var.h"
#include "Cst.h"
#include "NuanceurProg.h"
#include <iostream>

// PROTOTYPES DES FONCTIONS
void initialisation (void);
void dessinerObjet(DrawnShape shape, double size);
void dessinerOutline(DrawnShape shape, double size);
void dessinerObjetEtOutline(DrawnShape shape, double size);
void dessinerScene();
void affichage(void);
void attente(void);
void clavier(unsigned char touche, int x, int y);
void clavierSpecial(int touche, int x, int y);
void redimensionnement(GLsizei w, GLsizei h);

void rafraichirCamera();
void compileGrilleQuads(int listID, float size, int nbRows, int nbColums, bool outsideNormal);
void compileCube(int listID, float size, int nbRows, int nbColums, bool outsideNormal);
void dessinerGrilleQuads(float size, int nbRows, int nbColums, bool outsideNormal);
void rafraichirEclairage();
void rafraichirProjection();
void afficherShaderInfoLog(GLuint obj);
void afficherProgramInfoLog(GLuint obj);
void appliquerTextures();
SImage* LoadBMP(char* Filename);
void init2DTexture(GLint texName, GLint texWidth, GLint texHeight, GLubyte *texPtr);
void chargerTextures();
void desactiverTextures();


// initialisation d'openGL
void initialisation (void) {
    // modèle de rendu
    glShadeModel(GL_SMOOTH);	

    // couleur de l'arrière-plan
    glClearColor (1.0, 1.0, 1.0, 1.0);

    // activer les etats openGL
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);

    // fonction de profondeur
    glDepthFunc(GL_LEQUAL);

    // amélioration de la perspective
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );  // two-side mode in openGL
    glEnable( GL_VERTEX_PROGRAM_TWO_SIDE );             // two-side mode in GLSL
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // charger les textures
    chargerTextures();
}

void dessinerObjet(DrawnShape shape, double size)
{
    switch (shape)
    {
    case DrawnShape::SHAPE_TEAPOT:
        glutSolidTeapot(size);
        break;
    case DrawnShape::SHAPE_CUBE:
        glutSolidCube(size);
        break;
    case DrawnShape::SHAPE_SPHERE:
        glutSolidSphere(size, 20, 20);
        break;
    case DrawnShape::SHAPE_TEAPOT_2:
        glPushMatrix();
        {
            glutSolidTeapot(size);
            glTranslatef(9.5f, 9.5f, 0.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glutSolidTeapot(size);
        }
        glPopMatrix();
        break;
    case DrawnShape::SHAPE_CUBE_2:
        glPushMatrix();
        {
            glutSolidCube(size);
            glTranslatef(5.0f, 5.0f, 5.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glutSolidCube(size);
        }
        glPopMatrix();
        break;
    case DrawnShape::SHAPE_SPHERE_2:
        glPushMatrix();
        {
            glutSolidSphere(size, 20, 20);
            glTranslatef(8.0f, 8.0f, 8.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glutSolidSphere(size, 20, 20);
        }
        glPopMatrix();
        break;
    case DrawnShape::SHAPE_SCENE:
        glPushMatrix();
        {
            dessinerObjet(DrawnShape::SHAPE_CUBE, size);
            glTranslatef(-5.0f, 15.0f, 0.0f);
            glRotatef(90.0f, 0.2f, 0.5f, 0.7f);
            dessinerObjet(DrawnShape::SHAPE_TEAPOT_2, size);
        }
        glPopMatrix();
    case DrawnShape::SHAPE_NONE:
    default:
        break;
    }
}

void dessinerOutline(DrawnShape shape, double size)
{
    glPushMatrix();
    {
        glDisable(GL_LIGHTING);
        switch (Variable::outlineAlgorithm)
        {
        case OutlineAlgorithm::OUTLINE_HALO:
            Variable::progNuanceurOpenGL.activer();
            {
                glColor3f(0.0f, 0.0f, 0.0f);

                dessinerObjet(shape, size * 1.02);
            }
            Variable::progNuanceurCustom.activer();
            break;
        case OutlineAlgorithm::OUTLINE_NORMALS:
		    // Done in shader
			Variable::progNuanceurCustom.uniform1("affichageNormal", true);
			dessinerObjet(shape, size);
			Variable::progNuanceurCustom.uniform1("affichageNormal", false);
            break;
        case OutlineAlgorithm::OUTLINE_BF_CULLING:
            Variable::progNuanceurOpenGL.activer();
            {
                glEnable(GL_CULL_FACE);
                {
                    glCullFace(GL_BACK);
                    glPolygonMode(GL_FRONT, GL_LINE);
                    {
                        glEnable(GL_LINE_SMOOTH);
                        {
                            float lineWidth;
                            glGetFloatv(GL_LINE_WIDTH, &lineWidth);
                            // Multiply by 30/rho to compensate the line width with the camera zoom
                            glLineWidth(7.5f * (30.0 / Variable::rho));
                            {
                                glColor3f(0.0f, 0.0f, 0.0f);
                                dessinerObjet(shape, size);
                            }
                            glLineWidth(lineWidth);
                        }
                        glDisable(GL_LINE_SMOOTH);
                    }
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                glDisable(GL_CULL_FACE);
            }
            Variable::progNuanceurCustom.activer();
            break;
        case OutlineAlgorithm::OUTLINE_NONE:
        default:
            break;
        }
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
}

void dessinerObjetEtOutline(DrawnShape shape, double size)
{
    appliquerTextures();
    {
        switch (shape)
        {
        case DrawnShape::SHAPE_SCENE:

            switch (Variable::sceneRenderStyle)
            {
            case 0:
                // First way to generate a scene:
                //  all outlines -> clear depth buffer -> all objects
                //  objects in the back overwrite the outlines of objects in the front
                dessinerOutline(shape, size);

                glClear(GL_DEPTH_BUFFER_BIT);

                dessinerObjet(shape, size);

                break;

            case 1:
                // Second way to generate a scene:
                //  object's outline -> object's shape -> next object
                //  has different outcomes related to the order in which the objects are drawn
                glPushMatrix();
                {
                    dessinerObjetEtOutline(DrawnShape::SHAPE_CUBE, size);
                    glTranslatef(-5.0f, 15.0f, 0.0f);
                    glRotatef(90.0f, 0.2f, 0.5f, 0.7f);
                    dessinerObjetEtOutline(DrawnShape::SHAPE_TEAPOT_2, size);
                }
                glPopMatrix();
                break;

            case 2:
                // Third way to generate a scene:
                //  object's outline -> object's shape -> clear depth buffer -> next object
                //  the order in which the objects are drawn changes the outcome, would work otherwise
                glPushMatrix();
                {
                    dessinerObjetEtOutline(DrawnShape::SHAPE_CUBE, size);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    glTranslatef(-5.0f, 15.0f, 0.0f);
                    glRotatef(90.0f, 0.2f, 0.5f, 0.7f);
                    dessinerObjetEtOutline(DrawnShape::SHAPE_TEAPOT_2, size);
                }
                glPopMatrix();
                break;

            case 3:
                // Alternate way to generate a scene, using a stencil:
                //  does not work for Outline 4 (Halo) and 5 (Normal)
                //  has artefacts for Outline 6 (BF Culling)
                glEnable(GL_STENCIL_TEST);
                {
                    glClearStencil(0);
                    glClear(GL_STENCIL_BUFFER_BIT);
                    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

                    // Render the object
                    glStencilFunc(GL_ALWAYS, 1, -1);
                    dessinerObjet(shape, size);

                    // Render the Outline
                    glStencilFunc(GL_NOTEQUAL, 1, -1);
                    dessinerOutline(shape, size);
                }
                glDisable(GL_STENCIL_TEST);
                break;
            default:
                break;
            }

            break;
        case DrawnShape::SHAPE_TEAPOT:
        case DrawnShape::SHAPE_CUBE:
        case DrawnShape::SHAPE_SPHERE:
        case DrawnShape::SHAPE_TEAPOT_2:
        case DrawnShape::SHAPE_CUBE_2:
        case DrawnShape::SHAPE_SPHERE_2:

            switch (Variable::objectRenderStyle)
            {
            case 0:
                // Draw an object with a depth buffer

                dessinerOutline(shape, size);

                glClear(GL_DEPTH_BUFFER_BIT);

                dessinerObjet(shape, size);
                break;
            case 1:
                // Draw an object with a stencil buffer
                glEnable(GL_STENCIL_TEST);
                {
                    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

                    // Render the object
                    glStencilFunc(GL_ALWAYS, 1, -1);
                    dessinerObjet(shape, size);

                    // Render the outline
                    glStencilFunc(GL_NOTEQUAL, 1, -1);
                    dessinerOutline(shape, size);
                }
                glDisable(GL_STENCIL_TEST);
            }
            break;
        case DrawnShape::SHAPE_NONE:
        default:
            break;
        }
    }
    desactiverTextures();
}

void dessinerScene()
{
    Variable::progNuanceurCustom.activer();

    // Dessiner le modèle 
    glPushMatrix();
    {
        // appliquer le matériau du cube
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambiant_model);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_model);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_model);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission_model);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess_model);

        // dessiner le cube tournant, si désiré
        if (Variable::rotAutoModele) {
            Variable::angleRotX += 360.0 * (Constant::freqRotX / Constant::frameRate);
            Variable::angleRotY += 360.0 * (Constant::freqRotY / Constant::frameRate);
            Variable::angleRotZ += 360.0 * (Constant::freqRotZ / Constant::frameRate);

            if (Variable::angleRotX >= 360.0f)
                Variable::angleRotX -= 360.0f;

            if (Variable::angleRotY >= 360.0f)
                Variable::angleRotY -= 360.0f;

            if (Variable::angleRotZ >= 360.0f)
                Variable::angleRotZ -= 360.0f;
        }	

        glRotated(180.0, 1.0, 0.0, 0.0);
        glRotated(Variable::angleRotZ, 0.0, 0.0, 1.0);
        glRotated(Variable::angleRotY, 0.0, 1.0, 0.0);
        glRotated(Variable::angleRotX, 1.0, 0.0, 0.0);

        glClearStencil(0);
        glClear(GL_STENCIL_BUFFER_BIT);
        dessinerObjetEtOutline(Variable::drawnShape, 8.0);
    }
    glPopMatrix();

    Variable::progNuanceurCustom.activer();

    // flush les derniers vertex du pipeline graphique
    glFlush();
}

// fonction d'affichage
void affichage(void)
{
    // effacement de l'ecran et du tampon de profondeur
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // charger matrice de modélisation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // mettre à jour la caméra openGL
    rafraichirCamera();  // ne pas permettre pour l'instant de déplacer la caméra...

    // mettre à jour l'éclairage
    rafraichirEclairage();

    // dessiner la scène
    dessinerScene(); 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // mettre à jour la projection
    rafraichirProjection();

    // utiliser le "double-buffering"
    glutSwapBuffers();
}

// fonction appelee lorsque l'application ne fait rien
void attente(void)
{
    // une variable pour memoriser le temps a attendre
    static int nWaitUntil = glutGet(GLUT_ELAPSED_TIME);

    // on recupere le temps présent
    int nTimer = glutGet(GLUT_ELAPSED_TIME);
    // et on le compare a l'instant qu'il faut attendre
    if(nTimer >= nWaitUntil)
    {
        // pour rafraichir l'affichage
        glutPostRedisplay();
        // 5 fois pa5 seconde
        nWaitUntil = nTimer + (1000 / Constant::frameRate);
    }
}

// fonction de gestion du clavier
// 1-3 = Shape
// 4-6 = Outline, O pour enlever l'outline
// 7-9 = Shader
void clavier(unsigned char touche, int x, int y)
{
    // traitement des touches q et echap
    switch(touche) {
    case '1' :
        //Teapot
        Variable::drawnShape = DrawnShape::SHAPE_TEAPOT;
        break;
    case '2':
        //Cube
        Variable::drawnShape = DrawnShape::SHAPE_CUBE;
        break;
    case '3':
        //Sphere
        Variable::drawnShape = DrawnShape::SHAPE_SPHERE;
        break;
    case '!':
        //Teapot
        Variable::drawnShape = DrawnShape::SHAPE_TEAPOT_2;
        break;
    case '@':
        //Cube
        Variable::drawnShape = DrawnShape::SHAPE_CUBE_2;
        break;
    case '#':
        //Sphere
        Variable::drawnShape = DrawnShape::SHAPE_SPHERE_2;
        break;
    case 'z':
        //Scene
        Variable::drawnShape = DrawnShape::SHAPE_SCENE;
        break;
    case 'x':
        //Scene
        Variable::sceneRenderStyle++;
        if (Variable::sceneRenderStyle > 3)
        {
            Variable::sceneRenderStyle = 0;
        }
        switch (Variable::sceneRenderStyle)
        {
        case 0:
            std::cout << "Scene Style: Depth: All Outlines -> All Objects " << std::endl;
            break;
        case 1:
            std::cout << "Scene Style: Depth: Obj1 -> Obj2 -> ..." << std::endl;
            break;
        case 2:
            std::cout << "Scene Style: Depth: Obj1 -> Obj2 -> ... w/ Depth Clear" << std::endl;
            break;
        case 3:
            std::cout << "Scene Style: Stencil Buffer" << std::endl;
            break;
        default:
            std::cout << "Scene Style: Unknown" << std::endl;
            break;
        }
        break;
    case 'c':
        //Scene
        Variable::objectRenderStyle++;
        if (Variable::objectRenderStyle > 1)
        {
            Variable::objectRenderStyle = 0;
        }
        switch (Variable::objectRenderStyle)
        {
        case 0:
            std::cout << "Object Style: Depth" << std::endl;
            break;
        case 1:
            std::cout << "Object Style: Stencil" << std::endl;
            break;
        default:
            std::cout << "Object Style: Unknown" << std::endl;
            break;
        }
        break;

    case '4':
        //Halo
        Variable::outlineAlgorithm = OutlineAlgorithm::OUTLINE_HALO;
        break;
    case '5':
        //Normals
        Variable::outlineAlgorithm = OutlineAlgorithm::OUTLINE_NORMALS;
        break;
    case '6':
        //Backface Culling
        Variable::outlineAlgorithm = OutlineAlgorithm::OUTLINE_BF_CULLING;
        break;
    case 'o':
        //No Outline
        Variable::outlineAlgorithm = OutlineAlgorithm::OUTLINE_NONE;
        break;

    case '7':
        //Normal
        Variable::shaderStyle = ShaderStyle::SHADER_CLASSIC;
        Variable::progNuanceurCustom.uniform1("renderStyle", ShaderStyle::SHADER_CLASSIC);
        break;
    case '8':
        //Cel
        Variable::shaderStyle = ShaderStyle::SHADER_CEL_SHADING;
        Variable::progNuanceurCustom.uniform1("renderStyle", ShaderStyle::SHADER_CEL_SHADING);
        break;
    case '9':
        //Gooch
        Variable::shaderStyle = ShaderStyle::SHADER_GOOCH_SHADING;
        Variable::progNuanceurCustom.uniform1("renderStyle", ShaderStyle::SHADER_GOOCH_SHADING);
        break;

    case 'q':
        // destruction de la fenetre GLUT
        glutDestroyWindow(Variable::g_nWindowID);
        // on quitte notre programme
        exit(EXIT_SUCCESS);
        break;
      
    case 'p' :
        Variable::isPerspective = !Variable::isPerspective;
        affichage();
        break;

    // permutation du "spot light"
    case 's' :
        Variable::spotLightAllumee = !Variable::spotLightAllumee;
        glutPostRedisplay();
        break;

    // permutation du "dirLight"
    case 'd' :
        Variable::dirLightAllumee = !Variable::dirLightAllumee;
        glutPostRedisplay();
        break;

    // permutation du "point light"
    case 'a' :
        Variable::pointLightAllumee = !Variable::pointLightAllumee;
        glutPostRedisplay();
        break;

    // Toggle the textures
    case 't':
        Variable::utiliserTextures = !Variable::utiliserTextures;
        Variable::progNuanceurCustom.uniform1("utiliserTextures", Variable::utiliserTextures);
        glutPostRedisplay();
        break;

    case '-' :
        // la caméra recule
        Variable::rho += 2.0f;
        affichage();
        break;

    case '=':
    case '+':
        // la caméra avance
        Variable::rho -= 2.0f;
        if (Variable::rho <= 1.0)
            Variable::rho = 1.0f;
        affichage();
        break;

    case ' ' :
        // permuter la rotation automatique du modèle
        Variable::rotAutoModele = !Variable::rotAutoModele;
        affichage();
        break;
    }
}

// fonction de gestion du clavier spécial
void clavierSpecial(int touche, int x, int y)
{
    switch(touche)
    {
    case GLUT_KEY_RIGHT :
        // effectuer une rotation vers la droite autour du cube
        Variable::theta += DEG2RAD(2.0f);
        affichage();
        break;

    case GLUT_KEY_LEFT :
        // effectuer une rotation vers la droite autour du cube
        Variable::theta -= DEG2RAD(2.0f);
        affichage();
        break;

    case GLUT_KEY_UP :
        // effectuer une rotation vers la droite autour du cube
        Variable::phi -= DEG2RAD(2.0f);
        affichage();
        break;

    case GLUT_KEY_DOWN :
        // effectuer une rotation vers la droite autour du cube
        Variable::phi += DEG2RAD(2.0f);
        affichage();
        break;
    }
}

// fonction de redimensionnement de la fenêtre graphique
void redimensionnement(GLsizei w, GLsizei h)
{
    Variable::currentW = w;
    Variable::currentH = h;
    glViewport(0, 0, w, h);
    affichage();
}


// fonction callback lorsque la souris est cliquee
void sourisClickee(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        // on vient de presser la souris
        Variable::dernierX = x;
        Variable::dernierY = y;
    }
}

// fonction de redimensionnement de la fenêtre graphique
void mouvementSouris(int x, int y)
{
    Variable::angleRotX += y - Variable::dernierY;
    Variable::angleRotZ -= x - Variable::dernierX;

    Variable::dernierX = x;
    Variable::dernierY = y;
}

int main(int argc,char *argv[])
{
    // initialisation de GLUT
    glutInit(&argc, argv);
    // parametrage de l'affichage
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    // taille de la fenetre
    glutInitWindowSize(800, 800);
    // creation de la fenetre
    Variable::g_nWindowID = glutCreateWindow("Exemple");

    // referencement de la fonction d'affichage
    glutDisplayFunc(affichage);
    // referencement de la fonction d'attente
    glutIdleFunc(attente);
    // referencement de la fonction de gestion des touches
    glutKeyboardFunc(clavier);
    // referencement de la fonction de gestion spéciale des touches extended
    glutSpecialFunc(clavierSpecial);
    // referencement de la fonction de redimensionnement
    glutReshapeFunc (redimensionnement);
    // référencement de la fonction de mouvement de la souris
    glutMotionFunc (mouvementSouris);
    // référencement de la fonction de click de la souris
    glutMouseFunc (sourisClickee);

    // vérification de la présence des extensions
    // ou de la version 2.0 d'openGL
    glewInit();
    if (glewIsSupported("GL_VERSION_2_0"))
    {
        printf("Ready for OpenGL 2.0\n");
    }
    else
    {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }

    // compiler et linker les nuanceurs
    Variable::progNuanceurCustom.compilerEtLier();

    // initialisation de variables d'état openGL et création des listes
    initialisation();

    // boucle principale de gestion des evenements
    glutMainLoop();

    // le programme n'arrivera jamais jusqu'ici
    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////
////////////  FONCTIONS POUR LA CAMÉRA ///////////////////
//////////////////////////////////////////////////////////
void rafraichirCamera()
{
    Variable::x = Variable::rho * cos(Variable::theta) * sin(Variable::phi);
    Variable::y = Variable::rho * sin(Variable::theta) * sin(Variable::phi);
    Variable::z = Variable::rho * cos(Variable::phi);

    gluLookAt(Variable::x, Variable::y, Variable::z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

//////////////////////////////////////////////////////////
///////////  FONCTIONS POUR L'ÉCLAIRAGE //////////////////
//////////////////////////////////////////////////////////
void rafraichirEclairage()
{
    // inscription des variables de lumière dans le nuanceur
    if (Variable::pointLightAllumee)
    {
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
        glLightfv(GL_LIGHT0, GL_EMISSION, light_emission0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

        glEnable(GL_LIGHT0);
        Variable::progNuanceurCustom.uniform1("pointLightOn", 1);
    }
    else
    {
        glDisable(GL_LIGHT0);
        Variable::progNuanceurCustom.uniform1("pointLightOn", 0);
    }

    if (Variable::spotLightAllumee)
    {
        // variables position et spotDirection pour la "flashLight")
        GLfloat spotPosition[4] =  { Variable::x, Variable::y, Variable::z, 1.0f};
        GLfloat spotDirection[3] = {-Variable::x,-Variable::y,-Variable::z};

        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
        glLightfv(GL_LIGHT1, GL_EMISSION, light_emission1);
        glLightfv(GL_LIGHT1, GL_POSITION, spotPosition);
        glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 1.0f);
        glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f); 
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection); 

        glEnable(GL_LIGHT1);
        Variable::progNuanceurCustom.uniform1("spotLightOn", 1);
    }
    else
    {
        glDisable(GL_LIGHT1);
        Variable::progNuanceurCustom.uniform1("spotLightOn", 0);
    }

    if (Variable::dirLightAllumee)
    {
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
        glLightfv(GL_LIGHT2, GL_EMISSION, light_emission2);
        glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

        glEnable(GL_LIGHT2);
        Variable::progNuanceurCustom.uniform1("dirLightOn", 1);
    }
    else
    {
        glDisable(GL_LIGHT2);
        Variable::progNuanceurCustom.uniform1("dirLightOn", 0);
    }
}

//////////////////////////////////////////////////////////
//////////  FONCTIONS POUR LA PROJECTION /////////////////
//////////////////////////////////////////////////////////
void rafraichirProjection()
{
    if (Variable::isPerspective)
    {
        gluPerspective(60.0, (GLfloat)Variable::currentW / (GLfloat)Variable::currentH, 0.1, 300.0);
    }
    else
    {
        if (Variable::currentW <= Variable::currentH)
        {
            glOrtho(-10.0, 10.0, -10.0*(GLfloat)Variable::currentH / (GLfloat)Variable::currentW,
                10.0*(GLfloat)Variable::currentH / (GLfloat)Variable::currentW, 0.01, 300.0);
        }
        else
        {
            glOrtho(-10.0*(GLfloat)Variable::currentW / (GLfloat)Variable::currentH,
                10.0*(GLfloat)Variable::currentW / (GLfloat)Variable::currentH, -10.0, 10.0, 0.01, 300.0);
        }
    }
}

//////////////////////////////////////////////////////////
//////////  FONCTIONS POUR LES NUANCEURS /////////////////
//////////////////////////////////////////////////////////
void afficherShaderInfoLog(GLuint obj, char* message)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    // afficher le message d'en-tête
    printf("%s\n", message);

    // afficher le message d'erreur, le cas échéant
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 1)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    } 
    else 
    {
        printf("Aucune erreur :-)\n\n");
    }
}

void afficherProgramInfoLog(GLuint obj, char* message)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    // afficher le message d'en-tête
    printf("%s\n", message);

    // afficher le message d'erreur, le cas échéant
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    } 
    else 
    {
        printf("Aucune erreur :-)\n\n");
    }
}

//////////////////////////////////////////////////////////
//////////  FONCTIONS POUR LES TEXTURES //////////////////
//////////////////////////////////////////////////////////
void appliquerTextures()
{
    // ETAGE 0 (texture de mur)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Variable::texture0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_2D);
	/*
    // ETAGE 1 (texture de rust)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Variable::texture1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 2.0f);

    glEnable(GL_TEXTURE_2D);

    // ETAGE 2 (texture "paint job")
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, Variable::texture2);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_2D);*/

    /*glUniform1i(glGetUniformLocation(fixedPipelineShaderProg, "texUnit0"), 0);
    glUniform1i(glGetUniformLocation(fixedPipelineShaderProg, "texUnit1"), 1);
    glUniform1i(glGetUniformLocation(fixedPipelineShaderProg, "texUnit2"), 2);*/
}

SImage* LoadBMP( char *Filename )
{
    if( !Filename )
        return NULL;

    FILE *file;
    unsigned int  width;
    unsigned int  height;
    unsigned long i;
    unsigned short int planes; // planes in image (must be 1) 
    unsigned short int bpp;    // bits per pixel (must be 24)
	
    // make sure the file is there.
    if( ( file = fopen( Filename, "rb" ) ) == NULL )
    {
        printf( "File Not Found : %s\n", Filename );
        return NULL;
    }

    // seek through the bmp header, up to the width/height:
    fseek( file, 18, SEEK_CUR );

    // read the width
    if( ( i = ( unsigned long )fread( &width, 4, 1, file ) ) != 1 )
    {
        printf( "Error reading width from %s.\n", Filename );
        return NULL;
    }

    // read the height 
    if( ( i = ( unsigned long )fread( &height, 4, 1, file ) ) != 1 )
    {
        printf( "Error reading height from %s.\n", Filename );
        return NULL;
    }

    // read the planes
    if( ( ( unsigned long )fread( &planes, 2, 1, file ) ) != 1 )
    {
        printf( "Error reading planes from %s.\n", Filename );
        return NULL;
    }

    if( planes != 1 )
    {
        printf( "Planes from %s is not 1: %u\n", Filename, planes );
        return NULL;
    }

    // read the bpp
    if( ( i = ( unsigned long )fread( &bpp, 2, 1, file ) ) != 1 )
    {
        printf("Error reading bpp from %s.\n", Filename );
        return NULL;
    }

    if( bpp != 24 )
    {
        printf( "Bpp from %s is not 24: %u\n", Filename, bpp );
        return NULL;
    }

    // seek past the rest of the bitmap header.
    fseek( file, 24, SEEK_CUR );

    // read the data.
    SImage* Image = new SImage( width, height );

    if( ( i = ( unsigned long )fread( Image->data, width * height * 3, 1, file ) ) != 1 )
    {
        printf("Error reading image data from %s.\n", Filename );
        delete Image;
        return NULL;
    }

    return Image;
}

void init2DTexture(GLint texName, GLint texWidth, GLint texHeight, GLubyte *texPtr)
{
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0); 
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texWidth, texHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, texPtr);
}

void chargerTextures()
{	
    SImage* Image = NULL;

    if( Image = LoadBMP("textures/stonewalldiffuse.bmp" ) )
    {
        glGenTextures( 1, &Variable::texture0 );
        init2DTexture(Variable::texture0, Image->width, Image->height, Image->data );
        delete Image;
    }

    if( Image = LoadBMP( "textures/rust.bmp" ) )
    {
        glGenTextures( 1, &Variable::texture1 );
        init2DTexture(Variable::texture1, Image->width, Image->height, Image->data );
        delete Image;
    }

    if( Image = LoadBMP( "textures/3dlabs.bmp" ) )
    {
        glGenTextures( 1, &Variable::texture2 );
        init2DTexture(Variable::texture2, Image->width, Image->height, Image->data );
        delete Image;
    }
}

void desactiverTextures()
{
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE2);
    glDisable(GL_TEXTURE_2D);
}
