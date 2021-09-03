#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;

static const unsigned flatentries = 2;
static const double minenergy     = 1.0*eV;
static const double maxenergy     = 8.0*eV;


G4OpticalSurface*
MakeS_TyvekCrystal()
{
  const unsigned num       = 2;
  double Ephoton[num]      = {1.5*eV, 8.0*eV};
  double Reflectivity[num] = {0.979, 0.979};

  //////////////////////////////////
  // Realistic Crystal-Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "TyvekOpSurface" );
  surface->SetType( dielectric_LUT );
  surface->SetModel( LUT );
  surface->SetFinish( polishedtyvekair );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "REFLECTIVITY", Ephoton, Reflectivity, num );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_ESR()
{
  const unsigned num       = 2;
  double Ephoton[num]      = {1.5*eV, 8.0*eV};
  double Reflectivity[num] = {0.985, 0.985};
  // source: https://www.osti.gov/servlets/purl/1184400
  //////////////////////////////////
  // ESR surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "ESROpSurface" );
  surface->SetType( dielectric_LUT );
  surface->SetModel( LUT );
  surface->SetFinish( polishedvm2000air );


  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "REFLECTIVITY", Ephoton, Reflectivity, num );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealTyvekCrystal()
{
  //////////////////////////////////
  // Ideal Crystal-Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "IdealTyvekOpSurface" );
  surface->SetType( dielectric_LUT );
  surface->SetModel( LUT );
  surface->SetFinish( polishedtyvekair );

  return surface;
}

