#include "G4NistManager.hh"

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/Materials.hh"
#else
#include "Materials.hh"
#endif

using namespace CLHEP;

// static list of common materials (cannot be const)
G4Element* C  = G4NistManager::Instance()->FindOrBuildElement( "C" );
G4Element* H  = G4NistManager::Instance()->FindOrBuildElement( "H" );
G4Element* Si = G4NistManager::Instance()->FindOrBuildElement( "Si" );
G4Element* O  = G4NistManager::Instance()->FindOrBuildElement( "O" );
G4Element* K  = G4NistManager::Instance()->FindOrBuildElement( "K" );
G4Element* Sb = G4NistManager::Instance()->FindOrBuildElement( "Sb" );
G4Element* Rb = G4NistManager::Instance()->FindOrBuildElement( "Rb" );
G4Element* Cs = G4NistManager::Instance()->FindOrBuildElement( "Cs" );
G4Element* Lu = G4NistManager::Instance()->FindOrBuildElement( "Lu" );
G4Element* Y  = G4NistManager::Instance()->FindOrBuildElement( "Y" );
G4Element* Ce = G4NistManager::Instance()->FindOrBuildElement( "Ce" );
G4Element* La = G4NistManager::Instance()->FindOrBuildElement( "La" );
G4Element* Br = G4NistManager::Instance()->FindOrBuildElement( "Br" );
G4Element* Na = G4NistManager::Instance()->FindOrBuildElement( "Na" );
G4Element* I  = G4NistManager::Instance()->FindOrBuildElement( "I" );
G4Element* Tl = G4NistManager::Instance()->FindOrBuildElement( "Tl" );
G4Element* Al = G4NistManager::Instance()->FindOrBuildElement( "Al" );

// Bialkali definition:
// Ref: http://hypernews.slac.stanford.edu/HyperNews/geant4/get/AUX/2013/03/11/12.39-85121-chDetectorConstruction.cc
G4Material*
Make_Bialkali()
{
  G4Material* material
    = new G4Material( "Bialkali", 4.28*g/cm3, 3, kStateSolid );
  material->AddElement( K,  13.3*perCent );
  material->AddElement( Cs, 45.2*perCent );
  material->AddElement( Sb, 41.5*perCent );
  return material;
}

G4Material*
Make_Epoxy()
{
  G4Material* epoxy = new G4Material( "Epoxy", 3.75*g/cm3, 2, kStateSolid );
  epoxy->AddElement( Al, 40*perCent );
  epoxy->AddElement( O,  60*perCent );
  const unsigned nentries       = 2;
  double photonE[nentries]      = {1.5*eV, 8.0*eV};
  double refrac_idx[nentries]   = {1.50, 1.50};
  double Reflectivity[nentries] = {0.0, 0.0};
  double abs_length[nentries]   = {10*m, 10*m};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",       photonE, refrac_idx,   nentries );
  table->AddProperty( "REFLECTIVITY", photonE, Reflectivity, nentries );
  table->AddProperty( "ABSLENGTH",    photonE, abs_length,   nentries );

  epoxy->SetMaterialPropertiesTable( table );
  return epoxy;
}

G4Material*
Make_Epoxy2()
{
  static const unsigned nentries       = 2;
  static double photonE[nentries]      = {1.5*eV, 8.0*eV};
  static double refrac_idx[nentries]   = {4.48, 4.48};
  static double Reflectivity[nentries] = {0.0, 0.0};
  static double abs_length[nentries]   = {10*m, 10*m};

  G4Material* material = new G4Material( "Epoxy2", 3.75*g/cm3, 2, kStateSolid );
  material->AddElement( Al, 40*perCent );
  material->AddElement( O,  60*perCent );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty(
    "RINDEX",       photonE, refrac_idx, nentries );
  table->AddProperty(
    "REFLECTIVITY", photonE, Reflectivity,    nentries );
  table->AddProperty(
    "ABSLENGTH",    photonE, abs_length,       nentries );

  material->SetMaterialPropertiesTable( table );
  return material;
}

