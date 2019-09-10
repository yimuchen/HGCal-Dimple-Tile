#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorMessenger.hh"
#include "HGCalTileSim/Tile/interface/Materials.hh"
#include "HGCalTileSim/Tile/interface/SurfaceProperty.hh"
#else
#include "LYSimDetectorConstruction.hh"
#include "LYSimDetectorMessenger.hh"
#include "Materials.hh"
#include "SurfaceProperty.hh"
#endif

#include <math.h>

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"
#include "G4GeometryManager.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Orb.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RegionStore.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"

using std::cos;
using std::sin;
using std::tan;
using std::atan;
using std::exp;
using namespace CLHEP;


// static Helper methods for computed extended geometry dimension.



LYSimPMTSD* LYSimDetectorConstruction::fPMTSD = NULL;

LYSimDetectorConstruction::LYSimDetectorConstruction()
  : G4VUserDetectorConstruction()
{
  fdetectorMessenger = new LYSimDetectorMessenger( this );

  // Set default parameters
  _wrapping = true;

  angle1 = 0*degree;
  angle2 = 0*degree;// for tile
  angle3 = 0*degree;// for wrap
  Dx2    = 0*mm;
  Dx3    = 0*mm;
  Dw2    = 0*mm;

  _tilex = 29.65*mm;
  _tiley = 29.65*mm;
  _tilez = 2.98*mm;// 3.7*mm for SCSN81

  Photocat_sizeX     = 1.3*mm;
  Photocat_sizeY     = 1.3*mm;
  Photocat_thickness = 0.025*mm;// arbitrary thickness for Photocathode
  ScintPMT_gap       = 2.0*mm;// rough estimate of air gap between scintillator and PMT face
  wrapgap            = 0.05*mm;


  tileAbsLength = 380*cm;
  inducedMuTile = 1.e-20/cm;
  readoutCorner = 1;

  Absmultiple    = 1.0;// factor for Abslength manipulation.
  _dimple_indent = 1.25*1.6*mm;
  _dimple_radius = 3.5*mm;// 3.4409*mm
  SiPM_Depth     = 0.0*mm;// SiPM Depth (0.0 is flush with top of tile, don't use anything less than 0.05*mm unless 0.0)
  // Dimple radius (normal is 3.4409*mm)
  // Dimple depth (normal is 1.6*mm)

  _dimple_type = 0;// 0: Normal, 1: Pyramid, 2: Parabolic
  solidWorld   = NULL;
  logicWorld   = NULL;
  physWorld    = NULL;
  logicWWW     = NULL;

  // Defining material list.
  fBialkali = Make_Bialkali();
  fEpoxy    = Make_Epoxy();
  fEpoxy2   = Make_Epoxy2();
  fAir      = Make_Custom_Air();

  fSCSN81 = Make_SCSN81( GetTileAbsLength(), GetInducedMuTile() );
  fEJ200  = Make_EJ200( Absmultiple );
  fEJ260  = Make_EJ260( GetTileAbsLength(), GetInducedMuTile() );

  fTyvekOpSurface             = MakeS_TyvekCrystal();
  fIdealTyvekOpSurface        = MakeS_IdealTyvekCrystal();
  fUnifiedTyvekOpSurface      = MakeS_UnifiedTyvek();
  fUnifiedIdealTyvekOpSurface = MakeS_IdealTyvek();
  fESROpSurface               = MakeS_ESR();
  fRoughOpSurface             = MakeS_Rough();
  fPolishedOpSurface          = MakeS_Polished();
  fIdealPolishedOpSurface     = MakeS_IdealPolished();
  fMirrorOpSurface            = MakeS_Mirror();
  fIdealMirrorOpSurface       = MakeS_IdealMirror();
  fSiPMSurface                = MakeS_SiPM();
  fUpdated                    = false;
}