G4OpticalSurface*
MakeS_UnifiedTyvek()
{
  const unsigned nentries        = 2;
  double phoE[nentries]          = {2.0*eV, 6.0*eV};
  double specularlobe[nentries]  = {1.0, 1.0};
  double specularspike[nentries] = {0., 0.};
  double backscatter[nentries]   = {0., 0.};
  double rindex[nentries]        = {1.0, 1.0};
  double reflectivity[nentries]  = {0.979, 0.979};
  double efficiency[nentries]    = {0.0, 0.0};

  //////////////////////////////////
  // Unified Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "UnifiedTyvekOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( groundbackpainted );
  surface->SetSigmaAlpha( 1.3*degree );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "RINDEX",                phoE, rindex,        nentries );
  table->AddProperty( "SPECULARLOBECONSTANT",  phoE, specularlobe,  nentries );
  table->AddProperty( "SPECULARSPIKECONSTANT", phoE, specularspike, nentries );
  table->AddProperty( "BACKSCATTERCONSTANT",   phoE, backscatter,   nentries );
  table->AddProperty( "REFLECTIVITY",          phoE, reflectivity,  nentries );
  table->AddProperty( "EFFICIENCY",            phoE, efficiency,    nentries );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealTyvek()
{
  const unsigned nentries        = 2;
  double phoE[nentries]          = {2.0*eV, 6.0*eV};
  double specularlobe[nentries]  = {1.0, 1.0};
  double specularspike[nentries] = {0., 0.};
  double backscatter[nentries]   = {0., 0.};
  double rindex[nentries]        = {1.0, 1.0};
  double reflectivity[nentries]  = {1.0, 1.0};
  double efficiency[nentries]    = {0.0, 0.0};

  //////////////////////////////////
  // Unified Ideal Tyvek surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface(
    "UnifiedIdealTyvekOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( groundbackpainted );
  surface->SetSigmaAlpha( 1.3*degree );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "RINDEX",                phoE, rindex,        nentries );
  table->AddProperty( "SPECULARLOBECONSTANT",  phoE, specularlobe,  nentries );
  table->AddProperty( "SPECULARSPIKECONSTANT", phoE, specularspike, nentries );
  table->AddProperty( "BACKSCATTERCONSTANT",   phoE, backscatter,   nentries );
  table->AddProperty( "REFLECTIVITY",          phoE, reflectivity,  nentries );
  table->AddProperty( "EFFICIENCY",            phoE, efficiency,    nentries );

  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_Polished()
{
  static const unsigned nentries       = 2;
  static double phoE[nentries]         = {1.0*eV, 6.0*eV};
  static double specularlobe[nentries] = {1.0, 1.0};

  //////////////////////////////////
  // Realistic polished surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "PolishedOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( ground );
  // necessary even for polished surfaces to enable UNIFIED code
  surface->SetSigmaAlpha( 1.3 * degree );// Janecek2010 (1.3 * degree)

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty(
    "SPECULARLOBECONSTANT", phoE, specularlobe, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealPolished()
{
  //////////////////////////////////
  // Ideal polished surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "IdealOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( glisur );
  surface->SetFinish( polished );

  return surface;
}

G4OpticalSurface*
MakeS_Mirror()
{
  const unsigned nentries       = 2;
  double phoE[nentries]         = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {0.9, 0.9};
  //////////////////////////////////
  // Mirror surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "MirrorOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( polished );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealMirror()
{
  const unsigned nentries       = 2;
  double phoE[nentries]         = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {1.0, 1.0};
  //////////////////////////////////
  // Ideal mirror surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "MirrorOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( polished );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_IdealWhiteSurface()
{
  //////////////////////////////////
  // Ideal mirror surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "WhiteOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( ground );
  surface->SetModel( unified );

  double phoE[flatentries]         = {minenergy, maxenergy};
  double reflectivity[flatentries] = {0.80, 0.80};

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, flatentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_Absorbing()
{
  const unsigned nentries       = 2;
  double phoE[nentries]         = {1.0*eV, 6.0*eV};
  double reflectivity[nentries] = {0.0, 0.0};
  //////////////////////////////////
  // Absorbing surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "AbsorbingOpSurface" );
  surface->SetType( dielectric_dielectric );
  surface->SetFinish( groundfrontpainted );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
  table->AddProperty( "REFLECTIVITY", phoE, reflectivity, nentries );
  surface->SetMaterialPropertiesTable( table );

  return surface;
}

G4OpticalSurface*
MakeS_RoughInterface( const double alpha )
{
  char name[1024];
  sprintf( name, "RoughInterfaceOpSurface%.lf", alpha );

  //////////////////////////////////
  // Rough surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( name );
  surface->SetType( dielectric_dielectric );
  surface->SetModel( unified );
  surface->SetFinish( ground );
  surface->SetSigmaAlpha( alpha );

  return surface;
}

G4OpticalSurface*
MakeS_RoughMirror()
{
  //////////////////////////////////
  // Rough surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "RoughMirrorOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetModel( unified );
  surface->SetFinish( ground );

  return surface;
}

G4OpticalSurface*
MakeS_SiPM( const double alpha )
{
  // From Meeting on HB SiPM selection (James Hirschauer)
  const unsigned nentries = 51;
  double phoE[nentries]   = {
    4.456977245*eV, 4.405663883*eV, 4.371335649*eV,  4.337993527*eV,
    4.305156186*eV, 4.272812248*eV, 4.120034327*eV,  4.047934735*eV,
    3.885068561*eV, 3.795976762*eV, 3.710990512*eV,  3.641025285*eV,
    3.562661791*eV, 3.416389546*eV, 3.254178294*eV,  3.200252775*eV,
    3.130913965*eV, 3.072870849*eV, 3.016867241*eV,  2.948003733*eV,
    2.868012792*eV, 2.772393126*eV, 2.622893865*eV,  2.488842802*eV,
    2.467789116*eV, 2.382432947*eV, 2.325721122*eV,  2.276066914*eV,
    2.224250888*eV, 2.174741594*eV, 2.115879533*eV,  2.067333517*eV,
    2.024496146*eV, 1.98339801*eV,  1.943935293*eV,  1.906041585*eV,
    1.869568783*eV, 1.834465614*eV, 1.80065635*eV,   1.76540215*eV,
    1.731477711*eV, 1.696391875*eV, 1.66504429*eV,   1.628029216*eV,
    1.594775069*eV, 1.562852229*eV, 1.546342471*eV,  1.512389673*eV,
    1.477990547*eV, 1.443354983*eV, 1.40859115*eV
  };
  double efficiency[nentries] = {
    0.011045, 0.024287, 0.044945,  0.058187,  0.071429,  0.084672,   0.119633,
    0.132347, 0.155127, 0.167841,  0.180556,  0.19327,   0.205984,   0.216053,
    0.225064, 0.237778, 0.257379,  0.270093,  0.282808,  0.294993,   0.307179,
    0.317778, 0.320435, 0.317796,  0.316738,  0.307211,  0.295563,   0.283914,
    0.271736, 0.260618, 0.246852,  0.235734,  0.224086,  0.212438,   0.20079,
    0.189142, 0.177494, 0.165846,  0.154198,  0.14255,   0.131432,   0.120315,
    0.109197, 0.09755,  0.086962,  0.076375,  0.071611,  0.061554,   0.053086,
    0.044618, 0.036681};

  const unsigned ref_ent = 32;
  double phoE2[ref_ent]  = {
    4.928613174*eV, 4.760932071*eV, 4.57675131 *eV, 4.416020551*eV,
    4.27782469 *eV, 4.132944198*eV, 3.988297134*eV, 3.870877084*eV,
    3.751866883*eV, 3.647774074*eV, 3.538260695*eV, 3.441997529*eV,
    3.347666946*eV, 3.264460058*eV, 3.176475533*eV, 3.098752668*eV,
    3.019365195*eV, 2.751902006*eV, 2.432636667*eV, 2.254544997*eV,
    2.136148464*eV, 2.032627719*eV, 1.907273067*eV, 1.796976535*eV,
    1.722050515*eV, 1.570692624*eV, 1.511965476*eV, 1.459049532*eV,
    1.377862407*eV, 1.305495288*eV, 1.265726027*eV, 1.240350474*eV
  };
  double reflectivity[ref_ent] = {
    0.67091, 0.70309, 0.73346, 0.72132, 0.68334, 0.62105, 0.59036, 0.57232,
    0.56450, 0.56008, 0.56441, 0.58236, 0.58280, 0.54482, 0.51025, 0.48443,
    0.46541, 0.41855, 0.38169, 0.36570, 0.35705, 0.35096, 0.34397, 0.33772,
    0.33321, 0.32645, 0.32498, 0.32241, 0.31988, 0.31625, 0.31417, 0.31323
  };

  G4OpticalSurface* surface = new G4OpticalSurface( "SiPM_Surface" );
  surface->SetType( dielectric_metal );
  surface->SetModel( unified );
  surface->SetFinish( ground );
  surface->SetSigmaAlpha( alpha );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "EFFICIENCY",   phoE,  efficiency,   nentries );
  table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, ref_ent  );
  surface->SetMaterialPropertiesTable( table );
  return surface;
}

G4OpticalSurface*
MakeS_PCBSurface()
{
  // PCB is a flat gray surface for now
  double phoE2[flatentries]        = {minenergy, maxenergy };
  double reflectivity[flatentries] = {0.5, 0.5};

  G4OpticalSurface* surface = new G4OpticalSurface( "PCB_Surface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( ground );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, flatentries );
  surface->SetMaterialPropertiesTable( table );
  return surface;
}

void
Update_SiPMSurfaceRef( G4OpticalSurface* s, const double x )
{
  static const unsigned ref_ent = 32;
  static double phoE2[ref_ent]  = {
    4.928613174*eV, 4.760932071*eV, 4.57675131 *eV, 4.416020551*eV,
    4.27782469 *eV, 4.132944198*eV, 3.988297134*eV, 3.870877084*eV,
    3.751866883*eV, 3.647774074*eV, 3.538260695*eV, 3.441997529*eV,
    3.347666946*eV, 3.264460058*eV, 3.176475533*eV, 3.098752668*eV,
    3.019365195*eV, 2.751902006*eV, 2.432636667*eV, 2.254544997*eV,
    2.136148464*eV, 2.032627719*eV, 1.907273067*eV, 1.796976535*eV,
    1.722050515*eV, 1.570692624*eV, 1.511965476*eV, 1.459049532*eV,
    1.377862407*eV, 1.305495288*eV, 1.265726027*eV, 1.240350474*eV
  };
  static double default_reflectivity[ref_ent] = {
    0.67091, 0.70309, 0.73346, 0.72132, 0.68334, 0.62105, 0.59036, 0.57232,
    0.56450, 0.56008, 0.56441, 0.58236, 0.58280, 0.54482, 0.51025, 0.48443,
    0.46541, 0.41855, 0.38169, 0.36570, 0.35705, 0.35096, 0.34397, 0.33772,
    0.33321, 0.32645, 0.32498, 0.32241, 0.31988, 0.31625, 0.31417, 0.31323
  };

  double reflectivity[ref_ent];

  for( unsigned i = 0; i < ref_ent; ++i ){
    reflectivity[i ] = default_reflectivity[i] * x;
  }

  // Add entries into properties table
  G4MaterialPropertiesTable* table = s->GetMaterialPropertiesTable();
  table->RemoveProperty( "REFLECTIVITY" );
  table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, ref_ent );
}

void
Update_SiPMSurfaceAlpha( G4OpticalSurface* s, const double alpha )
{
  s->SetSigmaAlpha( alpha );
}