G4Material*
Make_Custom_Air()
{
  static const unsigned nentries     = 2;
  static double photonE[nentries]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries] = {1.0, 1.0};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX", photonE, refrac_idx, nentries );

  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_AIR" );
  material->SetMaterialPropertiesTable( table );
  return material;
}

G4Material*
Make_Backing()
{
  G4Material* material
    = new G4Material( "Backing", 3.75*g/cm3, 2, kStateSolid );
  material->AddElement( C, 100*perCent );
  static const unsigned nentries       = 2;
  static double photonE[nentries]      = {1.5*eV, 8.0*eV};
  static double refrac_idx[nentries]   = {0., 0.};
  static double Reflectivity[nentries] = {0.0, 0.0};
  static double abs_length[nentries]   = {0., 0.};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",       photonE, refrac_idx,   nentries );
  table->AddProperty( "REFLECTIVITY", photonE, Reflectivity, nentries );
  table->AddProperty( "ABSLENGTH",    photonE, abs_length,   nentries );

  material->SetMaterialPropertiesTable( table );
  return material;
}

G4Material*
Make_Custom_SiO2()
{
  static const unsigned nentries = 50;
  // Photon energy corresponds to ~620nm, ~611nm, ... ~357nm wavelengths
  static double photonE[nentries] = {
    2.00*eV, 2.03*eV, 2.06*eV, 2.09*eV, 2.12*eV,
    2.15*eV, 2.18*eV, 2.21*eV, 2.24*eV, 2.27*eV,
    2.30*eV, 2.33*eV, 2.36*eV, 2.39*eV, 2.42*eV,
    2.45*eV, 2.48*eV, 2.51*eV, 2.54*eV, 2.57*eV,
    2.60*eV, 2.63*eV, 2.66*eV, 2.69*eV, 2.72*eV,
    2.75*eV, 2.78*eV, 2.81*eV, 2.84*eV, 2.87*eV,
    2.90*eV, 2.93*eV, 2.96*eV, 2.99*eV, 3.02*eV,
    3.05*eV, 3.08*eV, 3.11*eV, 3.14*eV, 3.17*eV,
    3.20*eV, 3.23*eV, 3.26*eV, 3.29*eV, 3.32*eV,
    3.35*eV, 3.38*eV, 3.41*eV, 3.44*eV, 3.47*eV};

  static double refrac_idx[nentries] = {
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60};

  static double abs_length[nentries] = {
    5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
    5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
    5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
    5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
    5.40*m, 5.40*m, 5.40*m, 5.40*m, 5.40*m,
    5.40*m, 5.40*m, 5.40*m, 5.40*m, 1.10*m,
    1.10*m, 1.10*m, 1.10*m, 1.10*m, 1.10*m,
    1.10*m, 1.*mm,  1.*mm,  1.*mm,  1.*mm,
    1.*mm,  1.*mm,  1.*mm,  1.*mm,  1.*mm,
    1.*mm,  1.*mm,  1.*mm,  1.*mm,  1.*mm };

  static double emission[nentries] = {
    0.05, 0.10, 0.30, 0.50, 0.75,
    1.00, 1.50, 1.85, 2.30, 2.75,
    3.25, 3.80, 4.50, 5.20, 6.00,
    7.00, 8.50, 9.50, 11.1, 12.4,
    12.9, 13.0, 12.8, 12.3, 11.1,
    11.0, 12.0, 11.0, 17.0, 16.9,
    15.0, 9.00, 2.50, 1.00, 0.05,
    0.00, 0.00, 0.00, 0.00, 0.00,
    0.00, 0.00, 0.00, 0.00, 0.00,
    0.00, 0.00, 0.00, 0.00, 0.00 };


  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty(
    "RINDEX", photonE, refrac_idx, nentries )->SetSpline( true );
  table->AddProperty(
    "WLSABSLENGTH", photonE, abs_length, nentries )->SetSpline( true );
  table->AddProperty(
    "WLSCOMPONENT", photonE, emission, nentries )->SetSpline( true );
  table->AddConstProperty( "WLSTIMECONSTANT", 0.5*ns );

  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_SILICON_DIOXIDE" );
  material->SetMaterialPropertiesTable( table );

  return material;
}


