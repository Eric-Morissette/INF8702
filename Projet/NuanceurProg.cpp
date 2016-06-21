
#include "NuanceurProg.h"


CNuanceurProg::CNuanceurProg(void)
{
    // indiquer que le programme en cours sera un programme vide (et déjà compilé)
    estVide_ = true;
    estCompileEtLie_ = true;

    // met prog_ à  0 parce que glUseProgram(0) va activer la fonctionalité fixe
    prog_ = 0;
}


CNuanceurProg::CNuanceurProg(char* nuanceurSommetsStr, char* nuanceurFragmentsStr, bool compilerMaintenant) :
estCompileEtLie_(false)
{
    // s'assurer qu'au moins UN des deux nuanceurs est défini
    assert(!((nuanceurSommetsStr == NULL) && (nuanceurFragmentsStr == NULL)));
   
    // indiquer que le programme de nuanceurs n'est pas vide
    estVide_ = false;

    //  mémoriser les noms de fichiers des nuanceurs
    nuanceurSommetsStr_ = nuanceurSommetsStr;
    nuanceurFragmentsStr_ = nuanceurFragmentsStr;

    if (compilerMaintenant) {
        compilerEtLierNuanceurs(nuanceurSommetsStr, nuanceurFragmentsStr);
    }
}


CNuanceurProg::~CNuanceurProg(void){ }


void CNuanceurProg::compilerEtLier()
{
    // vérifie si le programme en cours est un programme de nuanceurs
    assert(!estVide_);
    compilerEtLierNuanceurs(nuanceurSommetsStr_, nuanceurFragmentsStr_);
}


void CNuanceurProg::enregistrerUniformFloat(const char* nom, const float& val)
{
    floatUniform u(nom, &val);
    floatUniforms_.push_back(u);
}


void CNuanceurProg::enregistrerUniformInteger(const char* nom, const int& val)
{
    integerUniform u(nom, &val);
    integerUniforms_.push_back(u);
}


void CNuanceurProg::activer()
{
    // activer le programme de nuanceurs
    glUseProgram(prog_);

    // inscrire les uniforms float requis par les nuanceurs
    if (!floatUniforms_.empty()) {
        for (unsigned int i = 0; i<floatUniforms_.size(); i++) {
            glUniform1f(glGetUniformLocation(prog_, floatUniforms_[i].nom), (float)*floatUniforms_[i].val);
        }
    }

    // inscrire les uniforms int requis par les nuanceurs
    if (!integerUniforms_.empty()) {
        for (unsigned int i=0; i<integerUniforms_.size(); i++) {
            glUniform1i(glGetUniformLocation(prog_, integerUniforms_[i].nom), (int)*integerUniforms_[i].val);
        }
    }
}


void CNuanceurProg::uniform1(char* nom, int v)
{
    glUniform1i(glGetUniformLocation(prog_, nom), v);
}


void CNuanceurProg::uniform1(char* nom, float v)
{
    glUniform1f(glGetUniformLocation(prog_, nom), v);
}


void CNuanceurProg::afficherShaderInfoLog(GLuint obj, char* message)
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


void CNuanceurProg::afficherProgramInfoLog(GLuint obj, char* message)
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


void CNuanceurProg::compilerEtLierNuanceurs(char* nsStr, char* nfStr) 
{	
    GLuint nuanceurSommets;
    GLuint nuanceurFragments;

    char *ns = NULL;
    char *nf = NULL;

    // création du NUANCEUR DE SOMMETS (si spécifié)
    if (nsStr) {
        // indiquer la progression...
        printf("Compilation du nuanceur de sommets   : %s \n", nsStr);

        // créer le nuanceur en GLSL
        nuanceurSommets = glCreateShader(GL_VERTEX_SHADER);

        // lecture du code du nuanceur
        ns = textFileRead(nsStr);

        // créer un pointeur sur le texte du code du nuanceur
        const char * ns_ptr = ns;

        // sourcer le nuanceur
        glShaderSource(nuanceurSommets, 1, &ns_ptr, NULL);

        // libérer la mémoire des codes source
        free(ns);

        glCompileShader(nuanceurSommets);
        afficherShaderInfoLog(nuanceurSommets, "ERREURS DE COMPILATION DU NUANCEUR DE SOMMETS : ");
    }

    // création du NUANCEUR DE FRAGMENTS (si spécifié)
    if (nfStr) {
        // indiquer la progression...
        printf("Compilation du nuanceur de fragments : %s \n", nfStr);

        // créer le nuanceur en GLSL
        nuanceurFragments = glCreateShader(GL_FRAGMENT_SHADER);

        // lecture du code du nuanceur
        nf = textFileRead(nfStr);

        // créer un pointeur sur le texte du code du nuanceur
        const char * nf_ptr = nf;

        // sourcer le nuanceur
        glShaderSource(nuanceurFragments, 1, &nf_ptr, NULL);

        // libérer la mémoire des codes source
        free(nf);

        glCompileShader(nuanceurFragments);
        afficherShaderInfoLog(nuanceurFragments, "ERREURS DE COMPILATION DU NUANCEUR DE FRAGMENTS : ");
    }

    // créer le programme des nuanceurs et lier
    prog_ = glCreateProgram();
    glAttachShader(prog_,nuanceurSommets);
    glAttachShader(prog_,nuanceurFragments);
    glLinkProgram(prog_);

    // afficher les erreurs de compilation et de linkage
    afficherProgramInfoLog(prog_, "ERREURS DE L'EDITION DES LIENS : ");    

    // marquer les nuanceurs compilés
    estCompileEtLie_ = true;
}


GLuint CNuanceurProg::getProg()
{
    return prog_;
}