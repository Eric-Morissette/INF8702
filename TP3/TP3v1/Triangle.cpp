#include "Triangle.h"

using namespace Scene;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CTriangle \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::CTriangle( void )
    : ISurface (                 )
    , m_Normale( CVecteur3::ZERO )
{}

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CTriangle \n
///  Description : Constructeur par défaut
///
///  @param [in]       Triangle const Scene::CTriangle &    Le triangle à copier
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::CTriangle( const CTriangle& Triangle )
    : ISurface ( Triangle           )
    , m_Normale( Triangle.m_Normale )
{
    for( int i = 0; i < 3; i++ )
        m_Pts[ i ] = Triangle.m_Pts[ i ];
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual destructor  ~CTriangle \n
///  Description : Destructeur
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::~CTriangle( void )
{
}

///////////////////////////////////////////////////////////////////////////////
///  public  operator = \n
///  Description : Opérateur de copie
///
///  @param [in]       Triangle const Scene::CTriangle &    Le triangle à copier
///
///  @return Scene::CTriangle & Le triangle modifié
///
///  @author Olivier Dionne 
///  @date   14/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle& CTriangle::operator = ( const CTriangle& Triangle )
{
    ISurface::operator =( Triangle );
    m_Normale = Triangle.m_Normale;

    for( int i = 0; i < 3; i++ )
        m_Pts[ i ] = Triangle.m_Pts[ i ];

    return ( *this );
}

///////////////////////////////////////////////////////////////////////////////
///  protected virtual constant  AfficherInfoDebug \n
///  Description : Implémente le déboguage polymorphique par flux de sortie
///
///  @param [in, out]  Out std::ostream &   Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
ostream& CTriangle::AfficherInfoDebug( ostream& Out ) const
{
    Out << "[DEBUG]: Triangle.Point1         = " << m_Pts[ 0 ] << endl;
    Out << "[DEBUG]: Triangle.Point2         = " << m_Pts[ 1 ] << endl;
    Out << "[DEBUG]: Triangle.Point3         = " << m_Pts[ 2 ] << endl;
    Out << "[DEBUG]: Triangle.Normale        = " << m_Normale;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Pretraitement \n
///  Description : Pretraitement des données du triangle ( Appelé AVANT le lancer)
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CTriangle::Pretraitement( void )
{
    for( int i = 0; i < 3; i++ )
        m_Pts[ i ] = m_Pts[ i ] * m_Transformation;
    CalculerNormale();
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Intersection \n
///  Description : Effectue l'intersection Rayon/Triangle
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///
///  @return Scene::CIntersection Le résultat de l'ntersection
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CIntersection CTriangle::Intersection( const CRayon& Rayon )
{
    CIntersection Result;

	CVecteur3 dir = Rayon.ObtenirDirection();
	CVecteur3 ori = Rayon.ObtenirOrigine();

	REAL t, u, v;

	CVecteur3 E1 = m_Pts[1] - m_Pts[0];
	CVecteur3 E2 = m_Pts[2] - m_Pts[0];

	CVecteur3 vP = CVecteur3::ProdVect(dir, E2);
	REAL determinant = CVecteur3::ProdScal(E1, vP);

	if (determinant < 0.f)
	{
		return Result;
	}

	CVecteur3 vT = ori - m_Pts[0];
	u = CVecteur3::ProdScal(vT, vP);
	if (u < 0.f || u > determinant)
	{
		return Result;
	}

	CVecteur3 vQ = CVecteur3::ProdVect(vT, E1);
	v = CVecteur3::ProdScal(dir, vQ);
	if (v < 0.f || (u + v) > determinant)
	{
		return Result;
	}

	REAL inverseDeterminant = 1.f / determinant;
	t = CVecteur3::ProdScal(E2, vQ) * inverseDeterminant;

	// Si on se rend ici, il y a collision
	Result.AjusterDistance(t);
	Result.AjusterNormale(m_Normale);
	Result.AjusterSurface(this);

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual constant  Copier \n
///  Description : Alloue une copie du triangle courant
///
///  @return Scene::CTriangle * Nouvelle copie du triangle 
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle* CTriangle::Copier( void ) const
{
    return new CTriangle( *this );
}

///////////////////////////////////////////////////////////////////////////////
///  private  CalculerNormale \n
///  Description : Calculer la normale du triangle à partir des côtés
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CTriangle::CalculerNormale( void )
{
    CVecteur3 Edge1 = m_Pts[ 1 ] - m_Pts[ 0 ];
    CVecteur3 Edge2 = m_Pts[ 2 ] - m_Pts[ 0 ];
    m_Normale = CVecteur3::Normaliser( CVecteur3::ProdVect( Edge1, Edge2 ) );
}