G4Material*
Make_Custom_PolyStyrene()
{
  static const unsigned nentries = 50;
  // Photon energy corresponds to ~620nm, ~611nm, ... ~357nm wavelengths
  static double photonE[nentries] = {
    2.00*eV, 2.03*eV, 2.06*eV, 2.09*eV, 2.12*eV,
    2.15*eV, 2.18*eV, 2.21*eV, 2.24*eV, 2.27*eV,
    2.30*eV, 2.33*eV, 2.36*eV, 2.39*eV, 2.42*eV,
    2.45*eV, 2.48*eV, 2.51*eV, 2.54*eV, 2.57*eV,
    2.60*eV, 2.63*eV, 2.66*eV, 2.69*eV, 2.72*eV,
    2.75*eV, 2.78*eV, 2.81*eV, 2.84*eV, 2.87*eV,
    2.90*eV, 2.93*eV, 2.96*eV, 2.99*eV, 3.02*eV,
    3.05*eV, 3.08*eV, 3.11*eV, 3.14*eV, 3.17*eV,
    3.20*eV, 3.23*eV, 3.26*eV, 3.29*eV, 3.32*eV,
    3.35*eV, 3.38*eV, 3.41*eV, 3.44*eV, 3.47*eV};

  static double refrac_idx[nentries] = {
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50};

  static double abs_length[nentries] = {
    2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm,
    2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm,
    2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm,
    2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm,
    2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm, 2.*cm};

  static double ScintilFast[nentries] = {
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0};

  // Add entries into properties table
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",        photonE, refrac_idx,  nentries );
  table->AddProperty( "ABSLENGTH",     photonE, abs_length,  nentries );
  table->AddProperty( "FASTCOMPONENT", photonE, ScintilFast, nentries );
  table->AddConstProperty( "SCINTILLATIONYIELD", 10./keV );
  table->AddConstProperty( "RESOLUTIONSCALE",    1.0 );
  table->AddConstProperty( "FASTTIMECONSTANT",   10.*ns );


  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_POLYSTYRENE" );
  material->SetMaterialPropertiesTable( table );
  material->GetIonisation()->SetBirksConstant( 0.126*mm/MeV );

  return material;
}


G4Material*
Make_Custom_Pyrex()
{
  static const unsigned nentries     = 2;
  static double photonE[nentries]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries] = {1.52, 1.52};
  static double abs_length[nentries] = {10*m, 10*m};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",    photonE, refrac_idx, nentries );
  table->AddProperty( "ABSLENGTH", photonE, abs_length, nentries );

  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_Pyrex_Glass" );
  material->SetMaterialPropertiesTable( table );

  return material;
}

G4Material*
Make_Custom_Water()
{
  static const unsigned nentries     = 2;
  static double photonE[nentries]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries] = {1.4, 1.4};
  static double abs_length[nentries] = {10*m, 10*m};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",    photonE, refrac_idx, nentries );
  table->AddProperty( "ABSLENGTH", photonE, abs_length, nentries );

  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_WATER" );
  material->SetMaterialPropertiesTable( table );

  return material;
}

