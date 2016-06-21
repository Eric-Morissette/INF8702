#include "Quadrique.h"

using namespace Scene;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CQuadrique \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::CQuadrique( void )
    : ISurface     (                  )
    , m_Quadratique( CVecteur3::ZERO  )
    , m_Lineaire   ( CVecteur3::ZERO  )
    , m_Mixte      ( CVecteur3::ZERO  )
    , m_Cst        ( RENDRE_REEL( 0 ) )
{}

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CQuadrique \n
///  Description : Constructeur par défaut
///
///  @param [in]       Quadric const Scene::CQuadrique &   la quadrique à copier
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::CQuadrique( const CQuadrique& Quadric )
    : ISurface     ( Quadric               )
    , m_Quadratique( Quadric.m_Quadratique )
    , m_Lineaire   ( Quadric.m_Lineaire    )
    , m_Mixte      ( Quadric.m_Mixte       )
    , m_Cst        ( Quadric.m_Cst         )
{}

///////////////////////////////////////////////////////////////////////////////
///  public virtual destructor  ~CQuadrique \n
///  Description : Destructeur
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::~CQuadrique( void )
{
}

///////////////////////////////////////////////////////////////////////////////
///  public  operator = \n
///  Description : Opérateur de copie
///
///  @param [in]       Quadric const Scene::CQuadrique &    La quadrique à copier
///
///  @return Scene::CQuadrique & La quadrique modifiée
///
///  @author Olivier Dionne 
///  @date   14/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique& CQuadrique::operator = ( const CQuadrique& Quadric )
{
    ISurface::operator =( Quadric );
    m_Quadratique = Quadric.m_Quadratique;
    m_Lineaire    = Quadric.m_Lineaire;
    m_Mixte       = Quadric.m_Mixte;
    m_Cst         = Quadric.m_Cst;
    return ( *this );
}

