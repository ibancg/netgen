#ifndef FILE_GEOMETRY2D
#define FILE_GEOMETRY2D

/* *************************************************************************/
/* File:   geometry2d.hpp                                                  */
/* Author: Joachim Schoeberl                                               */
/* Date:   20. Jul. 02                                                     */
/* *************************************************************************/

#include <myadt.hpp>
#include <gprim.hpp>


// #include "../gprim/spline.hpp"
// #include "../gprim/splinegeometry.hpp"
#include "geom2dmesh.hpp"

namespace netgen
{

  class SplineSegExt : public SplineSeg<2>
  {
  public:
    SplineSeg<2>* seg;
    
    /// left domain
    int leftdom;
    /// right domain
    int rightdom;
    /// refinement at line
    double reffak;
    /// maximal h;
    double hmax;
    /// boundary condition number
    int bc;
    /// copy spline mesh from other spline (-1.. do not copy)
    int copyfrom;
    /// perform anisotropic refinement (hp-refinement) to edge
    double hpref_left;
    /// perform anisotropic refinement (hp-refinement) to edge
    double hpref_right;
    ///
    int layer;

    SplineSegExt (SplineSeg<2> & hseg)
      : seg(&hseg)
    {
      layer = 1;
    }
    // default constructor for archive
    SplineSegExt() {}

    ~SplineSegExt ()
    {
      delete seg;
    }

    virtual void DoArchive(Archive& ar)
    {
      ar & seg & leftdom & rightdom & reffak & hmax & bc & copyfrom
        & hpref_left & hpref_right & layer;
    }
    
    virtual const GeomPoint<2> & StartPI () const 
    { 
      return seg->StartPI();
    }

    virtual const GeomPoint<2> & EndPI () const 
    {
      return seg->EndPI();
    }

    virtual Point<2> GetPoint (double t) const 
    {
      return seg->GetPoint(t);
    }

    virtual Vec<2> GetTangent (const double t) const
    {
      return seg->GetTangent(t);
    }

    virtual void GetDerivatives (const double t,  
				 Point<2> & point,
				 Vec<2> & first,
				 Vec<2> & second) const
    {
      seg->GetDerivatives (t, point, first, second);
    }

    virtual void GetCoeff (Vector & coeffs) const 
    {
      seg->GetCoeff (coeffs);
    }

    virtual void GetPoints (int n, Array<Point<2> > & points) const
    {
      seg->GetPoints (n, points);
    }

    virtual double MaxCurvature () const 
    {
      return seg->MaxCurvature();
    }

    virtual string GetType () const
    {
      return seg->GetType();
    }

    virtual double CalcCurvature (double t) const
    {
      Point<2> point;
      Vec<2> first, second;
      GetDerivatives (t, point, first, second);
      double curv = fabs(first(0)*second(1)-first(1)*second(0)) / pow(first.Length(), 3);
      return curv;
    }

    virtual bool InConvexHull (Point<2> p, double eps) const
    {
      return seg->InConvexHull (p, eps);
    }

  };




  class SplineGeometry2d : public SplineGeometry<2>, public NetgenGeometry
  {
  protected:
    Array<char*> materials;
    Array<double> maxh;
    Array<bool> quadmeshing;
    Array<bool> tensormeshing;
    Array<int> layer;
    Array<string*> bcnames;
    double elto0 = 1.0;


  public:
    DLL_HEADER virtual ~SplineGeometry2d();

    DLL_HEADER void Load (istream& infile);
    DLL_HEADER void Load (const char * filename);
    DLL_HEADER void LoadFromString (const std::string& str);

    DLL_HEADER void LoadData( istream & infile );
    DLL_HEADER void LoadDataNew ( istream & infile );
    DLL_HEADER void LoadDataV2 ( istream & infile );

    void TestComment ( istream & infile ) ;

    void DoArchive(Archive& ar)
    {
      SplineGeometry<2>::DoArchive(ar);
      ar & materials & maxh & quadmeshing & tensormeshing & layer & bcnames & elto0;
    }

    const SplineSegExt & GetSpline (const int i) const 
    { 
      return dynamic_cast<const SplineSegExt&> (*splines[i]);
    }

    SplineSegExt & GetSpline (const int i) 
    { 
      return dynamic_cast<SplineSegExt&> (*splines[i]);
    }

    
    DLL_HEADER virtual int GenerateMesh (shared_ptr<Mesh> & mesh, MeshingParameters & mparam);
    
    void PartitionBoundary (MeshingParameters & mp, double h, Mesh & mesh2d);

    void CopyEdgeMesh (int from, int to, Mesh & mesh2d, Point3dTree & searchtree);


    void GetMaterial (int  domnr, char* & material );
    void SetMaterial (int  domnr, const string & material);

    double GetDomainMaxh ( const int domnr );
    void SetDomainMaxh ( const int domnr, double maxh );
    
    bool GetDomainQuadMeshing ( int domnr ) 
    { 
      if ( quadmeshing.Size() ) return quadmeshing[domnr-1]; 
      else return false;
    }
    bool GetDomainTensorMeshing ( int domnr ) 
    { 
      if ( tensormeshing.Size() ) return tensormeshing[domnr-1]; 
      else return false;
    }
    int GetDomainLayer ( int domnr ) 
    { 
      if ( layer.Size() ) return layer[domnr-1]; 
      else return 1;
    }


    string GetBCName (int bcnr) const;
    void SetBCName (int bcnr, string name);
    int GetBCNumber (string name) const; // 0 if not exists
    int AddBCName (string name);

    string * BCNamePtr ( const int bcnr );

    
    DLL_HEADER virtual Refinement & GetRefinement () const; 
  };
}







#endif
