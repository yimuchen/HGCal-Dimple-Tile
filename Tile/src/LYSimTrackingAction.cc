#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimAnalysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrajectory.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrackingAction.hh"
#else
#include "LYSimAnalysis.hh"
#include "LYSimTrajectory.hh"
#include "LYSimTrackingAction.hh"
#endif

#include "globals.hh"
#include "G4RunManager.hh"

#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"


void
LYSimTrackingAction::PreUserTrackingAction( const G4Track* aTrack )
{
  fpTrackingManager->SetStoreTrajectory(true);
  // Use custom trajectory class
  fpTrackingManager->SetTrajectory( new LYSimTrajectory( aTrack ) );
}

void
LYSimTrackingAction::PostUserTrackingAction( const G4Track* aTrack )
{
  LYSimTrajectory* trajectory =
    (LYSimTrajectory*)fpTrackingManager->GimmeTrajectory();
  if( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ){
    if( aTrack->GetParentID() == 0 ){ trajectory->SetDrawTrajectory( true );} else {
      const G4VProcess* creator = aTrack->GetCreatorProcess();
      if( creator && creator->GetProcessName() == "OpWLS" ){
        trajectory->WLS();
        trajectory->SetDrawTrajectory( true );
      }
      if( creator && creator->GetProcessName() == "Scintillation" ){
        trajectory->SetDrawTrajectory( false );
      }
    }
  } else   {// draw all other trajectories
    trajectory->SetDrawTrajectory( true );
  }
}