void
LYSimDetectorConstruction::UpdateGeometry()
{
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();

  G4RunManager::GetRunManager()->DefineWorldVolume( Construct() );
  G4cout << "[LYSim] Setting induced absorption coefficient = "
         << inducedMuTile << " [cm^-1]" << G4endl;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

LYSimDetectorConstruction::~LYSimDetectorConstruction()
{
  if( fdetectorMessenger ){ delete fdetectorMessenger; }
}

G4VPhysicalVolume*
LYSimDetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;

  // World
  G4Box* solidWorld = new G4Box( "World", WorldHalfX()
                               , WorldHalfY(), WorldHalfZ() );

  G4LogicalVolume* logicWorld = new G4LogicalVolume( solidWorld, fAir, "World" );

  G4VPhysicalVolume* physWorld = new G4PVPlacement( 0
                                                  , G4ThreeVector( 0, 0, 0 )
                                                  , logicWorld
                                                  , "World"
                                                  , 0
                                                  , false
                                                  , 0
                                                  , checkOverlaps );
  ///////////////////////////////////////////////
  // wrapping
  /////////////////////////////////////////////////
  Dx2 = _tilex-tan( angle1 )*_tiley-tan( angle2 )*_tiley;
  Dx3 = _tilex-tan( angle1 )*_tiley-tan( angle3 )*_tiley;
  Dw2 = _tilex-tan( angle1 )*_tiley-tan( angle3 )*_tiley;
  G4double xdisp  = 0.25*( _tilex-Dx2 );
  G4double xdisp2 = 0.25*( _tilex-Dx3 );

  G4ThreeVector wrapCenter( 0, 0, 0 );


  G4VSolid* solidWrap = ConstructTileSolid( "TileTrap"
                                          , angle1
                                          , angle3
                                          , Dw2+2*wrapgap
                                          , _tiley+2*wrapgap
                                          , _tilez+2*wrapgap
                                          , wrapCenter   );


  G4LogicalVolume* logicWrap = new G4LogicalVolume( solidWrap,   fAir,  "Wrap" );

  G4ThreeVector WrapOffset( -xdisp2, 0, 0 );

  G4VPhysicalVolume* physWrap = new G4PVPlacement( 0
                                                 , WrapOffset
                                                 , logicWrap
                                                 , "Wrap"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );


  ///////////////////////////////////////////////////////
  // Subtracted Dimple Version (dimple sub from tile, WWW = mothervolume of both)
  /////////////////////////////////////////////////////////
  G4ThreeVector tileCenter( 0, 0, 0 );
  G4ThreeVector TileOffset( 0, 0, 0 );// -xdisp if weak
  G4ThreeVector DimpleOffset = CalcDimpleOffset();

  G4VSolid* solidTile = ConstructTileSolid( "TileTrap"
                                          , angle1
                                          , angle2
                                          , Dx2
                                          , _tiley
                                          , _tilez
                                          , tileCenter );

  G4LogicalVolume* logicTile = new G4LogicalVolume( solidTile
                                                  , fEJ200, "TileLogic" );

  G4VPhysicalVolume* physTile = new G4PVPlacement( 0
                                                 , TileOffset
                                                 , logicTile
                                                 , "TilePhysic"
                                                 , NULL
                                                 , false
                                                 , 0
                                                 , checkOverlaps );


  G4VSolid* solidDimple = _dimple_type == 1 ? ConstructPyramidDimpleSolid() :
                          _dimple_type == 2 ? ConstructParabolicDimpleSolid() :
                          ConstructSphereDimpleSolid();

  G4LogicalVolume* logicDimple = new G4LogicalVolume( solidDimple
                                                    , fAir, "Dimple" );

  G4VPhysicalVolume* DimpleRod = new G4PVPlacement( 0
                                                  , DimpleOffset
                                                  , logicDimple
                                                  , "Dimple"
                                                  , logicTile
                                                  , false
                                                  , 0
                                                  , checkOverlaps );


  ////////////////////////////////////////////
  // SiPM
  ////////////////////////////////////////////
  const double SubD      = GetDimpleSizeRadius() - GetDimpleIndent();
  const double backthick = 0.025*mm;

  const double case_z = SiPM_Depth == 0*mm ?  0.5*_tilez+0.025*mm :
                        -SubD+0.025*mm-SiPM_Depth;
  const double sipm_z = SiPM_Depth == 0*mm ?
                        0.5*_tilez+0.5*Photocat_thickness :
                        -SubD+0.5*Photocat_thickness-SiPM_Depth;

  G4ThreeVector CaseOffset( xdisp2, 0, case_z );
  G4ThreeVector transSiPM( xdisp2, 0, sipm_z );

  G4Box* solidSiPM = new G4Box( "SiPM",  0.5*Photocat_sizeX
                              , 0.5*Photocat_sizeY, 0.5*Photocat_thickness );

  G4LogicalVolume* logicSiPM = new G4LogicalVolume( solidSiPM
                                                  , fBialkali,  "SiPM" );

  G4Box* solidSiPMback = new G4Box( "SiPMBack"
                                  , 0.5*2.1*mm, 0.5*2.1*mm, backthick );

  G4SubtractionSolid* SiPMCase
    = new G4SubtractionSolid( "SiPMCase"
                            , solidSiPMback
                            , solidSiPM
                            , 0
                            , G4ThreeVector( 0, 0, 0.5*Photocat_thickness-backthick ) );

  G4LogicalVolume* logicSiPMCase = new G4LogicalVolume( SiPMCase
                                                      , fEpoxy, "SiPMBack" );

  G4VPhysicalVolume* SiPMCasing = new G4PVPlacement( 0
                                                   , CaseOffset
                                                   , logicSiPMCase
                                                   , "Case"
                                                   , logicWrap
                                                   , false
                                                   , 0
                                                   , checkOverlaps );

  G4VPhysicalVolume* physSiPM = new G4PVPlacement( 0
                                                 , transSiPM
                                                 , logicSiPM
                                                 , "SiPM"
                                                 , logicWrap
                                                 , false
                                                 , 0
                                                 , checkOverlaps );
  // Defining surfaces
  G4LogicalBorderSurface* WrapAirSurface =
    new G4LogicalBorderSurface( "WrapAirSurface"
                              , physWrap
                              , physWorld
                              , fESROpSurface );
  G4LogicalBorderSurface* TileSurface =
    new G4LogicalBorderSurface( "TileSurface"
                              , physTile
                              , physWorld
                              , fPolishedOpSurface );
  G4LogicalBorderSurface* DimpleAirSurface =
    new G4LogicalBorderSurface( "DimpleSurface"
                              , DimpleRod
                              , physTile
                              , fIdealPolishedOpSurface );
  G4LogicalSkinSurface* CaseSurface
    = new G4LogicalSkinSurface( "SiPMCaseSurface"
                              , logicSiPMCase
                              , fIdealMirrorOpSurface );
  // Must be declared again (NOT Sure why)
  fSiPMSurface = MakeS_SiPM();
  G4LogicalSkinSurface* SiPMSurface = new G4LogicalSkinSurface( "SiPMSurface"
                                                              , logicSiPM
                                                              , fSiPMSurface );

  // Setting the sensitive detector
  if( !fPMTSD ){
    fPMTSD = new LYSimPMTSD( "/LYSimPMT" );
    G4SDManager* sdman = G4SDManager::GetSDMpointer();
    sdman->AddNewDetector( fPMTSD );
  }

  logicSiPM->SetSensitiveDetector( fPMTSD );

  // Visual attributes
  logicWorld->SetVisAttributes( G4VisAttributes::Invisible );

  G4VisAttributes* SiPMVisAtt = new G4VisAttributes( G4Colour( 1., 1., 1. ) );
  SiPMVisAtt->SetForceWireframe( false );
  SiPMVisAtt->SetVisibility( true );
  logicSiPM->SetVisAttributes( SiPMVisAtt );

  G4VisAttributes* CaseVisAtt = new G4VisAttributes( G4Colour( 0.5, 0.5, 0.5 ) );
  CaseVisAtt->SetForceSolid( true );
  CaseVisAtt->SetVisibility( true );
  logicSiPMCase->SetVisAttributes( CaseVisAtt );

  G4VisAttributes* DimpleVisAtt = new G4VisAttributes( G4Colour( 0., 1., 1. ) );
  DimpleVisAtt->SetForceWireframe( true );
  DimpleVisAtt->SetVisibility( true );
  logicDimple->SetVisAttributes( DimpleVisAtt );

  G4VisAttributes* TileVisAtt = new G4VisAttributes( G4Colour( 1., 1., 0. ) );
  TileVisAtt->SetForceWireframe( true );
  TileVisAtt->SetVisibility( true );
  logicTile->SetVisAttributes( TileVisAtt );

  G4VisAttributes* WrapVisAtt = new G4VisAttributes( G4Colour( 0., 1., 0. ) );
  WrapVisAtt->SetForceWireframe( true );
  WrapVisAtt->SetVisibility( true );
  logicWrap->SetVisAttributes( WrapVisAtt );

  return physWorld;
}


