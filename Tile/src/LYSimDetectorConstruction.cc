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

LYSimPMTSD* LYSimDetectorConstruction::fPMTSD = NULL;

LYSimDetectorConstruction::LYSimDetectorConstruction()
  : G4VUserDetectorConstruction()
{
  fdetectorMessenger = new LYSimDetectorMessenger( this );

  _tilex   = 29.65*mm;
  _tiley   = 29.65*mm;
  _tilez   = 2.98*mm;
  _tile_x1 = 0.0*mm;
  _tile_x2 = 0.0*mm;
  wrapgap  = 0.1*mm;

  _absmult      = 1;
  _wrap_reflect = 1;

  _sipm_x        = 1.3*mm;
  _sipm_y        = 1.3*mm;
  _sipm_z        = 0.3*mm;// arbitrary thickness for Photocathode
  _sipm_depth    = 0.0*mm;
  _sipm_rimwidth = 0.1*mm;

  // Default Dimple settings
  _dimple_type   = SPHERICAL;// 0: Normal, 1: Pyramid, 2: Parabolic
  _dimple_indent = 1.25*1.6*mm;
  _dimple_radius = 3.5*mm;// 3.4409*mm

  // Defining material list.
  fBialkali = Make_Bialkali();
  fEpoxy    = Make_Epoxy();
  fEpoxy2   = Make_Epoxy2();
  fAir      = Make_Custom_Air();
  fEJ200    = Make_EJ200();
  SetTileAbsMult( _absmult );

  // Defining surface list.
  fESROpSurface           = MakeS_Rough();
  fPolishedOpSurface      = MakeS_Polished();
  fIdealPolishedOpSurface = MakeS_IdealPolished();
  fIdealMirrorOpSurface   = MakeS_IdealMirror();
  fSiPMSurface            = MakeS_SiPM();
  SetWrapReflect( _wrap_reflect );
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
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

LYSimDetectorConstruction::~LYSimDetectorConstruction()
{
  if( fdetectorMessenger ){ delete fdetectorMessenger; }
}

G4VPhysicalVolume*
LYSimDetectorConstruction::Construct()
{
  static const bool checkOverlaps = true;

  ///////////////////////////////////////////////////////////////////////////////
  // World volume
  ///////////////////////////////////////////////////////////////////////////////
  std::cout << "Constructing World Volume" << std::endl;
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
  ///////////////////////////////////////////////////////////////////////////////
  // wrapping
  ///////////////////////////////////////////////////////////////////////////////
  std::cout << "Constructing Wrapping stuff" << std::endl;
  G4VSolid* solidWrap = ConstructHollowWrapSolid();

  // The matrial of the wrap isn't as important as the surface
  G4LogicalVolume* logicWrap = new G4LogicalVolume( solidWrap, fEpoxy,  "Wrap" );

  G4VPhysicalVolume* physWrap = new G4PVPlacement( 0
                                                 , G4ThreeVector( 0, 0, 0 )
                                                 , logicWrap
                                                 , "Wrap"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // Subtracted Dimple Version (dimple sub from tile, WWW = mothervolume of both)
  ///////////////////////////////////////////////////////////////////////////////
  std::cout << "Constructing Solid physical tile" << std::endl;

  G4VSolid* solidTile = ConstructTrapazoidSolid( "TileTrap"
                                               , _tilex
                                               , _tiley
                                               , _tilez
                                               , _tile_x1
                                               , _tile_x2 );

  G4VSolid* solidTileSubtract = new G4Sphere( "TileSub", 0, GetDimpleSizeRadius()
                                            , 0, 2* pi, 0, pi );

  // G4VSolid* solidDimple
  //  = _dimple_type == PYRAMID ? ConstructPyramidDimpleSolid() :
  //    _dimple_type == PARABOLIC ? ConstructParabolicDimpleSolid() :
  //    ConstructSphereDimpleSolid();

  G4VSolid* hollowedTile = new G4SubtractionSolid( "TileSolid"
                                                 , solidTile, solidTileSubtract
                                                 , 0, CalcDimpleOffset() );

  G4LogicalVolume* logicTile = new G4LogicalVolume( hollowedTile
                                                  , fEJ200, "TileLogic" );

  G4VPhysicalVolume* physTile = new G4PVPlacement( 0
                                                 , G4ThreeVector( 0, 0, 0 )
                                                 , logicTile
                                                 , "TilePhysic"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // SiPM
  ///////////////////////////////////////////////////////////////////////////////
  std::cout << "Constructing Solid SiPM" << std::endl;

  static const double sipm_sensor_z = 0.0025*mm;

  G4Box* solidSiPMInner = new G4Box( "SiPMInner"
                                   , 0.5*_sipm_x, 0.5*_sipm_y, sipm_sensor_z );

  G4Box* solidSiPMOuter = new G4Box( "SiPMOuter"
                                   , 0.5*_sipm_x + _sipm_rimwidth
                                   , 0.5*_sipm_y + _sipm_rimwidth
                                   , 0.5*_sipm_z );

  G4VSolid* solidSiPMCase
    = new G4SubtractionSolid( "SiPMCase"
                            , solidSiPMOuter, solidSiPMInner
                            , 0
                            , G4ThreeVector( 0, 0
                                           , -0.5*_sipm_z + sipm_sensor_z ) );

  G4LogicalVolume* logicSiPM = new G4LogicalVolume( solidSiPMInner
                                                  , fBialkali,  "SiPM" );

  G4LogicalVolume* logicSiPMCase = new G4LogicalVolume( solidSiPMCase
                                                      , fEpoxy, "SiPMBack" );

  const double caseoffset_z = +0.5*_tilez - 0.5 * _sipm_z - _sipm_depth;
  const double sipmoffset_z = +0.5*_tilez - _sipm_z + sipm_sensor_z
                              - _sipm_depth;
  const G4ThreeVector CaseOffset( 0, 0, caseoffset_z );
  const G4ThreeVector SiPMOffset( 0, 0, sipmoffset_z );

  G4VPhysicalVolume* physSiPMCase = new G4PVPlacement( 0
                                                     , CaseOffset
                                                     , logicSiPMCase
                                                     , "Case"
                                                     , logicWorld
                                                     , false
                                                     , 0
                                                     , checkOverlaps );

  G4VPhysicalVolume* physSiPM = new G4PVPlacement( 0
                                                 , SiPMOffset
                                                 , logicSiPM
                                                 , "SiPM"
                                                 , logicWorld
                                                 , false
                                                 , 0
                                                 , checkOverlaps );

  ///////////////////////////////////////////////////////////////////////////////
  // Placing dimple into physical world to enable a surface construction.
  ///////////////////////////////////////////////////////////////////////////////
  /*
     std::cout << "Constructing Physical Dimple volume" << std::endl;
     G4VSolid* hollowedDimple = new G4SubtractionSolid( "DimpleAir"
                                                   , solidDimple, solidSiPMOuter
                                                   , 0, CalcSiPMDimpleOffset() );

     G4LogicalVolume* logicDimple = new G4LogicalVolume( hollowedDimple
                                                    , fAir, "Dimple" );

     G4VPhysicalVolume* physDimple = new G4PVPlacement( 0
                                                   , CalcDimpleOffset()
                                                   , logicDimple
                                                   , "Dimple"
                                                   , logicWorld
                                                   , false
                                                   , 0
                                                   , checkOverlaps );
   */
  ///////////////////////////////////////////////////////////////////////////////
  // Defining surfaces
  ///////////////////////////////////////////////////////////////////////////////
  std::cout << "Defining surfaces" << std::endl;

  G4LogicalBorderSurface* WrapAirSurface =
    new G4LogicalBorderSurface( "WrapAirSurface"
                              , physWorld
                              , physWrap
                              , fESROpSurface );
  G4LogicalBorderSurface* TileSurface =
    new G4LogicalBorderSurface( "TileSurface"
                              , physTile
                              , physWorld
                              , fIdealPolishedOpSurface );
  // G4LogicalBorderSurface* DimpleTileSurface =
  //   new G4LogicalBorderSurface( "DimpleTileSurface"
  //                             , physDimple
  //                             , physTile
  //                             , fIdealPolishedOpSurface );
  // G4LogicalBorderSurface* DimpleAirSurface =
  //  new G4LogicalBorderSurface( "DimpleAirSurface"
  //                            , physDimple
  //                            , physWorld
  //                            , fIdealPolishedOpSurface );
  G4LogicalSkinSurface* CaseSurface
    = new G4LogicalSkinSurface( "SiPMCaseSurface"
                              , logicSiPMCase
                              , fIdealMirrorOpSurface );
  G4LogicalSkinSurface* SiPMSurface
    = new G4LogicalSkinSurface( "SiPMSurface"
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

  // G4VisAttributes* DimpleVisAtt = new G4VisAttributes( G4Colour( 0., 1., 1. ) );
  // DimpleVisAtt->SetForceWireframe( true );
  // DimpleVisAtt->SetVisibility( false );
  // logicDimple->SetVisAttributes( DimpleVisAtt );

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
LYSimDetectorConstruction::ConstructTrapazoidSolid(
  const G4String& name,
  double          x,
  double          y,
  double          z,
  double          indent_x1,
  double          indent_x2 ) const
{
  const G4ThreeVector corners[8] = {
    G4ThreeVector( -x/2,           -y/2, -z/2 ),
    G4ThreeVector( x/2,            -y/2, -z/2 ),
    G4ThreeVector( -x/2+indent_x2, y/2,  -z/2 ),
    G4ThreeVector( x/2-indent_x1,  y/2,  -z/2 ),
    G4ThreeVector( -x/2,           -y/2, z/2 ),
    G4ThreeVector( x/2,            -y/2, z/2 ),
    G4ThreeVector( -x/2+indent_x2, y/2,  z/2 ),
    G4ThreeVector( x/2-indent_x1,  y/2,  z/2 )
  };

  return new G4Trap( name, corners );
}

G4VSolid*
LYSimDetectorConstruction::ConstructHollowWrapSolid() const
{
  G4VSolid* wrapOuter = ConstructTrapazoidSolid( "WrapOuter"
                                               , _tilex + 2*wrapgap + 0.5*mm
                                               , _tiley + 2*wrapgap + 0.5*mm
                                               , _tilez + 2*wrapgap + 0.5*mm
                                               , 0, 0 );
  G4VSolid* wrapInner = ConstructTrapazoidSolid( "WrapInner"
                                               , _tilex + 2*wrapgap
                                               , _tiley + 2*wrapgap
                                               , _tilez + 2*wrapgap
                                               , 0, 0 );
  return new G4SubtractionSolid( "WrapSolid", wrapOuter, wrapInner );
}

G4VSolid*
LYSimDetectorConstruction::ConstructSphereDimpleSolid() const
{
  G4Sphere* solidsphere = new G4Sphere( "DimpleSphere"
                                      , 0, GetDimpleSizeRadius()
                                      , 0.,  2.*pi
                                      , pi/2, pi );

  G4Box* solidsub = new G4Box( "DimpleSphereSubBox"
                             , GetDimpleSizeRadius(), GetDimpleSizeRadius()
                             , GetDimpleSizeRadius() - GetDimpleIndent() );

  return new G4SubtractionSolid( "Dimple", solidsphere, solidsub );
}

G4VSolid*
LYSimDetectorConstruction::ConstructParabolicDimpleSolid() const
{
  const G4double SemiZ = ( 447317/288000 )*mm + _dimple_indent;
  const G4double SemiX = ( 447317/120000 )*mm;

  return new G4Ellipsoid( "DimpleParabolic",
    SemiX,
    SemiX,
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
  if( _dimple_type == PYRAMID ){
    return G4ThreeVector( 0, 0, 0.5*_tilez-0.5*_dimple_indent );
  } else if( _dimple_type == PARABOLIC ){
    return G4ThreeVector( 0, 0, 0.5*_tilez+( 447317/288000 )*mm );
  } else {
    return G4ThreeVector( 0, 0
                        , 0.5*_tilez+GetDimpleSizeRadius()-GetDimpleIndent() );
  }
}

G4ThreeVector
LYSimDetectorConstruction::CalcSiPMDimpleOffset() const
{
  if( _dimple_type == PYRAMID ){
    return G4ThreeVector( 0, 0, 0.5*_dimple_indent - 0.5*_sipm_z );
  } else if( _dimple_type == PARABOLIC ){
    return G4ThreeVector( 0, 0, 0 );// TO-DO!!
  } else {
    return G4ThreeVector( 0, 0,
      -GetDimpleSizeRadius() + GetDimpleIndent()- 0.5*_sipm_z );
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


void
LYSimDetectorConstruction::SetTileAbsMult( const double mult )
{
  _absmult = mult;
  Update_EJ200_AbsLength( fEJ200, _absmult );
}

void
LYSimDetectorConstruction::SetWrapReflect( const double r )
{
  // Add entries into properties table
  static const unsigned nentries      = 2;
  static double phoE[nentries]  = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {r, r};

  G4MaterialPropertiesTable* table = fESROpSurface->GetMaterialPropertiesTable();
  if( table ){
    table->RemoveProperty( "REFLECTIVITY" );
    table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  } else {
    table = new G4MaterialPropertiesTable();
    table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
    fESROpSurface->SetMaterialPropertiesTable( table );
  }
}