G4Material*
Make_LYSO()
{
  // ------------------------------
  G4Material* material
    = new G4Material( "LYSO", 7.1*g/cm3, 5, kStateSolid );
  material->AddElement( Lu, 71.43*perCent );
  material->AddElement( Y,   4.03*perCent );
  material->AddElement( Si,  6.37*perCent );
  material->AddElement( O,  18.14*perCent );
  material->AddElement( Ce,  0.02*perCent );  // cooke2000


  // LYSO Sellmeier fit from Sasha
  double n0      = 1.43923e0;
  double n1      = 3.67622e-1;
  double lambda1 = 2.95130e2;

  const unsigned nentriesrefindex = 35;
  // wavelength array in nm
  double lysowavelength[nentriesrefindex] =
  {
    360, 370, 380, 390, 400,
    410, 420, 430, 440, 450,
    460, 470, 480, 490, 500,
    510, 520, 530, 540, 550,
    560, 570, 580, 590, 600,
    610, 620, 630, 640, 650,
    660, 670, 680, 690, 700
  };

  double lysoenergies[nentriesrefindex];

  for( unsigned i = 0; i < nentriesrefindex; i++ ){
    lysoenergies[i] = 1239.842 / lysowavelength[i] * eV;
  }

  double lysorefindex[nentriesrefindex];

  for( unsigned i = 0; i < nentriesrefindex; i++ ){
    lysorefindex[i] = sqrt( 1 + pow( n0, 2.0 ) + pow( n1, 2.0 ) / ( 1 - pow( lambda1, 2.0 ) / pow( lysowavelength[i], 2.0 ) ) );
  }

  /*
  double lysoconstrefindex[nentriesrefindex];

  for( unsigned i = 0; i < nentriesrefindex; i++ ){
    lysoconstrefindex[i] = 1.82;
  }
  */



  const unsigned nentrieslal      = 2;
  double energieslal[nentrieslal] = {1.0*eV, 6.0*eV};
  double lal[nentrieslal]         = {200*cm, 200*cm};

  // Light Absorption Length
  // From 1mm sample transmission data
  const unsigned nentrieslysolal          = 10;
  double lysoenergieslal[nentrieslysolal] = {
    3.351*eV, 3.263*eV, 3.179*eV, 3.100*eV, 3.024*eV,
    2.952*eV, 2.883*eV, 2.695*eV, 2.384*eV, 2.066*eV
  };
  double lysolal[nentrieslysolal] = {
    0.025*cm, 0.1*cm,   1*cm,     4*cm,      6*cm,
    7*cm,     8*cm,     9*cm,     10*cm,     12*cm
  };


  // Scintillation emission spectrum (fast component)
  // Gamma-ray emission
  static const unsigned nentriesemissiongamma                    = 16;
  static double lysoenergiesemissiongamma[nentriesemissiongamma] =
  {
    3.44*eV, 3.26*eV, 3.18*eV, 3.10*eV, 3.02*eV,
    2.95*eV, 2.88*eV, 2.82*eV, 2.70*eV, 2.58*eV,
    2.48*eV, 2.38*eV, 2.30*eV, 2.21*eV, 2.14*eV,
    1.82*eV
  };
  static double lysoemissiongamma[nentriesemissiongamma] =
  {
    0.00, 0.06, 0.28, 0.72, 1.40,
    2.00, 2.20, 2.06, 1.48, 0.94,
    0.60, 0.40, 0.30, 0.20, 0.10,
    0.00
  };

  // Photoluminescence (theta = 10 deg)
  static const unsigned nentrieslysoemission               = 21;
  static double lysoenergiesemission[nentrieslysoemission] = {
    3.54*eV, 3.35*eV, 3.26*eV, 3.18*eV, 3.13*eV,
    3.10*eV, 3.02*eV, 2.95*eV, 2.88*eV, 2.82*eV,
    2.76*eV, 2.70*eV, 2.64*eV, 2.58*eV, 2.53*eV,
    2.48*eV, 2.43*eV, 2.38*eV, 2.30*eV, 2.21*eV,
    2.00*eV };
  static double lysoemission[nentrieslysoemission] = {
    0,    0.26,    1.26,    2.14,     2.2,
    2.16, 2.04,    1.9,     1.64,     1.3,
    0.9,  0.62,    0.38,    0.26,     0.14,
    0.1,  0.08,    0.06,    0.04,     0.02,
    0 };

  G4MaterialPropertiesTable* lysoprop = new G4MaterialPropertiesTable();
  lysoprop->AddProperty(
    "FASTCOMPONENT", lysoenergiesemission, lysoemission, nentrieslysoemission );
  lysoprop->AddProperty(
    "RINDEX",       lysoenergies, lysorefindex, nentriesrefindex );
  lysoprop->AddProperty(
    "ABSLENGTH", lysoenergieslal, lysolal, nentrieslysolal );// scenario
  lysoprop->AddConstProperty( "SCINTILLATIONYIELD", 32./keV );// saint-gobain
  lysoprop->AddConstProperty( "RESOLUTIONSCALE",    1.0 );
  lysoprop->AddConstProperty( "FASTTIMECONSTANT",  41.0*ns );   // saint-gobain
  lysoprop->AddConstProperty( "YIELDRATIO",         1.0 );
  material->SetMaterialPropertiesTable( lysoprop );

  return material;
}

