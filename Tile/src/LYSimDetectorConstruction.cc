#include "LYSimDetectorConstruction.hh"
#include "LYSimDetectorMessenger.hh"
#include "Materials.hh"

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
#include "G4OpticalSurface.hh"
#include "G4Orb.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RegionStore.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4Sphere.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Torus.hh"
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

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LYSimPMTSD* LYSimDetectorConstruction::fPMTSD = NULL;

LYSimDetectorConstruction::LYSimDetectorConstruction()
  : G4VUserDetectorConstruction()
{
  fdetectorMessenger = new LYSimDetectorMessenger( this );

  DimpleType = 0;// 0: Normal, 1: Pyramid, 2: Parabolic
  solidWorld = NULL;
  logicWorld = NULL;
  physWorld  = NULL;
  logicWWW   = NULL;

  // Defining material list.
  fAir      = NULL;
  fBialkali = fEpoxy = fEpoxy2 = NULL;// SiPM and Adhesive
  fSCSN81   = fEJ200 = fEJ260 = NULL; // Scintillating material.

  fTyvekOpSurface    = fIdealTyvekOpSurface = fUnifiedTyvekOpSurface = fUnifiedIdealTyvekOpSurface = NULL;
  fESROpSurface      = NULL;
  fRoughOpSurface    = NULL;
  fPolishedOpSurface = fIdealPolishedOpSurface = NULL;
  fMirrorOpSurface   = fIdealMirrorOpSurface = NULL;
  fUpdated           = false;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LYSimDetectorConstruction::~LYSimDetectorConstruction()
{
  if( fdetectorMessenger ){ delete fdetectorMessenger; }
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume*
LYSimDetectorConstruction::Construct()
{
  SetDefaults();
  DefineMaterials();
  DefineSurfaces();
  return ConstructDetector();
}

void
LYSimDetectorConstruction::SetDefaults()
{
  // Set default parameters
  // fiber_hole_toggle = true;
  wrapping_toggle = true;
  // Geometry parameters
  //
  angle1             = 0*degree;
  angle2             = 0*degree;// for tile
  angle3             = 0*degree;// for wrap
  Dx2                = 0*mm;
  Dx3                = 0*mm;
  Dw2                = 0*mm;
  scint_sizeX        = 29.65*mm;
  scint_sizeY        = 29.65*mm;
  scint_thickness    = 2.98*mm;// 3.7*mm for SCSN81
  Photocat_sizeX     = 1.3*mm;
  Photocat_sizeY     = 1.3*mm;
  Photocat_thickness = 0.025*mm;// arbitrary thickness for Photocathode
  ScintPMT_gap       = 2.0*mm;// rough estimate of air gap between scintillator and PMT face
  wrapgap            = 0.05*mm;


  tileAbsLength = 380*cm;
  inducedMuTile = 1.e-20/cm;
  readoutCorner = 1;

  // world volume just needs to be big enough to accommodate everything
  world_sizeX = 3*( scint_sizeX );
  world_sizeY = 3*( scint_sizeY );
  world_sizeZ = 30*( scint_thickness );

  Absmultiple = 1.0;// factor for Abslength manipulation.
  Din         = 1.25*1.6*mm;
  deft        = sqrt( ( 2*( 4.5 )-1.6 )*1.6 );
  Rad         = 1.25*deft*mm;// 3.4409*mm
  SiPM_Depth  = 0.0*mm;  // SiPM Depth (0.0 is flush with top of tile, don't use anything less than 0.05*mm unless 0.0)
  // Dimple radius (normal is 3.4409*mm)
  // Dimple depth (normal is 1.6*mm)
}

G4VPhysicalVolume*
LYSimDetectorConstruction::ConstructDetector()
{
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4Box* solidWorld =
    new G4Box( "World", 0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ );

  G4LogicalVolume* logicWorld = new G4LogicalVolume( solidWorld, fAir, "World" );

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement( 0,// no rotation
      G4ThreeVector(),// at (0,0,0)
      logicWorld,// its logical volume
      "World",// its name
      0,// its mother  volume
      false,// no boolean operation
      0,// copy number
      checkOverlaps );// overlaps checking

  // logicWorld -> SetVisAttributes(new G4VisAttributes(white));
  logicWorld->SetVisAttributes( G4VisAttributes::Invisible );// MakeInvisible
  // logicWWW=logicWorld;


  ///////////////////////////////////////////////
  // wrapping
  /////////////////////////////////////////////////

  Dx2 = scint_sizeX-tan( angle1 )*scint_sizeY-tan( angle2 )*scint_sizeY;
  Dx3 = scint_sizeX-tan( angle1 )*scint_sizeY-tan( angle3 )*scint_sizeY;
  G4double xdisp  = 0.25*( scint_sizeX-Dx2 );
  G4double xdisp2 = 0.25*( scint_sizeX-Dx3 );

  if( wrapping_toggle ){
    /*
        G4Box* solidWrap =
       new G4Box("WrapBox",                                           //its name
        0.5*scint_sizeX+wrapgap, 0.5*scint_sizeY+wrapgap, 0.5*scint_thickness+wrapgap);     //its size
     */

    G4ThreeVector wrapCenter( 0, 0, 0 );

    Dw2 = scint_sizeX-tan( angle1 )*scint_sizeY-tan( angle3 )*scint_sizeY;

    G4VSolid* solidWrap =
      ConstructTileSolid( "TileTrap",
        angle1,// angle measured ccw from y axis for the side at -x
        angle3,// angle measured ccw from y axis for the side at +x
        Dw2+2*wrapgap,// length along x of side at y=+Dy
        scint_sizeY+2*wrapgap,// length along y
        scint_thickness+2*wrapgap,// length along z
        wrapCenter// coordinate of corner at -x, -y, -z with respect to origin
        );

    G4Box* wrapsub = new G4Box( "WrapSub",// its name
      0.5*10*mm, 0.5*scint_sizeY+1*mm, 0.5*scint_thickness+1*mm );// its size

    G4SubtractionSolid* Wrap = new G4SubtractionSolid( "wrapper", solidWrap, wrapsub, 0, G4ThreeVector( 10*mm+wrapgap, 0, 0 ) );

    G4LogicalVolume* logicWrap =
      new G4LogicalVolume( solidWrap,// its solid (usually solidWrap)
        fAir,// its material
        "Wrap" );// its name
    logicWWW = logicWrap;

    G4ThreeVector WrapOffset( -xdisp2, 0, 0 );

    G4VPhysicalVolume* physWrap =
      new G4PVPlacement( 0,
        WrapOffset,
        logicWrap,
        "Wrap",
        logicWorld,
        false,
        0,
        checkOverlaps );

    G4LogicalBorderSurface* WrapAirSurface =
      new G4LogicalBorderSurface( "WrapAirSurface",
        physWrap,
        physWorld,
        fESROpSurface );// fIdealMirrorOpSurface

    // Wrap visualization attributes
    G4VisAttributes* WrapVisAtt = new G4VisAttributes( G4Colour( 0., 1., 0. ) );
    WrapVisAtt->SetForceWireframe( true );
    WrapVisAtt->SetVisibility( true );
    logicWrap->SetVisAttributes( WrapVisAtt );

  }


  ///////////////////////////////////////////////////////
  // Subtracted Dimple Version (dimple sub from tile, WWW = mothervolume of both) SHALLOW DIMPLE 4.5mm Radius
  /////////////////////////////////////////////////////////

  G4ThreeVector tileCenter( 0, 0, 0 );

  G4VSolid* solidRod =
    ConstructTileSolid( "TileTrap",
      angle1,// angle measured ccw from y axis for the side at -x
      angle2,// angle measured ccw from y axis for the side at +x
      Dx2,// length along x of side at y=+Dy
      scint_sizeY,// length along y
      scint_thickness,// length along z
      tileCenter// coordinate of corner at -x, -y, -z with respect to origin
      );
/*
    G4Box* solidRod =
      new G4Box("RodBox",                                           //its name
    0.5*scint_sizeX, 0.5*scint_sizeY, 0.5*scint_thickness);     //its size
 */

  G4ThreeVector RodOffset( 0, 0, 0 );// -xdisp if weak

  G4Box* calicesub = new G4Box( "calicesub",
    0.5*10*mm, 0.5*scint_sizeY+1*mm, 0.5*scint_thickness+1*mm );// its size

  // G4SubtractionSolid *ROD = new G4SubtractionSolid("rodsub",solidRod,calicesub,0,G4ThreeVector(10*mm, 0, 0));

  G4double Dsize = 0.5*( ( Rad*Rad )/Din + Din );

  G4Sphere* solidDimple = new G4Sphere( "Dimple", 0., Dsize, 0., 2.*pi, pi/2., pi );

  G4double zdisp = 0.5*scint_thickness+Dsize-Din;
/*
    G4SubtractionSolid *Calice = new G4SubtractionSolid("Calice",solidRod,solidDimple,0,G4ThreeVector(xdisp, 0, zdisp));
 */
  G4LogicalVolume* logicCalice =
    new G4LogicalVolume( solidRod,// its solid (usually solidRod)
      fEJ200,// its material
      "Rod" );// its name

  G4VPhysicalVolume* physRod =
    new G4PVPlacement( 0,
      RodOffset,
      logicCalice,
      "Rod",
      logicWWW,
      false,
      0,
      checkOverlaps );

  G4LogicalBorderSurface* RodAirSurface =
    new G4LogicalBorderSurface( "RodAirSurface",
      physRod,
      physWorld,
      fPolishedOpSurface );

  // Rod visualization attributes
  G4VisAttributes* RodVisAtt = new G4VisAttributes( G4Colour( 1., 1., 0. ) );
  RodVisAtt->SetForceWireframe( true );
  RodVisAtt->SetVisibility( true );
  logicCalice->SetVisAttributes( RodVisAtt );

  G4double BSiz( 15.0*mm );
  G4double SubDis = Dsize-Din;
  G4double Subsub( 0.30*mm );
  G4Box* solidSub =
    new G4Box( "SubBox",// its name
      BSiz, BSiz, SubDis );// its size
  /*
     G4Box* solidSiPMsub =
     new G4Box("SiPMsubtract",
     0.5*2.1*mm, 0.5*2.1*mm, SubDis+SiPM_Depth);
   */

  G4SubtractionSolid* DimpleAir = new G4SubtractionSolid( "DimpleAir", solidDimple, solidSub );


  if( DimpleType == 0 ){
    G4LogicalVolume* logicDimple =
      new G4LogicalVolume( DimpleAir,// its solid
        fAir,// its material
        "Dimple" );// its name

    logicD = logicDimple;

    G4ThreeVector DimpleOffset( xdisp, 0, zdisp );
    G4VPhysicalVolume* DimpleRod =
      new G4PVPlacement( 0,
        DimpleOffset,
        logicDimple,
        "Dimple",
        logicCalice,
        false,
        0,
        checkOverlaps );

    G4LogicalBorderSurface* DimpleAirSurface =
      new G4LogicalBorderSurface( "DimpleSurface",
        DimpleRod,
        physRod,
        fIdealPolishedOpSurface );

    G4VisAttributes* DimpleVisAtt = new G4VisAttributes( G4Colour( 0., 1., 1. ) );
    DimpleVisAtt->SetForceWireframe( true );
    DimpleVisAtt->SetVisibility( true );
    logicDimple->SetVisAttributes( DimpleVisAtt );
  } else if( DimpleType == 1 ){
    // Pyramid Dimple
    G4Trd* PyramidDimple =
      new G4Trd( "Pyramid",
        0.0*mm, Rad,
        0.0*mm, Rad,
        Din*0.5 );

    G4LogicalVolume* logicPyramid =
      new G4LogicalVolume( PyramidDimple,// its solid
        fAir,// its material
        "Pyramid" );// its name

    G4ThreeVector PyramidOffset( 0, 0, 0.5*scint_thickness-0.5*Din );

    G4VPhysicalVolume* PyramidRod =
      new G4PVPlacement( 0,
        PyramidOffset,
        logicPyramid,
        "Pyramid",
        logicCalice,
        false,
        0,
        checkOverlaps );

    G4LogicalBorderSurface* PyramidAirSurface =
      new G4LogicalBorderSurface( "PyramidSurface",
        PyramidRod,
        physRod,
        fIdealPolishedOpSurface );

    G4VisAttributes* PyramidVisAtt = new G4VisAttributes( G4Colour( 0., 1., 1. ) );
    PyramidVisAtt->SetForceWireframe( true );
    PyramidVisAtt->SetVisibility( true );
    logicPyramid->SetVisAttributes( PyramidVisAtt );
  } else {
    G4double SemiZ = ( 447317/288000 )*mm + Din;
    G4double SemiX = ( 447317/120000 )*mm;
    G4double SemiY = SemiX;

    G4Ellipsoid* ParabolaDimple =
      new G4Ellipsoid( "Ellip",
        SemiX,
        SemiY,
        SemiZ,
        -SemiZ,// must be < SemiZ
        -SemiZ+Din );// must be >-SemiZ

    G4LogicalVolume* logicEllipse =
      new G4LogicalVolume( ParabolaDimple,// its solid
        fAir,// its material
        "Ellipse" );// its name

    G4ThreeVector EllipseOffset( 0, 0, 0.5*scint_thickness+SemiZ-Din );

    G4VPhysicalVolume* EllipseRod = new G4PVPlacement( 0,
      EllipseOffset,
      logicEllipse,
      "Ellipse",
      logicCalice,
      false,
      0,
      checkOverlaps );

    G4LogicalBorderSurface* EllipseAirSurface = new G4LogicalBorderSurface(
      "EllipseSurface",
      EllipseRod,
      physRod,
      fIdealPolishedOpSurface );

    G4VisAttributes* EllipseVisAtt = new G4VisAttributes( G4Colour( 0., 1., 1. ) );
    EllipseVisAtt->SetForceWireframe( true );
    EllipseVisAtt->SetVisibility( true );
    logicEllipse->SetVisAttributes( EllipseVisAtt );
  }

  ////////////////////////////////////////////
  // SiPM
  ////////////////////////////////////////////

  G4double SubD    = Dsize-Din;
  G4Box* solidSiPM =
    new G4Box( "SiPM",
      0.5*Photocat_sizeX, 0.5*Photocat_sizeY, 0.5*Photocat_thickness );

  G4LogicalVolume* logicSiPM =
    new G4LogicalVolume( solidSiPM,
      fBialkali,// Change
      "SiPM" );
  G4RotationMatrix* rotSiPM = new G4RotationMatrix;
  rotSiPM->rotateX( 0*rad );
  rotSiPM->invert();

  G4double backthick( 0.025*mm );

  G4Box* solidSiPMback =
    new G4Box( "SiPMBack",
      0.5*2.1*mm, 0.5*2.1*mm, backthick );


  G4SubtractionSolid* SiPMCase = new G4SubtractionSolid( "SiPMCase",
    solidSiPMback,
    solidSiPM,
    0,
    G4ThreeVector( 0, 0, 0.5*Photocat_thickness-backthick ) );
  G4LogicalVolume* logicSiPMCase =
    new G4LogicalVolume( SiPMCase,
      fEpoxy,
      "SiPMBack" );

  if( SiPM_Depth == 0.0*mm ){
    G4ThreeVector CaseOffset( xdisp2, 0, 0.5*scint_thickness+0.025*mm-SiPM_Depth );
    G4VPhysicalVolume* SiPMCasing =
      new G4PVPlacement( 0,
        CaseOffset,
        logicSiPMCase,
        "Case",
        logicWWW,
        false,
        0,
        checkOverlaps );

  } else {
    G4ThreeVector CaseOffset( xdisp2, 0, -SubD+0.025*mm-SiPM_Depth );
    G4VPhysicalVolume* SiPMCasing =
      new G4PVPlacement( 0,
        CaseOffset,
        logicSiPMCase,
        "Case",
        logicD,
        false,
        0,
        checkOverlaps );

  }
  G4VisAttributes* CaseVisAtt = new G4VisAttributes( G4Colour( 0.5, 0.5, 0.5 ) );
  CaseVisAtt->SetForceSolid( true );
  CaseVisAtt->SetVisibility( true );
  logicSiPMCase->SetVisAttributes( CaseVisAtt );

  G4LogicalSkinSurface* CaseSurface = new G4LogicalSkinSurface( "name", logicSiPMCase, fIdealMirrorOpSurface );

  if( SiPM_Depth == 0.0*mm ){
    G4ThreeVector transSiPM( xdisp2, 0, 0.5*scint_thickness+0.5*Photocat_thickness-SiPM_Depth );
    G4VPhysicalVolume* physSiPM =
      new G4PVPlacement( rotSiPM,
        transSiPM,
        logicSiPM,
        "SiPM",
        logicWWW,
        false,
        0,
        checkOverlaps );
  } else {
    G4ThreeVector transSiPM( xdisp2, 0, -SubD+0.5*Photocat_thickness-SiPM_Depth );
    G4VPhysicalVolume* physSiPM =
      new G4PVPlacement( rotSiPM,
        transSiPM,
        logicSiPM,
        "SiPM",
        logicD,
        false,
        0,
        checkOverlaps );
  }

  G4OpticalSurface* SiPMOpSurface   = new G4OpticalSurface( "SiPM_Surface" );
  G4LogicalSkinSurface* SiPMSurface = new G4LogicalSkinSurface( "name", logicSiPM, SiPMOpSurface );

  SiPMOpSurface->SetType( dielectric_metal );
  SiPMOpSurface->SetModel( unified );

  G4MaterialPropertiesTable* SiPMOpSurfaceProperty = new G4MaterialPropertiesTable();

  // From Meeting on HB SiPM selection (James Hirschauer)

  const G4int SiPMnumentries            = 49;
  G4double SiPMenergies[SiPMnumentries] = {
    3.269640158*eV, 3.191037408*eV, 3.128716912*eV, 3.102662139*eV,
    3.069524008*eV, 3.034012326*eV, 3.001068157*eV, 2.959583079*eV,
    2.901064107*eV, 2.838801776*eV, 2.779808831*eV, 2.721566033*eV,
    2.667612494*eV, 2.615095968*eV, 2.563388259*eV, 2.515844086*eV,
    2.469260675*eV, 2.423979937*eV, 2.379744327*eV, 2.337228676*eV,
    2.296532193*eV, 2.257220927*eV, 2.219239034*eV, 2.182957454*eV,
    2.147148186*eV, 2.112470008*eV, 2.079619445*eV, 2.047338607*eV,
    2.015906691*eV, 1.987528043*eV, 1.959370034*eV, 1.930211969*eV,
    1.902680667*eV, 1.875153451*eV, 1.84878577*eV,  1.823292126*eV,
    1.799066012*eV, 1.775177229*eV, 1.751266161*eV, 1.728338166*eV,
    1.705491878*eV, 1.683828975*eV, 1.662853866*eV, 1.643031375*eV,
    1.623272564*eV, 1.603516479*eV, 1.584163112*eV, 1.565103929*eV,
    1.548241716*eV};
  G4double SiPMreflectivity[SiPMnumentries] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  G4double quantumefficiency[SiPMnumentries] = {
    0.00080151,  0.003873974,  0.009957452,   0.025682901,
    0.070237974, 0.150458664,  0.218876073,   0.239857707,
    0.250435989, 0.259508637,  0.264540183,   0.266889254,
    0.270669968, 0.273577562,  0.272402437,   0.270462795,
    0.268157907, 0.264243881,  0.25831461,    0.252029247,
    0.245123015, 0.237845409,  0.230874024,   0.224981219,
    0.220313658, 0.21616835,   0.211206974,   0.205552053,
    0.199704762, 0.193696557,  0.187884494,   0.182241421,
    0.176095301, 0.168483378,  0.16291645,    0.157493577,
    0.152319279, 0.146859841,  0.141516867,   0.135704067,
    0.129615124, 0.123742916,  0.11736541,    0.11184209,
    0.105217764, 0.097865669,  0.091000102,   0.084681686,
    0.080451249};

  // const G4int SiPMnumentries = 2;
  // G4double SiPMenergies[SiPMnumentries] = {1.0*eV,6.0*eV};
  // G4double SiPMreflectivity[SiPMnumentries] = {0.0,0.0};
  // G4double quantumefficiency[SiPMnumentries] = {1.0,1.0};
  SiPMOpSurfaceProperty->AddProperty(
    "REFLECTIVITY", SiPMenergies, SiPMreflectivity,  SiPMnumentries );
  SiPMOpSurfaceProperty->AddProperty(
    "EFFICIENCY",   SiPMenergies, quantumefficiency, SiPMnumentries );

  SiPMOpSurface->SetMaterialPropertiesTable( SiPMOpSurfaceProperty );
  if( !fPMTSD ){
    fPMTSD = new LYSimPMTSD( "/LYSimPMT" );
    G4SDManager* sdman = G4SDManager::GetSDMpointer();
    sdman->AddNewDetector( fPMTSD );
  }

  // Photocathode visualization attributes
  G4VisAttributes* SiPMVisAtt = new G4VisAttributes( G4Colour( 1., 1., 1. ) );
  SiPMVisAtt->SetForceWireframe( false );
  SiPMVisAtt->SetVisibility( true );
  logicSiPM->SetVisAttributes( SiPMVisAtt );

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

void
LYSimDetectorConstruction::DefineMaterials()
{
  fBialkali = Make_Bialkali();
  fEpoxy    = Make_Epoxy();
  fEpoxy2   = Make_Epoxy2();
  fAir      = Make_Custom_Air();

  fSCSN81 = Make_SCSN81( GetTileAbsLength(), GetInducedMuTile() );
  fEJ200  = Make_EJ200( Absmultiple );
  fEJ260  = Make_EJ260( GetTileAbsLength(), GetInducedMuTile() );
}



void
LYSimDetectorConstruction::DefineSurfaces()
{
  {
    //////////////////////////////////
    // Realistic Crystal-Tyvek surface
    //////////////////////////////////
    fTyvekOpSurface = new G4OpticalSurface( "TyvekOpSurface" );
    fTyvekOpSurface->SetType( dielectric_LUT );
    fTyvekOpSurface->SetModel( LUT );
    fTyvekOpSurface->SetFinish( polishedtyvekair );

    const G4int num            = 2;
    G4double Ephoton[num]      = {1.5*eV, 8.0*eV};
    G4double Reflectivity[num] = {0.979, 0.979};

    G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();

    MPT->AddProperty( "REFLECTIVITY", Ephoton, Reflectivity, num );

    fTyvekOpSurface->SetMaterialPropertiesTable( MPT );
  }

  {
    //////////////////////////////////
    // ESR surface
    //////////////////////////////////
    fESROpSurface = new G4OpticalSurface( "ESROpSurface" );
    fESROpSurface->SetType( dielectric_LUT );
    fESROpSurface->SetModel( LUT );
    fESROpSurface->SetFinish( polishedvm2000air );

    const G4int num            = 2;
    G4double Ephoton[num]      = {1.5*eV, 8.0*eV};
    G4double Reflectivity[num] = {0.985, 0.985};
    // source: https://www.osti.gov/servlets/purl/1184400

    G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();

    MPT->AddProperty( "REFLECTIVITY", Ephoton, Reflectivity, num );

    fESROpSurface->SetMaterialPropertiesTable( MPT );
  }

  {
    //////////////////////////////////
    // Ideal Crystal-Tyvek surface
    //////////////////////////////////
    fIdealTyvekOpSurface = new G4OpticalSurface( "IdealTyvekOpSurface" );
    fIdealTyvekOpSurface->SetType( dielectric_LUT );
    fIdealTyvekOpSurface->SetModel( LUT );
    fIdealTyvekOpSurface->SetFinish( polishedtyvekair );
  }

  {
    //////////////////////////////////
    // Unified Tyvek surface
    //////////////////////////////////
    fUnifiedTyvekOpSurface = new G4OpticalSurface( "UnifiedTyvekOpSurface" );
    fUnifiedTyvekOpSurface->SetType( dielectric_dielectric );
    fUnifiedTyvekOpSurface->SetModel( unified );
    fUnifiedTyvekOpSurface->SetFinish( groundbackpainted );
    fUnifiedTyvekOpSurface->SetSigmaAlpha( 1.3*degree );

    const G4int NUM             = 2;
    G4double pp[NUM]            = {2.0*eV, 6.0*eV};
    G4double specularlobe[NUM]  = {1.0, 1.0};
    G4double specularspike[NUM] = {0., 0.};
    G4double backscatter[NUM]   = {0., 0.};
    G4double rindex[NUM]        = {1.0, 1.0};
    G4double reflectivity[NUM]  = {0.979, 0.979};
    G4double efficiency[NUM]    = {0.0, 0.0};

    G4MaterialPropertiesTable* SMPT = new G4MaterialPropertiesTable();

    SMPT->AddProperty( "RINDEX",                pp,        rindex, NUM );
    SMPT->AddProperty( "SPECULARLOBECONSTANT",  pp,  specularlobe, NUM );
    SMPT->AddProperty( "SPECULARSPIKECONSTANT", pp, specularspike, NUM );
    SMPT->AddProperty( "BACKSCATTERCONSTANT",   pp,   backscatter, NUM );
    SMPT->AddProperty( "REFLECTIVITY",          pp,  reflectivity, NUM );
    SMPT->AddProperty( "EFFICIENCY",            pp,    efficiency, NUM );

    fUnifiedTyvekOpSurface->SetMaterialPropertiesTable( SMPT );

  }


  {
    //////////////////////////////////
    // Unified Ideal Tyvek surface
    //////////////////////////////////
    fUnifiedIdealTyvekOpSurface = new G4OpticalSurface(
      "UnifiedIdealTyvekOpSurface" );
    fUnifiedIdealTyvekOpSurface->SetType( dielectric_dielectric );
    fUnifiedIdealTyvekOpSurface->SetModel( unified );
    fUnifiedIdealTyvekOpSurface->SetFinish( groundbackpainted );
    fUnifiedIdealTyvekOpSurface->SetSigmaAlpha( 1.3*degree );

    const G4int NUM             = 2;
    G4double pp[NUM]            = {2.0*eV, 6.0*eV};
    G4double specularlobe[NUM]  = {1.0, 1.0};
    G4double specularspike[NUM] = {0., 0.};
    G4double backscatter[NUM]   = {0., 0.};
    G4double rindex[NUM]        = {1.0, 1.0};
    G4double reflectivity[NUM]  = {1.0, 1.0};
    G4double efficiency[NUM]    = {0.0, 0.0};

    G4MaterialPropertiesTable* SMPT = new G4MaterialPropertiesTable();

    SMPT->AddProperty( "RINDEX",                pp,        rindex, NUM );
    SMPT->AddProperty( "SPECULARLOBECONSTANT",  pp,  specularlobe, NUM );
    SMPT->AddProperty( "SPECULARSPIKECONSTANT", pp, specularspike, NUM );
    SMPT->AddProperty( "BACKSCATTERCONSTANT",   pp,   backscatter, NUM );
    SMPT->AddProperty( "REFLECTIVITY",          pp,  reflectivity, NUM );
    SMPT->AddProperty( "EFFICIENCY",            pp,    efficiency, NUM );

    fUnifiedIdealTyvekOpSurface->SetMaterialPropertiesTable( SMPT );
  }

  {
    //////////////////////////////////
    // Realistic polished surface
    //////////////////////////////////
    fPolishedOpSurface = new G4OpticalSurface( "PolishedOpSurface" );// , unified);
    fPolishedOpSurface->SetType( dielectric_dielectric );
    fPolishedOpSurface->SetModel( unified );
    fPolishedOpSurface->SetFinish( ground );// necessary even for polished surfaces to enable UNIFIED code
    fPolishedOpSurface->SetSigmaAlpha( 1.3 * degree );// Janecek2010 (1.3 * degree)

    G4MaterialPropertiesTable* PolishedOpSurfaceProperty = new G4MaterialPropertiesTable();

    const G4int NUM             = 2;
    G4double pp[NUM]            = {1.0*eV, 6.0*eV};
    G4double specularlobe[NUM]  = {1.0, 1.0};
    G4double specularspike[NUM] = {0., 0.};
    G4double backscatter[NUM]   = {0., 0.};

    PolishedOpSurfaceProperty->AddProperty(
      "SPECULARLOBECONSTANT", pp, specularlobe, NUM );
    fPolishedOpSurface->SetMaterialPropertiesTable( PolishedOpSurfaceProperty );
  }

  {
    //////////////////////////////////
    // Ideal polished surface
    //////////////////////////////////
    fIdealPolishedOpSurface = new G4OpticalSurface( "IdealOpSurface" );
    fIdealPolishedOpSurface->SetType( dielectric_dielectric );
    fIdealPolishedOpSurface->SetModel( glisur );
    fIdealPolishedOpSurface->SetFinish( polished );
  }

  {
    //////////////////////////////////
    // Mirror surface
    //////////////////////////////////
    fMirrorOpSurface = new G4OpticalSurface( "MirrorOpSurface" );
    fMirrorOpSurface->SetType( dielectric_metal );
    fMirrorOpSurface->SetFinish( polished );
    fMirrorOpSurface->SetModel( unified );

    G4MaterialPropertiesTable* MirrorOpSurfaceProperty = new G4MaterialPropertiesTable();
    const G4int NUM                                    = 2;
    G4double pp[NUM]                                   = {1.0*eV, 6.0*eV};
    G4double reflectivity[NUM]                         = {0.9, 0.9};
    MirrorOpSurfaceProperty->AddProperty( "REFLECTIVITY", pp, reflectivity, NUM );
    fMirrorOpSurface->SetMaterialPropertiesTable( MirrorOpSurfaceProperty );
  }

  {
    //////////////////////////////////
    // Ideal mirror surface
    //////////////////////////////////
    fIdealMirrorOpSurface = new G4OpticalSurface( "MirrorOpSurface" );
    fIdealMirrorOpSurface->SetType( dielectric_metal );
    fIdealMirrorOpSurface->SetFinish( polished );
    fIdealMirrorOpSurface->SetModel( unified );

    G4MaterialPropertiesTable* IdealMirrorOpSurfaceProperty = new G4MaterialPropertiesTable();
    const G4int NUM                                         = 2;
    G4double pp[NUM]                                        = {1.0*eV, 6.0*eV};
    G4double reflectivity[NUM]                              = {1.0, 1.0};
    IdealMirrorOpSurfaceProperty->AddProperty( "REFLECTIVITY", pp, reflectivity, NUM );
    fIdealMirrorOpSurface->SetMaterialPropertiesTable( IdealMirrorOpSurfaceProperty );
  }

  {
    //////////////////////////////////
    // Absorbing surface
    //////////////////////////////////
    fAbsorbingOpSurface = new G4OpticalSurface( "AbsorbingOpSurface" );
    fAbsorbingOpSurface->SetType( dielectric_dielectric );
    fAbsorbingOpSurface->SetFinish( groundfrontpainted );
    fAbsorbingOpSurface->SetModel( unified );

    G4MaterialPropertiesTable* AbsorbingOpSurfaceProperty = new G4MaterialPropertiesTable();
    const G4int NUM                                       = 2;
    G4double pp[NUM]                                      = {1.0*eV, 6.0*eV};
    G4double reflectivity[NUM]                            = {0.0, 0.0};
    AbsorbingOpSurfaceProperty->AddProperty( "REFLECTIVITY", pp, reflectivity, NUM );
    fAbsorbingOpSurface->SetMaterialPropertiesTable( AbsorbingOpSurfaceProperty );
  }
  {
    //////////////////////////////////
    // Rough surface
    //////////////////////////////////
    fRoughOpSurface = new G4OpticalSurface( "RoughOpSurface" );
    fRoughOpSurface->SetType( dielectric_dielectric );
    fRoughOpSurface->SetFinish( ground );
    fRoughOpSurface->SetModel( unified );
  }
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
  G4SurfaceProperty::CleanSurfacePropertyTable();

  DefineMaterials();
  DefineSurfaces();
  G4RunManager::GetRunManager()->DefineWorldVolume( ConstructDetector() );
  G4cout << "[LYSim] Setting induced absorption coefficient = "
         << inducedMuTile << " [cm^-1]" << G4endl;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  // G4RunManager::GetRunManager()->PhysicsHasBeenModified();

  // G4RegionStore::GetInstance()->UpdateMaterialList(physWorld);
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
