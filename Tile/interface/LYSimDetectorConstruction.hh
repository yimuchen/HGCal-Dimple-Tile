#ifndef LYSimDetectorConstruction_h
#define LYSimDetectorConstruction_h 1

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimPMTSD.hh"
#else
#include "LYSimPMTSD.hh"
#endif

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class LYSimDetectorMessenger;
class G4Box;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;
class LYSimPMTSD;

class LYSimDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  LYSimDetectorConstruction();
  virtual ~LYSimDetectorConstruction();
  virtual G4VPhysicalVolume* Construct();
  void                       UpdateGeometry();

  // Get/Set functions of tile geometry parameters
  inline void
  SetTileZ ( const G4double x ){ _tilez = x;}
  inline G4double
  GetTileZ() const {return _tilez;}
  inline void
  SetTileX ( const G4double x ){_tilex = x;}
  inline G4double
  GetTileX() const {return _tilex;}
  inline void
  SetTileY ( const G4double y ){_tiley = y;}
  inline G4double
  GetTileY() const {return _tiley;}

  // Get/Set functions of dimple geometry paramters
  inline double
  GetDimpleRadius() const { return _dimple_radius; }
  inline void
  SetDimpleRadius( const double x ){ _dimple_radius = x; }
  inline double
  GetDimpleIndent() const { return _dimple_indent; }
  inline void
  SetDimpleIndent( const double x ){ _dimple_indent = x; }
  int
  GetDimpleType() const {return _dimple_type; }
  inline void
  SetDimpleType( const int x ){ _dimple_type = x; }

  enum  DIMPLE_TYPE
  {
    SPHERICAL,
    PARABOLIC,
    PYRAMID
  };

  // Additional geometric factors.
  double WorldHalfX() const;
  double WorldHalfY() const;
  double WorldHalfZ() const;

  double GetDimpleSizeRadius() const;
  double LocalTileZ( const double x, const double y ) const;

  // Material updating functions
  void SetTileAbsMult( const double x );
  inline double
  GetTileAbsMult() const { return _absmult; }

  void SetWrapReflect( const double x );
  inline double
  GetWrapReflect() const { return _wrap_reflect; }

  // Accesing Custom Materials
  inline G4Material* GetEJ200(){ return fEJ200; }
  inline G4OpticalSurface* GetSiPMSurface(){ return fSiPMSurface; }

private:
  // Subfunctions for cleaner code
  G4VSolid* ConstructTrapazoidSolid(
    const G4String& name,
    double        x, // Maximum Length along z direction.
    double        y, // Length along y direction
    double        z, // length along z direction
    double        indent_x1, // Indent in x direction
    double        indent_x2  // indent of corner
  ) const;

  G4VSolid* ConstructHollowWrapSolid() const;

  G4VSolid*     ConstructSphereDimpleSolid() const;
  G4VSolid*     ConstructPyramidDimpleSolid() const;
  G4VSolid*     ConstructParabolicDimpleSolid() const;

  G4ThreeVector CalcDimpleOffset() const;
  G4ThreeVector CalcSiPMDimpleOffset() const;

  // Pointer to detector messenger class
  LYSimDetectorMessenger* fdetectorMessenger;

  // Pointers to materials
  G4Material* fAir;
  G4Material* fEJ200;
  G4Material* fBialkali;
  G4Material* fEpoxy;
  G4Material* fEpoxy2;

  // Pointers to surfaces
  G4OpticalSurface* fESROpSurface;
  G4OpticalSurface* fPolishedOpSurface;
  G4OpticalSurface* fIdealPolishedOpSurface;
  G4OpticalSurface* fIdealMirrorOpSurface;
  G4OpticalSurface* fSiPMSurface;

  // Pointers for access to Sensitive Detector
  static LYSimPMTSD* fPMTSD;

  // Tile Geometric definitions
  double _tilex;
  double _tiley;
  double _tilez;
  double _tile_x1;
  double _tile_x2;

  // Wrapping Geometry definitions
  double wrapgap;// distance between tile and wrapping

  // Absorption length multiplier
  double _absmult;
  double _wrap_reflect;

  // Dimple variables;
  double _dimple_indent;// Depth of dimple
  double _dimple_radius;// Dimple radius
  int _dimple_type;

  double _sipm_x;
  double _sipm_y;
  double _sipm_z;
  double _sipm_depth;// SiPM depth
  double _sipm_rimwidth;
};

#endif
