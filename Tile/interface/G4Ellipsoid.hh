#ifndef G4Ellipsoid_HH
#define G4Ellipsoid_HH

#include <CLHEP/Units/PhysicalConstants.h>

#include "G4Polyhedron.hh"
#include "G4VSolid.hh"

class G4Ellipsoid : public G4VSolid
{
public:    // with description

  G4Ellipsoid( const G4String& pName,
               G4double        pxSemiAxis,
               G4double        pySemiAxis,
               G4double        pzSemiAxis,
               G4double        pzBottomCut = 0,
               G4double        pzTopCut    = 0 );

  virtual ~G4Ellipsoid();

  // Access functions

  inline G4double GetSemiAxisMax( G4int i ) const;
  inline G4double GetZBottomCut() const;
  inline G4double GetZTopCut() const;
  inline void     SetSemiAxis( G4double x, G4double y, G4double z );
  inline void     SetZCuts( G4double newzBottomCut, G4double newzTopCut );
  inline G4double GetCubicVolume();
  inline G4double GetSurfaceArea();

  // Solid standard methods

  void ComputeDimensions( G4VPVParameterisation*   p,
                          const G4int              n,
                          const G4VPhysicalVolume* pRep );

  void   BoundingLimits( G4ThreeVector& pMin, G4ThreeVector& pMax ) const;
  G4bool CalculateExtent( const EAxis pAxis,
                          const G4VoxelLimits& pVoxelLimit,
                          const G4AffineTransform& pTransform,
                          G4double& pmin, G4double& pmax ) const;
  EInside       Inside( const G4ThreeVector& p ) const;
  G4ThreeVector SurfaceNormal( const G4ThreeVector& p ) const;
  G4double      DistanceToIn( const G4ThreeVector& p,
                              const G4ThreeVector& v ) const;
  G4double DistanceToIn( const G4ThreeVector& p ) const;
  G4double DistanceToOut( const G4ThreeVector& p,
                          const G4ThreeVector& v,
                          const G4bool         calcNorm = G4bool(false),
                          G4bool*              validNorm = 0,
                          G4ThreeVector*       n         = 0 ) const;
  G4double DistanceToOut( const G4ThreeVector& p ) const;

  G4GeometryType GetEntityType() const;

  G4VSolid* Clone() const;

  std::ostream& StreamInfo( std::ostream& os ) const;

  G4ThreeVector GetPointOnSurface() const;

  // Visualisation functions

  G4Polyhedron* GetPolyhedron() const;
  void          DescribeYourselfTo( G4VGraphicsScene& scene ) const;
  G4VisExtent   GetExtent() const;
  G4Polyhedron* CreatePolyhedron() const;

public:    // without description

  G4Ellipsoid( __void__& );
  // Fake default constructor for usage restricted to direct object
  // persistency for clients requiring preallocation of memory for
  // persistifiable objects.

  G4Ellipsoid( const G4Ellipsoid& rhs );
  G4Ellipsoid& operator=( const G4Ellipsoid& rhs );
  // Copy constructor and assignment operator.

protected:    // without description

  mutable G4bool fRebuildPolyhedron;
  mutable G4Polyhedron* fpPolyhedron;

private:

  G4double kRadTolerance;
  G4double halfCarTolerance, halfRadTolerance;

  G4double fCubicVolume;
  G4double fSurfaceArea;
  G4double xSemiAxis, ySemiAxis, zSemiAxis,
           semiAxisMax, zBottomCut, zTopCut;
};

#include "G4Ellipsoid.icc"

#endif
