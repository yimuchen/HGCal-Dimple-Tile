#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/Analysis.hh"
#include "HGCalTileSim/Tile/interface/AnalysisMessenger.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimEventAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimScintillation.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrajectoryPoint.hh"
#else
#include "Analysis.hh"
#include "AnalysisMessenger.hh"
#include "LYSimDetectorConstruction.hh"
#include "LYSimEventAction.hh"
#include "LYSimPrimaryGeneratorAction.hh"
#include "LYSimScintillation.hh"
#include "LYSimTrajectoryPoint.hh"
#endif

#include "g4root.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4PrimaryVertex.hh"
#include "G4RootAnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"


using namespace std;
using namespace CLHEP;

Analysis* Analysis::singleton = 0;

Analysis::Analysis()
{
  fMessenger = new AnalysisMessenger( this );
  G4AnalysisManager::Instance();

}

Analysis::~Analysis()
{
  if( fMessenger ){ delete fMessenger;}
}

void
Analysis::PrepareNewEvent( const G4Event* )
{
}

void
Analysis::EndOfEvent( const G4Event* anEvent )
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  G4String hitCollName   = "PMTHitsCollection";
  G4SDManager* SDman     = G4SDManager::GetSDMpointer();
  static G4int hitCollID = -1;
  if( hitCollID < 0 ){
    hitCollID = SDman->GetCollectionID( hitCollName );
  }

  G4HCofThisEvent* hitsCollections = 0;
  hitsCollections = anEvent->GetHCofThisEvent();

  LYSimPMTHitsCollection* hits = 0;
  if( hitsCollections ){
    hits = static_cast<LYSimPMTHitsCollection*>(
      hitsCollections->GetHC( hitCollID ) );
  } else {
    G4cerr << "hitsCollection not found" << G4endl;
    return;
  }

  G4double EventEnergy   = 0;
  G4int EventPhotonCount = 0;
  G4double nHits         = hits->entries();
  PhotonCount++;

  for( G4int i = 0; i < nHits; i++ ){
    G4double HitEnergy = ( *hits )[i]->GetEnergy();
    G4double HitTime   = ( *hits )[i]->GetTime();
    if( i == 0 ){
      HitCount++;
      // man->AddNtupleRow(); asdf
    }

    assert( ( *hits )[i]->GetPhotonCount() == 1 );
    EventEnergy      += HitEnergy;
    EventPhotonCount += ( *hits )[i]->GetPhotonCount();
    man->FillH1( 1, 1239.842/( HitEnergy/eV ) );
    man->FillH1( 4, HitTime/ns );
  }

  man->FillH1( 2, EventPhotonCount );
  man->FillH1( 3, EventEnergy/eV );
}

void
Analysis::PrepareNewRun( const G4Run* )
{
  // Reset variables relative to the run
  PhotonCount = 0;
  HitCount    = 0;
}


void
Analysis::EndOfRun( const G4Run* )
{
  G4double detEff = PhotonCount > 0 ? (double)HitCount/(double)PhotonCount :
                    0.0;
  G4cout << "Efficiency in this run is " << detEff  << G4endl;
  G4cout << inducedMuTile << "\t"
         << inducedMuFiber << "\t"
         << detEff <<  G4endl;
}