///////////////////////////////////////////////////////////////////////////////
///  protected virtual constant  AfficherInfoDebug \n
///  Description : Implémente le déboguage polymorphique par flux de sortie
///
///  @param [in, out]  Out std::ostream &    Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
ostream& CQuadrique::AfficherInfoDebug( ostream& Out ) const
{
    Out << "[DEBUG]: Quadric.Quadratique       = " << m_Quadratique << endl;
    Out << "[DEBUG]: Quadric.Lineaire          = " << m_Lineaire    << endl;
    Out << "[DEBUG]: Quadric.Mixte             = " << m_Mixte       << endl;
    Out << "[DEBUG]: Quadric.Constante         = " << m_Cst;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Pretraitement \n
///  Description : Pretraitement des données de la quadrique( appelé AVANT le lancer)
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CQuadrique::Pretraitement( void )
{
   // Algorithme tiré de ... 
   // R. Goldman, "Two Approach to a Computer Model for Quadric Surfaces",
   // IEEE CG&A, Sept 1983, pp.21
   
    REAL A = m_Quadratique.x;
	REAL B = m_Quadratique.y;
    REAL C = m_Quadratique.z;
    REAL D = m_Mixte.z    * RENDRE_REEL( 0.5 );
	REAL E = m_Mixte.x    * RENDRE_REEL( 0.5 );
	REAL F = m_Mixte.y    * RENDRE_REEL( 0.5 );
	REAL G = m_Lineaire.x * RENDRE_REEL( 0.5 );
	REAL H = m_Lineaire.y * RENDRE_REEL( 0.5 );
	REAL J = m_Lineaire.z * RENDRE_REEL( 0.5 );
	REAL K = m_Cst;

	CMatrice4 Q( A, D, F, G,
			     D, B, E, H,
			     F, E, C, J,
			     G, H, J, K );

    CMatrice4 Inverse = m_Transformation.Inverse();

    Q = Inverse * Q * Inverse.Transpose();

    m_Quadratique.x = Q[ 0 ][ 0 ];
    m_Quadratique.y = Q[ 1 ][ 1 ];
	m_Quadratique.z = Q[ 2 ][ 2 ];
	m_Cst           = Q[ 3 ][ 3 ];
	m_Mixte.x       = Q[ 1 ][ 2 ] * RENDRE_REEL( 2.0 );
	m_Mixte.y       = Q[ 0 ][ 2 ] * RENDRE_REEL( 2.0 );
	m_Mixte.z       = Q[ 0 ][ 1 ] * RENDRE_REEL( 2.0 );
	m_Lineaire.x    = Q[ 0 ][ 3 ] * RENDRE_REEL( 2.0 );
	m_Lineaire.y    = Q[ 1 ][ 3 ] * RENDRE_REEL( 2.0 );
    m_Lineaire.z    = Q[ 2 ][ 3 ] * RENDRE_REEL( 2.0 );
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Intersection \n
///  Description : Effectue l'intersection Rayon/Quadrique
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///
///  @return Scene::CIntersection Le résultat de l'ntersection
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CIntersection CQuadrique::Intersection( const CRayon& Rayon )
{
	CIntersection Result;
	float fIntersection;

	//Composants du rayon
	const CVecteur3 dir = Rayon.ObtenirDirection();
	const CVecteur3 ori = Rayon.ObtenirOrigine();

	//matrice Quadratique Selon les notes de cours

	//matrice Quadratique Selon le tp
	const REAL A = m_Quadratique.x;
	const REAL B = m_Quadratique.y;
	const REAL C = m_Quadratique.z;
	const REAL D = m_Mixte.x;
	const REAL E = m_Mixte.y;
	const REAL F = m_Mixte.z;
	const REAL G = m_Lineaire.x;
	const REAL H = m_Lineaire.y;
	const REAL I = m_Lineaire.z;
	const REAL J = m_Cst;

	// Facteurs quadratiques
	REAL AQuad = A * pow(dir.x, 2) + B * pow(dir.y, 2) + C * pow(dir.z, 2) 
		+ D * dir.x * dir.y + E * dir.x * dir.z + F * dir.y * dir.z;

	REAL BQuad = 2 * A * ori.x * dir.x + 2 * B * dir.y * ori.y + 2 * C * dir.z * ori.z + D * (ori.x * dir.y + ori.y * dir.x)
		+ E * ori.x * dir.z + F * (ori.y * dir.z + dir.y * ori.z) + G * dir.x + H * dir.y + I * dir.z;

	REAL CQuad = A * pow(ori.x, 2) + B * pow(ori.y, 2) + C * pow(ori.z, 2) + D * ori.x * ori.y +
		E * ori.x * ori.z + F * ori.y * ori.z + G * ori.x + H * ori.y + I * ori.z + J;

	if (AQuad == 0.f)
	{
		fIntersection = -CQuad / BQuad;
	}
	else
	{
		float discriminant = (pow(BQuad, 2)) - (4 * AQuad * CQuad);
		if (discriminant < 0.f)
		{
			return Result;
		}
		else
		{
			float intersection1 = (-BQuad - sqrt(discriminant)) / (2 * AQuad);
			float intersection2 = (-BQuad + sqrt(discriminant)) / (2 * AQuad);

			// Si les deux points d'intersection sont negatifs, il n'y a pas de collision
			if (intersection1 < 0 && intersection2 < 0)
			{
				return Result;
			}
			// On veut le point le plus pres de la camera
			else if (intersection1 > intersection2)
			{
				fIntersection = intersection2;
			}
			else //if (intersection1 < intersection2)
			{
				fIntersection = intersection1;
			}
		}
	}
	
	// S'il y a collision
	CVecteur3 vIntersection(ori + dir * fIntersection);

	// Ajuster Distance
	Result.AjusterDistance(CVecteur3::Norme(vIntersection - ori));

	// Ajuster la Normale
	CVecteur3 nIntersection;
	nIntersection.x = 2 * A * vIntersection.x + D * vIntersection.y + E * vIntersection.z + G;
	nIntersection.y = 2 * B * vIntersection.y + D * vIntersection.x + F * vIntersection.z + H;
	nIntersection.z = 2 * C * vIntersection.z + E * vIntersection.x + F * vIntersection.y + I;

	if (CVecteur3::ProdScal(nIntersection, dir) > 0)
	{
		nIntersection = -nIntersection;
	}
	Result.AjusterNormale(CVecteur3::Normaliser(nIntersection));

	// Ajuster la surface
	Result.AjusterSurface(this);

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual constant  Copier \n
///  Description : Alloue une copie de la quadrique courante
///
///  @return Scene::CQuadrique *la copie de la quadrique
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique* CQuadrique::Copier( void ) const
{
    return new CQuadrique( *this );
}