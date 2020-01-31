#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimAnalysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimScintillation.hh"
#else
#include "LYSimAnalysis.hh"
#include "LYSimScintillation.hh"
#endif

#include "g4root.hh"
#include <G4VParticleChange.hh>

using namespace CLHEP;

LYSimScintillation::LYSimScintillation( const G4String & processName, G4ProcessType type )
  : G4Scintillation( processName, type )
{
  G4AnalysisManager::Instance();
}

LYSimScintillation::~LYSimScintillation()
{
}

G4VParticleChange*
LYSimScintillation::PostStepDoIt( const G4Track& aTrack, const G4Step& aStep )
{
  G4VParticleChange* result          = G4Scintillation::PostStepDoIt( aTrack, aStep );
  G4double depenergy                 = aStep.GetTotalEnergyDeposit();
  G4double TotEnergy                 = 0;
  G4ParticleDefinition* particleType = aTrack.GetDefinition();
  G4String particleName              = particleType->GetParticleName();
  G4AnalysisManager* man             = G4AnalysisManager::Instance();
  if( depenergy > 0.0 ){
    G4ThreeVector pos = aStep.GetPreStepPoint()->GetPosition();
    depenergy += TotEnergy;

    /****************************************************************************
       G4cout << "[LYSim] Energy Deposited keV: " << depenergy << G4endl;

       G4cout << "scintillating: "
           << std::setprecision( 4 )
           << std::setw( 10 ) << depenergy/keV << " keV of "
           << std::setw( 9 )
           << aStep.GetPreStepPoint()->GetKineticEnergy()/keV << " keV [kine] "
           << std::setw( 10 )
           << aStep.GetPreStepPoint()->GetTotalEnergy()/keV << " keV [total] "
           << "deposited at ("
           << std::fixed
           << std::setw( 7 ) << pos.x()/mm << " mm,"
           << std::setw( 7 ) << pos.y()/mm << " mm,"
           << std::setw( 7 ) << pos.z()/mm << " mm) "
           << "by parent ID " << std::setw( 5 ) << aTrack.GetTrackID() << " : "
           << std::setw( 5 ) << particleName << " "
           << "producing "
           << result->GetNumberOfSecondaries()
           << " optical photons" << G4endl
           << std::resetiosflags( std::ios::fixed );
     ***************************************************************************/
  }

  return result;
}