G4VSolid*
LYSimDetectorConstruction::ConstructTileSolid ( const G4String& name,
                                                G4double        angle1,
                                                G4double        angle2,
                                                G4double        Dx2,
                                                G4double        Dy,
                                                G4double        Dz,
                                                G4ThreeVector&  center )
{
  G4double x2offset = -Dy* tan( angle1 );
  // G4double Dx1 = Dx2 + x2offset;
  G4double Dx1 = Dx2 + x2offset + Dy*tan( angle2 );

  G4ThreeVector centerOfGravity( 0, 0, 0 );
  corners[0] = G4ThreeVector( 0., 0., 0. );
  corners[1] = G4ThreeVector( Dx1,          0., 0. );
  corners[2] = G4ThreeVector( x2offset,     Dy, 0. );
  corners[3] = G4ThreeVector( Dx2+x2offset, Dy, 0. );
  corners[4] = G4ThreeVector( 0.,           0., Dz );
  corners[5] = G4ThreeVector( Dx1,          0., Dz );
  corners[6] = G4ThreeVector( x2offset,     Dy, Dz );
  corners[7] = G4ThreeVector( Dx2+x2offset, Dy, Dz );

  for( int i = 0; i < 8; i++ ){
    centerOfGravity += corners[i];
  }

  centerOfGravity /= 8;

  for( int i = 0; i < 8; i++ ){
    corners[i] -= centerOfGravity;
  }

  center = centerOfGravity;

  G4VSolid* solidTile = new G4Trap( name, corners );

  return solidTile;
}

