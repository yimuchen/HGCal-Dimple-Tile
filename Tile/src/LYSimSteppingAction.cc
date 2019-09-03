#include "LYSimDetectorConstruction.hh"
#include "LYSimPMTSD.hh"
#include "LYSimSteppingAction.hh"

#include "G4EventManager.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"

using namespace CLHEP;
// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LYSimSteppingAction* LYSimSteppingAction::fgInstance = 0;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LYSimSteppingAction*
LYSimSteppingAction::Instance()
{
  // Static acces function via G4RunManager

  return fgInstance;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LYSimSteppingAction::LYSimSteppingAction()
  : G4UserSteppingAction(),
  fVolume( 0 ),
  PhotonHitCount( 0 )
{
  fgInstance = this;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LYSimSteppingAction::~LYSimSteppingAction()
{
  fgInstance = 0;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
LYSimSteppingAction::UserSteppingAction( const G4Step* step )
{
  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4OpBoundaryProcess* boundary     = NULL;

  // find the boundary process only once
  if( !boundary ){
    G4ProcessManager* pm = step->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses     = pm->GetProcessListLength();
    G4ProcessVector* pv  = pm->GetProcessList();
    G4int i;

    for( i = 0; i < nprocesses; i++ ){
      if( ( *pv )[i]->GetProcessName() == "OpBoundary" ){
        boundary = (G4OpBoundaryProcess*)( *pv )[i];
        break;
      }
    }
  }

  if( boundary ){
    boundaryStatus = boundary->GetStatus();

    switch( boundaryStatus ){
    case Detection:     // Note, this assumes that the volume causing detection
      // is the photocathode because it is the only one with
      // non-zero efficiency
    {
      // Trigger sensitive detector manually since photon is
      // absorbed but status was Detection
      G4EventManager::GetEventManager()->KeepTheCurrentEvent();
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      G4String sdName    = "/LYSimPMT";
      LYSimPMTSD* pmtSD  = (LYSimPMTSD*)SDman->FindSensitiveDetector( sdName );
      if( pmtSD ){pmtSD->ProcessHits_constStep( step, NULL );}
      break;
    }
    default:
      break;
    }
  }
  // kill tracks with length > 5000mm
  G4double tracklength = step->GetTrack()->GetTrackLength();
  if( tracklength > 5000.*mm ){
    G4cout << "Track length exceeded limit of 5000 mm" << G4endl;
    step->GetTrack()->SetTrackStatus( fStopAndKill );
  }
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
LYSimSteppingAction::Reset()
{
  PhotonHitCount = 0;
}
