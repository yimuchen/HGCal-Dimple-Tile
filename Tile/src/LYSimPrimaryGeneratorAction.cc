#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/Analysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/ProjectPath.hh"
#else
#include "Analysis.hh"
#include "LYSimDetectorConstruction.hh"
#include "LYSimPrimaryGeneratorAction.hh"
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

LYSimPrimaryGeneratorAction::LYSimPrimaryGeneratorAction( LYSimDetectorConstruction* det )
{
  std::cout << "[LYSIM] entering LYSIMPrimaryGeneratorAction" << std::endl;
  fDetector = det;

  /**
   * This particle source will be directly controlled via the /gps commands In
   * user scripts and the ApplyCommand calls in the main function. We are going
   * to set up the common stuff here.
   */
  particleSource = new G4GeneralParticleSource();

  particleSource->SetParticleDefinition(
    G4OpticalPhoton::OpticalPhotonDefinition() );

}

LYSimPrimaryGeneratorAction::~LYSimPrimaryGeneratorAction()
{
  delete particleSource;
}

void
LYSimPrimaryGeneratorAction::InitSource()
{
  // For setting the via the interactive session, look at
  // http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/
  // ForApplicationDeveloper/html/GettingStarted/generalParticleSource.html

  // Setting default spacial distribution
  G4SPSPosDistribution* pos = particleSource->GetCurrentSource()->GetPosDist();
  pos->SetPosDisType( "Volume" );
  pos->SetPosDisShape( "Cylinder" );
  pos->SetRadius( 0.001*CLHEP::mm );
  pos->SetHalfZ( fDetector->GetTileZ() );
  // Covering more than necessary.
  pos->SetCentreCoords( G4ThreeVector( 0, 0, 0 ) );
  // pos->ConfineSourceToVolume( "TilePhysic" );

  std::cout << pos->GetHalfZ() << std::endl;
  std::cout << fDetector->GetTileZ() << std::endl;

  // Setting default angular distrution of particle  (isotropic)
  G4SPSAngDistribution* ang = particleSource->GetCurrentSource()->GetAngDist();
  ang->SetAngDistType( "iso" );

  // Energy distribution.
  G4SPSEneDistribution* ene = particleSource->GetCurrentSource()->GetEneDist();
  ene->SetEnergyDisType( "Arb" );
  ene->ArbEnergyHistoFile( project_base + "/Tile/data/PhotonSpectrum.dat" );
  ene->ArbInterpolate( "Lin" );
}

void
LYSimPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
  assert( particleSource->GetParticleDefinition()->GetParticleName()
    == "opticalphoton" );

  // Randomizing the photon polarization for each event
  const double angle = G4UniformRand() * 360.0*deg;
  const G4ThreeVector normal( 1., 0., 0. );
  const G4ThreeVector kphoton = particleSource->GetParticleMomentumDirection();
  const G4ThreeVector product = normal.cross( kphoton );
  const double modul2         = product*product;

  G4ThreeVector e_perpend( 0., 0., 1. );
  if( modul2 > 0. ){
    e_perpend = ( 1./std::sqrt( modul2 ) )*product;
  }
  G4ThreeVector e_paralle = e_perpend.cross( kphoton );
  G4ThreeVector polar     = std::cos( angle )*e_paralle
                            + std::sin( angle )*e_perpend;
  particleSource->SetParticlePolarization( polar );

  // We still need to generate the primary vertex.
  particleSource->GeneratePrimaryVertex( anEvent );
}

static double CalcThickness( const LYSimDetectorConstruction* detector,
                             const double x, const double y );
static int CalcNumPhotons( const double thickness );

void
LYSimPrimaryGeneratorAction::MoveSource( const double x_center,
                                         const double y_center,
                                         const double width )
{
  const float x = ( 2*double(rand()%1000000)/double(1000000)-1 )*width + x_center;
  const float y = ( 2*double(rand()%1000000)/double(1000000)-1 )*width + y_center;
  const float t = CalcThickness( fDetector, x, y );
  const int np  = CalcNumPhotons( t );

  G4SPSPosDistribution* pos = particleSource->GetCurrentSource()->GetPosDist();
  pos->SetHalfZ( t/2 );
  pos->SetCentreCoords( G4ThreeVector( x, y, t/2 ) );
  particleSource->GetCurrentSource()->SetNumberOfParticles( np );
  //pos->ConfineSourceToVolume( "TilePhysic" );
}

double
CalcThickness( const LYSimDetectorConstruction* detector,
               const double x, const double y )
{
  const double dimple_r  = detector->GetDimpleRadius();
  const double dimple_i  = detector->GetDimpleIndent();
  const double beam_r    = sqrt( x*x+y*y );
  const double big_r     = detector->GetDimpleSizeRadius();
  const double thickness = detector->GetTileZ();
  const int type         = detector->Get_dimple_type();

  if( beam_r <= dimple_r ){
    if( type == 0 ){
      return thickness - ( sqrt( big_r*big_r - beam_r*beam_r ) -
                           ( big_r - dimple_i ) );
    } else if( type == 2 ){//  # Parabolic
      return thickness - abs( 1.35 * beam_r*beam_r - dimple_i );
    } else if( type == 1 ){
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

int
CalcNumPhotons( const double thickness )
{
  return int(std::floor( (double)6677 * thickness/3.0 ) );
}