G4VSolid*
LYSimDetectorConstruction::ConstructSphereDimpleSolid() const
{
  return new G4Sphere( "DimpleSphere"
                     , GetDimpleRadius() - GetDimpleIndent()// r_min
                     , GetDimpleSizeRadius()// r_max
                     , 0.,  2.*pi// Theta
                     , pi/2+std::asin( GetDimpleRadius()/GetDimpleSizeRadius() )
                     , pi );
}

G4VSolid*
LYSimDetectorConstruction::ConstructParabolicDimpleSolid() const
{
  const G4double SemiZ = ( 447317/288000 )*mm + _dimple_indent;
  const G4double SemiX = ( 447317/120000 )*mm;
  const G4double SemiY = SemiX;

  return new G4Ellipsoid( "DimpleParabolic",
    SemiX,
    SemiY,
    SemiZ,
    -SemiZ,// must be < SemiZ
    -SemiZ+_dimple_indent );// must be >-SemiZ
}

G4VSolid*
LYSimDetectorConstruction::ConstructPyramidDimpleSolid() const
{
  return new G4Trd( "DimplePyramid",
    0.0*mm, _dimple_radius,
    0.0*mm, _dimple_radius,
    _dimple_indent*0.5 );
}

double
LYSimDetectorConstruction::GetDimpleSizeRadius() const
{
  return 0.5*( ( _dimple_radius*_dimple_radius )/_dimple_indent
               + _dimple_indent );
}

G4ThreeVector
LYSimDetectorConstruction::CalcDimpleOffset() const
{
  if( _dimple_type == 1 ){
    return G4ThreeVector( 0, 0, 0.5*_tilez-0.5*_dimple_indent );
  } else if( _dimple_type == 2 ){
    return G4ThreeVector( 0, 0, 0.5*_tilez+( 447317/288000 )*mm );
  } else {
    const double xdisp = 0.25*( _tilex-Dx2 );
    const double zdisp = 0.5*_tilez
                         +GetDimpleSizeRadius()-GetDimpleIndent();
    return G4ThreeVector( xdisp, 0, zdisp );
  }
}

// Additional geometry factors
double
LYSimDetectorConstruction::WorldHalfX() const
{
  return _tilex * 1.5;
}

double
LYSimDetectorConstruction::WorldHalfY() const
{
  return _tiley *1.5;
}

double
LYSimDetectorConstruction::WorldHalfZ() const
{
  return _tilez * 10;
}

double
LYSimDetectorConstruction::LocalTileZ( const double x, const double y ) const
{
  const double dimple_r  = GetDimpleRadius();
  const double dimple_i  = GetDimpleIndent();
  const double beam_r    = sqrt( x*x+y*y );
  const double big_r     = GetDimpleSizeRadius();
  const double thickness = GetTileZ();
  const int type         = GetDimpleType();

  if( beam_r <= dimple_r ){
    if( type == SPHERICAL ){
      return thickness - ( sqrt( big_r*big_r - beam_r*beam_r ) -
                           ( big_r - dimple_i ) );
    } else if( type == PARABOLIC ){//  # Parabolic
      return thickness - abs( 1.35 * beam_r*beam_r - dimple_i );
    } else if( type == PYRAMID ){
      if( fabs( y ) <= fabs( x ) ){
        return thickness - ( dimple_i - ( dimple_i / dimple_r ) * fabs( x ) );
      } else {
        return thickness - ( dimple_i - ( dimple_i / dimple_r ) * fabs( y ) );
      }
    }  else {
      return thickness;
    }
  } else {
    return thickness;
  }
}
