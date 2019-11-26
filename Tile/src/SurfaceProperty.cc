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
  double reflectivity[flatentries] = {0.70, 0.70};

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
MakeS_Rough()
{
  //////////////////////////////////
  // Rough surface
  //////////////////////////////////
  G4OpticalSurface* surface = new G4OpticalSurface( "RoughOpSurface" );
  surface->SetType( dielectric_metal );
  surface->SetFinish( ground );
  surface->SetModel( unified );

  return surface;
}

G4OpticalSurface*
MakeS_SiPM()
{
  // From Meeting on HB SiPM selection (James Hirschauer)
  const unsigned nentries = 49;
  double phoE[nentries]   = {
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

  double efficiency[nentries] = {
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
  // SiPM reflectivity paper: Using a flat 0.12 for now
  // https://reader.elsevier.com/reader/sd/pii/S016890021831297X?token=FADE142C43F500DEE5289D65B9D6573497F4AD220F3462EBA9239256CFD8E11FF98FF84E302016EAB89FA1B62B1EEDCE
  double phoE2[flatentries]        = {minenergy, maxenergy };
  double reflectivity[flatentries] = {0.12, 0.12};

  G4OpticalSurface* surface = new G4OpticalSurface( "SiPM_Surface" );
  surface->SetType( dielectric_metal );
  surface->SetModel( unified );

  G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();

  table->AddProperty( "EFFICIENCY",   phoE,  efficiency,   nentries );
  table->AddProperty( "REFLECTIVITY", phoE2, reflectivity, flatentries );
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