G4Material*
Make_FiberCore()
{
// fiberCore Y11 (http://kuraraypsf.jp/psf/index.html)
  G4Material* material
    = new G4Material( "FiberCorePS", 1.05*g/cm3, 2, kStateSolid );
  material->AddElement( C, 92.26*perCent );
  material->AddElement( H,  7.74*perCent );

  // Fiber material definition
  static double baseabs_length = 350.0*cm;
  static double baseMu         = 1 / baseabs_length;
  // double inducedMu = GetInducedMuFiber();
  // double mu = baseMu + inducedMu;
  static double mu        = baseMu;
  static double absLength = 1 / mu;

  static const unsigned nentries     = 2;
  static double photonE[nentries]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries] = {1.59, 1.59};
  static double abs_length[nentries] = {absLength, absLength};

  // FIXME: findout Y11 parameters
  const unsigned nentries1   = 91;
  double photonE1[nentries1] = {
    1.776*eV, 1.794*eV, 1.807*eV, 1.821*eV, 1.832*eV,
    1.851*eV, 1.868*eV, 1.876*eV, 1.887*eV, 1.890*eV,
    1.902*eV, 1.908*eV, 1.917*eV, 1.926*eV, 1.932*eV,
    1.941*eV, 1.947*eV, 1.959*eV, 1.969*eV, 1.981*eV,
    1.994*eV, 2.004*eV, 2.010*eV, 2.020*eV, 2.027*eV,
    2.030*eV, 2.040*eV, 2.047*eV, 2.054*eV, 2.061*eV,
    2.071*eV, 2.081*eV, 2.088*eV, 2.099*eV, 2.110*eV,
    2.135*eV, 2.154*eV, 2.161*eV, 2.177*eV, 2.212*eV,
    2.244*eV, 2.273*eV, 2.285*eV, 2.302*eV, 2.311*eV,
    2.320*eV, 2.333*eV, 2.359*eV, 2.377*eV, 2.391*eV,
    2.410*eV, 2.424*eV, 2.443*eV, 2.458*eV, 2.478*eV,
    2.513*eV, 2.572*eV, 2.594*eV, 2.616*eV, 2.632*eV,
    2.649*eV, 2.666*eV, 2.678*eV, 2.695*eV, 2.701*eV,
    2.731*eV, 2.749*eV, 2.768*eV, 2.792*eV, 2.811*eV,
    2.824*eV, 2.831*eV, 2.850*eV, 2.877*eV, 2.904*eV,
    2.910*eV, 2.931*eV, 2.952*eV, 2.980*eV, 3.017*eV,
    3.046*eV, 3.069*eV, 3.092*eV, 3.123*eV, 3.155*eV,
    3.212*eV, 3.271*eV, 3.315*eV, 3.378*eV, 3.454*eV,
    3.522*eV};

  double wls_abs_length[nentries1] = {
    71.2971*cm,   117.49*cm,     146.611*cm,     181.757*cm,      211.883*cm,
    224.937*cm,   207.866*cm,    204.854*cm,     188.787*cm,      174.728*cm,
    155.649*cm,   139.582*cm,    128.536*cm,     131.548*cm,      141.59*cm,
    152.636*cm,   167.699*cm,    185.774*cm,     198.828*cm,      204.854*cm,
    200.837*cm,   187.782*cm,    165.69*cm,      123.515*cm,      85.3556*cm,
    67.2803*cm,   61.2552*cm,    63.2636*cm,     69.2887*cm,      86.3598*cm,
    111.464*cm,   139.582*cm,    156.653*cm,     168.703*cm,      178.745*cm,
    177.741*cm,   166.695*cm,    160.669*cm,     152.636*cm,      144.603*cm,
    136.569*cm,   129.54*cm,     119.498*cm,     108.452*cm,      99.4142*cm,
    88.3682*cm,   82.3431*cm,    84.3515*cm,     81.3389*cm,      74.3096*cm,
    65.272*cm,    56.2343*cm,    42.1757*cm,     31.1297*cm,      22.0921*cm,
    11.046*cm,    1.64583*cm,    0.51974*cm,     0.214673*cm,     0.121914*cm,
    0.0742481*cm, 0.0539618*cm,  0.0416667*cm,   0.0337031*cm,    0.0298338*cm,
    0.0277388*cm, 0.029216*cm,   0.0309561*cm,   0.0321661*cm,    0.0317524*cm,
    0.0301988*cm, 0.0278955*cm,  0.0261243*cm,   0.025*cm,        0.0261936*cm,
    0.0282951*cm, 0.0321661*cm,  0.0347711*cm,   0.0387255*cm,    0.0404713*cm,
    0.0418432*cm, 0.046801*cm,   0.0536685*cm,   0.0671769*cm,    0.0822918*cm,
    0.109722*cm,  0.147388*cm,   0.205729*cm,    0.308594*cm,     0.448864*cm,
    0.548611*cm};

  const unsigned nentries2   = 42;
  double photonE2[nentries2] = {
    1.993*eV, 2.029*eV, 2.070*eV, 2.109*eV, 2.153*eV,
    2.187*eV, 2.222*eV, 2.246*eV, 2.271*eV, 2.305*eV,
    2.331*eV, 2.353*eV, 2.366*eV, 2.384*eV, 2.394*eV,
    2.407*eV, 2.417*eV, 2.431*eV, 2.445*eV, 2.460*eV,
    2.475*eV, 2.490*eV, 2.510*eV, 2.520*eV, 2.535*eV,
    2.546*eV, 2.562*eV, 2.572*eV, 2.583*eV, 2.594*eV,
    2.605*eV, 2.616*eV, 2.627*eV, 2.644*eV, 2.661*eV,
    2.666*eV, 2.678*eV, 2.689*eV, 2.701*eV, 2.719*eV,
    2.749*eV, 2.780*eV };

  double wls_emission[nentries2] = {
    0.00505051, 0.012626,    0.0252525,    0.035353,     0.0555556,
    0.0782828,  0.126263,    0.164141,     0.222222,     0.270202,
    0.315657,   0.373737,    0.444444,     0.515152,     0.580808,
    0.65404,    0.719697,    0.762626,     0.792929,     0.777778,
    0.747475,   0.70202,     0.686869,     0.69697,      0.739899,
    0.787879,   0.858586,    0.919192,     0.969697,     1,
    0.984848,   0.924242,    0.815657,     0.64899,      0.517677,
    0.39899,    0.287879,    0.186869,     0.103535,     0.0530303,
    0.0151515,  0 };

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",        photonE, refrac_idx,     nentries );
  table->AddProperty( "ABSLENGTH",     photonE, abs_length,     nentries );
  table->AddProperty( "WLSABSLENGTH", photonE1, wls_abs_length, nentries1 );
  table->AddProperty( "WLSCOMPONENT", photonE2, wls_emission,   nentries2 );
  table->AddConstProperty( "WLSTIMECONSTANT", 0.5*ns );
  material->SetMaterialPropertiesTable( table );

  return material;
}

