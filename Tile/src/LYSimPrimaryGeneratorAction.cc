#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimAnalysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorMessenger.hh"
#include "HGCalTileSim/Tile/interface/ProjectPath.hh"
#else
#include "LYSimAnalysis.hh"
#include "LYSimDetectorConstruction.hh"
#include "LYSimPrimaryGeneratorAction.hh"
#include "LYSimPrimaryGeneratorMessenger.hh"
#include "ProjectPath.hh"
#endif

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4UnitsTable.hh"

#include <CLHEP/Units/PhysicalConstants.h>

// Static helper functions.
static double CalcNumPhotons( const double thickness );

LYSimPrimaryGeneratorAction::LYSimPrimaryGeneratorAction( LYSimDetectorConstruction* det ) :
  particleSource( new G4GeneralParticleSource() ),
  fDetector( det ),
  messenger( new LYSimPrimaryGeneratorMessenger( this ) ),
  _beamx( 0 ),
  _beamy( 0 ),
  _width( 0 ),
  _photon_multiplier( 1 )
{
  std::cout << "[LYSIM] entering LYSIMPrimaryGeneratorAction" << std::endl;

  /**
   * This particle source will be directly controlled via the /gps commands In
   * user scripts and the ApplyCommand calls in the main function. We are going
   * to set up the common stuff here. For setting the via the interactive
   * session, look at:
   * http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/
   * ForApplicationDeveloper/html/GettingStarted/generalParticleSource.html
   */
  //  particleSource->SetParticleDefinition(
  //    G4OpticalPhoton::OpticalPhotonDefinition() );
  particleSource->SetMultipleVertex( true );

  /*
  // Setting default spacial distribution
  G4SPSPosDistribution* pos = particleSource->GetCurrentSource()->GetPosDist();
  pos->SetPosDisType( "Volume" );
  pos->SetPosDisShape( "Cylinder" );
  pos->SetRadius( 0.0001*CLHEP::mm );
  // Be default, move source to center of tile with no spread adjustments.
  pos->SetHalfZ( 1*CLHEP::mm );
  pos->SetCentreCoords( G4ThreeVector( _beamx, _beamy, 0 ) );

  // Setting default angular distrution of particle  (isotropic)
  G4SPSAngDistribution* ang = particleSource->GetCurrentSource()->GetAngDist();
  ang->SetAngDistType( "iso" );

  // Energy distribution.
  std::cout << project_base << std::endl;//
  G4SPSEneDistribution* ene = particleSource->GetCurrentSource()->GetEneDist();
  ene->SetEnergyDisType( "Arb" );
  ene->ArbEnergyHistoFile( project_base + "/data/PhotonSpectrum.dat" );
  ene->ArbInterpolate( "Lin" );
  */
}

LYSimPrimaryGeneratorAction::~LYSimPrimaryGeneratorAction()
{
  delete particleSource;
}

void
LYSimPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
  RandomizePosition();

  assert( particleSource->GetParticleDefinition()->GetParticleName()
    == "opticalphoton" );

  // We still need to generate the primary vertex.
  particleSource->GeneratePrimaryVertex( anEvent );
}

void
LYSimPrimaryGeneratorAction::RandomizePosition()
{
  particleSource->ClearAll();
  // Randomizing position of the the source for each event.
  const double x = ( 2*G4UniformRand()-1 )*_width + _beamx;
  const double y = ( 2*G4UniformRand()-1 )*_width + _beamy;
  const double z = fDetector->GetTileZ();
  const double t = fDetector->LocalTileZ( x, y );
  const int np   = _photon_multiplier * CalcNumPhotons( t );

  for( int i = 0; i < np; ++i ){
    // Creating a new particle source with just 1 photon
    particleSource->AddaSource( 1 );
    particleSource->GetCurrentSource()->SetNumberOfParticles( 1 );
    particleSource->GetCurrentSource()->SetParticleDefinition(
    G4OpticalPhoton::OpticalPhotonDefinition() );

    // Setting default spacial distribution
    G4SPSPosDistribution* pos = particleSource->GetCurrentSource()->GetPosDist();
    pos->SetPosDisType( "Volume" );
    pos->SetPosDisShape( "Cylinder" );
    pos->SetRadius( 0.0001*CLHEP::mm );
    pos->SetHalfZ( t/2 );
    pos->SetCentreCoords( G4ThreeVector( x, y, -z/2 + t/2 ) );

    // Setting default angular distrution of particle  (isotropic)
    G4SPSAngDistribution* ang = particleSource->GetCurrentSource()->GetAngDist();
    ang->SetAngDistType( "iso" );

    // Energy distribution.
    G4SPSEneDistribution* ene = particleSource->GetCurrentSource()->GetEneDist();
    ene->SetEnergyDisType( "Arb" );
    ene->ArbEnergyHistoFile( project_base + "/data/PhotonSpectrum.dat" );
    ene->ArbInterpolate( "Lin" );

    // Randomizing the polarization.
    const double angle = G4UniformRand() * 360.0*deg;
    const G4ThreeVector normal( 1., 0., 0. );
    const G4ThreeVector kphoton   = particleSource->GetParticleMomentumDirection();
    const G4ThreeVector product   = normal.cross( kphoton );
    const double modul2           = product*product;
    const G4ThreeVector e_perpend = ( 1./std::sqrt( modul2 ) )*product;
    const G4ThreeVector e_paralle = e_perpend.cross( kphoton );
    const G4ThreeVector polar     = std::cos( angle )*e_paralle
                                    + std::sin( angle )*e_perpend;
    particleSource->SetParticlePolarization( polar );
  }

}

double
CalcNumPhotons( const double thickness )
{
  return 6677.0 * thickness/3.0;
}
