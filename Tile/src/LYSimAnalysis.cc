#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimAnalysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimScintillation.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrajectoryPoint.hh"
#else
#include "LYSimAnalysis.hh"
#include "LYSimDetectorConstruction.hh"
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

// Explicit ROOT file includes can only been done in CMSSW
#include "TFile.h"

using namespace std;
using namespace CLHEP;

LYSimAnalysis* LYSimAnalysis::singleton = 0;

LYSimAnalysis::LYSimAnalysis()
{
}

LYSimAnalysis::~LYSimAnalysis()
{
}

void
LYSimAnalysis::PrepareExperiment()
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  file = TFile::Open(filename.c_str());
  file->Close();

  // Create histogram(s) (avoid non-integer bins)
  man->CreateH1( "ogammaE", "Optical photons Wavelength [nm]",      100, 200., 1200. );
  man->CreateH1( "Ndetect", "Number of detected photons per event", 500,   0.,  500. );
}

void
LYSimAnalysis::PrepareNewRun( const G4Run* )
{
  // Reset variables relative to the run
  PhotonCount = 0;
  HitCount    = 0;
}

void
LYSimAnalysis::PrepareNewEvent( const G4Event* event )
{
  std::cout << "Starting Event " << event->GetEventID() << std::endl;
}

void
LYSimAnalysis::EndOfEvent( const G4Event* anEvent )
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
    if( i == 0 ){
      HitCount++;
    }

    assert( ( *hits )[i]->GetPhotonCount() == 1 );
    EventEnergy      += HitEnergy;
    EventPhotonCount += ( *hits )[i]->GetPhotonCount();
    man->FillH1( 1, 1239.842/( HitEnergy/eV ) );
  }

  man->FillH1( 2, EventPhotonCount );
}


void
LYSimAnalysis::EndOfRun( const G4Run* )
{
  G4double detEff = PhotonCount > 0 ? (double)HitCount/(double)PhotonCount :
                    0.0;
  G4cout << "Efficiency in this run is " << detEff  << G4endl;
}

void
LYSimAnalysis::EndOfExperiment()
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  file->Close();
  man->Write();
  man->CloseFile();
}