G4Material*
Make_FiberInner()
{
  // --------------------------------------------------
  // Y11(200) inner clade PMMA
  // --------------------------------------------------
  G4Material* material
    = new G4Material( "FiberInnerCladding", 1.190*g/cm3, 3, kStateSolid );
  material->AddElement( C, 60.57*perCent );
  material->AddElement( H,  8.05*perCent );
  material->AddElement( O, 31.38*perCent );
  static const unsigned nentries3     = 2;
  static double photonE[nentries3]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries3] = {1.49, 1.49};
  static double abs_length[nentries3] = {20.0*m, 20.0*m};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty(
    "RINDEX",    photonE, refrac_idx, nentries3 );
  table->AddProperty(
    "ABSLENGTH", photonE, abs_length,       nentries3 );
  material->SetMaterialPropertiesTable( table );

  return material;
}

G4Material*
Make_FiberOuter()
{
  // --------------------------------------------------
  // Double Cladding (fluorinated polyethylene)  Note: Y11(200) used Fluorinated
  // polymer (FP)
  // --------------------------------------------------
  G4Material* material
    = new G4Material( "FiberOuterCladding", 1.43*g/cm3, 2, kStateSolid );
  material->AddElement( C, 85.63*perCent );
  material->AddElement( H, 14.37*perCent );
  static const unsigned nentries     = 2;
  static double photonE[nentries]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries] = {1.42, 1.42};
  static double abs_length[nentries] = {20.0*m, 20.0*m};

  G4MaterialPropertiesTable* table
    = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",    photonE, refrac_idx, nentries );
  table->AddProperty( "ABSLENGTH", photonE, abs_length, nentries );
  material->SetMaterialPropertiesTable( table );

  return material;
}


