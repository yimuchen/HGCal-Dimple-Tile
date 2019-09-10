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

  // Get set functions of geometric variables.
  inline void
  SetWrapping ( const G4bool x ){_wrapping = x;}
  inline G4bool
  GetWrapping () const {return _wrapping;}

  // Set ref index of scint pmt gap
  inline void
  SetRefIndex ( const G4double iRefIndex ){RefIndex = iRefIndex;}
  inline G4double
  GetRefIndex() const {return RefIndex;}

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

  inline void
  SetScintPMTGapThickness ( const G4double x ){ScintPMT_gap = x;}
  inline G4double
  GetScintPMTGapThickness() const {return ScintPMT_gap;}

  inline void
  SetTileAbsLength ( const G4double x ){tileAbsLength = x;}
  inline G4double
  GetTileAbsLength() const {return tileAbsLength;}

  inline void
  SetInducedMuTile( const G4double x ){inducedMuTile = x;}
  inline G4double
  GetInducedMuTile() const {return inducedMuTile;}

  // G
  inline double
  GetDimpleRadius() const { return _dimple_radius; }
  inline void
  SetDimpleRadius( const double x ){ _dimple_radius = x ; }
  inline double
  GetDimpleIndent() const { return _dimple_indent; }
  inline void
  SetDimpleIndent( const double x ){ _dimple_indent = x ; }

  int
  GetDimpleType() const {return _dimple_type; }
  inline void
  SetDimpleType( const int x ){ _dimple_type = x ; }

  enum  DIMPLE_TYPE {
    SPHERICAL ,
    PARABOLIC ,
    PYRAMID
  };

  // Additional geometric factors.
  double WorldHalfX() const ;
  double WorldHalfY() const ;
  double WorldHalfZ() const ;

  double GetDimpleSizeRadius() const;
  double LocalTileZ( const double x, const double y ) const ;

private:
  // Subfunctions for cleaner code
  G4VSolid*          ConstructTileSolid(
    const G4String& name,
    G4double        angle1, // angle measured ccw from y axis for the side at -x
    G4double        angle2, // angle measured ccw from y axis for the side at +x
    G4double        Dx2,    // length along x of side at y=+Dy
    G4double        Dy,     // length along y
    G4double        Dz,     // length along z
    G4ThreeVector&  centerCoord ); // coordinate of center of gravity w.r.t.
                                   // corner 0 at -x, -y

  G4VSolid*     ConstructSphereDimpleSolid() const;
  G4VSolid*     ConstructPyramidDimpleSolid() const;
  G4VSolid*     ConstructParabolicDimpleSolid() const;
  G4ThreeVector CalcDimpleOffset() const;

  // Pointer to detector messenger class
  LYSimDetectorMessenger* fdetectorMessenger;

  G4bool fUpdated;

  // Contains the minimum and maximum Z-position of the previously defined volume
  // Use this variable to define volume successively along the z-axis
  // (max for +z direction, min for -z direction)
  // Set value after defining a physical volume

  // Pointers to main volumes
  G4Box* solidWorld;
  G4LogicalVolume* logicWorld;
  G4VPhysicalVolume* physWorld;
  G4VPhysicalVolume* physTile;

  // pointer for wrapping
  G4LogicalVolume* logicWWW;
  // pointer for dimple
  G4LogicalVolume* logicD;

  // Pointers to materials
  G4Material* fAir;
  G4Material* fSCSN81;
  G4Material* fEJ200;
  G4Material* fEJ260;
  G4Material* fBialkali;
  G4Material* fEpoxy;
  G4Material* fEpoxy2;

  // Pointers to surfaces
  G4OpticalSurface* fTyvekOpSurface;
  G4OpticalSurface* fIdealTyvekOpSurface;
  G4OpticalSurface* fUnifiedTyvekOpSurface;
  G4OpticalSurface* fUnifiedIdealTyvekOpSurface;
  G4OpticalSurface* fESROpSurface;
  G4OpticalSurface* fPolishedOpSurface;
  G4OpticalSurface* fIdealPolishedOpSurface;
  G4OpticalSurface* fMirrorOpSurface;
  G4OpticalSurface* fIdealMirrorOpSurface;
  G4OpticalSurface* fAbsorbingOpSurface;
  G4OpticalSurface* fRoughOpSurface;
  G4OpticalSurface* fSiPMSurface;


  // Pointers for access to Sensitive Detector
  static LYSimPMTSD* fPMTSD;

  // Geometry parameters
  // Important parameters marked with ////
  G4double RefIndex;// Refractive index of fiber or scint-PMT gap
  G4double angle1;
  G4double angle2;
  G4double angle3;
  G4double Dx2;
  G4double Dx3;
  G4double Dw2;

  double _tilex;
  double _tiley;
  double _tilez;

  G4bool _wrapping;// (Tyvek) wrapping around scintillator
  G4double Photocat_sizeX;
  G4double Photocat_sizeY;
  G4double Photocat_thickness;
  G4double ScintPMT_gap;// thickness of gap between scintillator and PMT
  G4double wrapgap;// distance between tile and wrapping
  G4double SiPM_Depth;// SiPM depth


  G4double tileAbsLength;
  G4double inducedMuTile;// Radiation-induced absorption coefficient (cm^-1)
  G4double Absmultiple;// factor for Abslength manipulation.

  // Dimple variables;
  G4double _dimple_indent;// Depth of dimple
  G4double _dimple_radius;// Dimple radius
  G4int _dimple_type;

  G4ThreeVector corners[8];// stores the corner coordinates of the tile
  G4ThreeVector readout0, readout1;// location of readout at corner 0/1
  // G4ThreeVector mirror0, mirror1;     //location of mirrors at corner 0/1
  G4int readoutCorner;// determines location of fiber readout (0:corner 0, 1:corner 1)
};

#endif