G4Material*
Make_SCSN81( const double abslength, const double induced_Mu )
{
  G4Material* material
    = G4NistManager::Instance()->FindOrBuildMaterial( "G4_POLYSTYRENE" );
  const unsigned nentries     = 2;
  double photonE[nentries]    = {1.0*eV, 6.0*eV};
  double refrac_idx[nentries] = {1.59, 1.59};

  double mu              = 1/abslength + induced_Mu;
  double total_absLength = 1 / mu;

  double abs_length[nentries] = {total_absLength, total_absLength};
  // Add entries into properties table
  G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
  MPT->AddProperty( "RINDEX",    photonE, refrac_idx, nentries );
  MPT->AddProperty( "ABSLENGTH", photonE, abs_length, nentries );
  material->SetMaterialPropertiesTable( MPT );

  return material;
}


G4Material*
Make_EJ200( const double mult )
{
  // material = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); // H:8.5%; C: 91.5%; density = 1.032 g/cm^3
  G4Material* material = new G4Material( "EJ200", 1.023*g/cm3, 2, kStateSolid );
  material->AddElement( C, 91.53*perCent );
  material->AddElement( H,  8.47*perCent );

  static const unsigned nentries = 57;
  // ~350 to 550nm
  static double photonE[nentries] = {
    3.542405514*eV, 3.492512479*eV, 3.444005361*eV, 3.396827205*eV,
    3.350924135*eV, 3.306245147*eV, 3.262741921*eV, 3.220368649*eV,
    3.179081872*eV, 3.170951228*eV, 3.162862066*eV, 3.154814071*eV,
    3.146806929*eV, 3.138840329*eV, 3.130913965*eV, 3.123027531*eV,
    3.115180729*eV, 3.107373258*eV, 3.099604825*eV, 3.091875137*eV,
    3.084183905*eV, 3.076530844*eV, 3.068915668*eV, 3.061338099*eV,
    3.053797857*eV, 3.046294668*eV, 3.038828260*eV, 3.031398362*eV,
    3.024004707*eV, 2.987570916*eV, 2.952004595*eV, 2.917275129*eV,
    2.883353326*eV, 2.850211333*eV, 2.817822568*eV, 2.786161640*eV,
    2.755204289*eV, 2.724927319*eV, 2.695308543*eV, 2.666326731*eV,
    2.637961553*eV, 2.610193537*eV, 2.583004021*eV, 2.556375113*eV,
    2.530289653*eV, 2.504731172*eV, 2.479683860*eV, 2.455132535*eV,
    2.431062608*eV, 2.407460058*eV, 2.384311404*eV, 2.361603676*eV,
    2.339324396*eV, 2.317461551*eV, 2.296003574*eV, 2.274939321*eV,
    2.254258055*eV
  };

  static double refrac_idx[nentries] = {
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
    1.58, 1.58
  };

  static double Normabs_length[nentries] = {
    0.5743617028*mm, 0.5767800495*mm,  0.5773794872*mm,   0.5780135803*mm,
    0.5789505759*mm, 0.582016903*mm,   0.5830349115*mm,   0.5975989105*mm,
    0.729882704*mm,  0.7881381067*mm,  0.8633691117*mm,   0.9585952443*mm,
    1.079257244*mm,  1.228571927*mm,   1.419603272*mm,    1.660828418*mm,
    1.972327621*mm,  2.372104668*mm,   2.896569657*mm,    3.584178546*mm,
    4.500608885*mm,  5.707467479*mm,   7.335457013*mm,    9.476300059*mm,
    12.39047472*mm,  16.11698146*mm,   21.00255358*mm,    26.86579025*mm,
    34.26737294*mm,  76.31922807*mm,   105.842978*mm,     125.2249369*mm,
    137.1644668*mm,  145.9649899*mm,   152.6773991*mm,    162.9471828*mm,
    170.5827376*mm,  177.8360165*mm,   188.3857227*mm,    203.2702981*mm,
    206.9511627*mm,  209.597121*mm,    218.800288*mm,     228.2574561*mm,
    241.3501381*mm,  252.1649689*mm,   267.0613552*mm,    280.9645483*mm,
    293.2338149*mm,  304.009777*mm,    324.5206616*mm,    339.7711016*mm,
    357.9664491*mm,  370.3212829*mm,   393.1346279*mm,    426.5560315*mm,
    434.4316833*mm
  };

  double abs_length[nentries];

  for( unsigned i = 0; i < nentries; i++ ){
    abs_length[i] = Normabs_length[i] * mult;
  }

  // Add entries into properties table
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",    photonE, refrac_idx, nentries );
  table->AddProperty( "ABSLENGTH", photonE, abs_length, nentries );
  table->AddConstProperty( "SCINTILLATIONYIELD",         10./keV );

  table->AddConstProperty( "RESOLUTIONSCALE",            1.0 );
  table->AddConstProperty( "FASTSCINTILLATIONRISETIME",  0.9*ns );

  // Disabled for CMSSW
  //table->AddConstProperty( "ELECTRONSCINTILLATIONYIELD", 10./keV );
  //table->AddConstProperty( "ALPHASCINTILLATIONYIELD",   100./MeV );
  //table->AddConstProperty( "FASTTIMECONSTANT",           2.1*ns );

  // table->AddProperty("FASTCOMPONENT",photonE,Scints,nentries);
  // table->AddConstProperty("YIELDRATIO",1.0);//Fast component/Tot scint
  material->SetMaterialPropertiesTable( table );

  // FIXME: Set the Birks Constant for the EJ200 scintillator
  material->GetIonisation()->SetBirksConstant( 0.126*mm/MeV );

  return material;
}

G4Material*
Make_EJ260( const double abs, const double induced_mu )
{
  G4Material* material = new G4Material( "EJ260", 1.023*g/cm3, 2, kStateSolid );
  material->AddElement( C, 91.49*perCent );
  material->AddElement( H,  8.51*perCent );

  static const unsigned nentries     = 2;
  static double photonE[nentries]    = {1.0*eV, 6.0*eV};
  static double refrac_idx[nentries] = {1.58, 1.58};

  double total_mu  = 1/abs + induced_mu;
  double absLength = 1 / total_mu;

  double abs_length[nentries]      = {absLength, absLength};
  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "RINDEX",    photonE, refrac_idx, nentries );
  table->AddProperty( "ABSLENGTH", photonE, abs_length, nentries );
  table->AddConstProperty( "SCINTILLATIONYIELD",        9.2/keV );  // ELJEN
  table->AddConstProperty( "RESOLUTIONSCALE",           1.0 );
  table->AddConstProperty( "FASTSCINTILLATIONRISETIME", 1.3*ns );// Rise time

  // Disabled for CMSSW
  // table->AddConstProperty( "FASTIMECONSTANT",           9.2*ns );
  // table->AddConstProperty("ELECTRONSCINTILLATIONYIELD",9.2/keV);
  // table->AddConstProperty("ALPHASCINTILLATIONYIELD",10.12/keV);

  material->SetMaterialPropertiesTable( table );

  // FIXME: Set the Birks Constant for the EJ260 scintillator
  material->GetIonisation()->SetBirksConstant( 0.126*mm/MeV );

  return material;
}
