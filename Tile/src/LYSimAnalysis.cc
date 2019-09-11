#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimAnalysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimScintillation.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrajectoryPoint.hh"
#else
#include "LYSimAnalysis.hh"
#include "LYSimDetectorConstruction.hh"
#include "LYSimFormat.hh"
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

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

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
  tree   = new TTree( "LYSim", "LYSim" );
  format = new LYSimFormat();
  format->AddToTree( tree );
  photon_energy_hist = new TH1D( "PhotonWLE", "", 100, 300., 600. );
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
  std::cout << "Starting Event " << event->GetEventID()
            << "("<< event->GetNumberOfPrimaryVertex()  << ")" << std::endl;
  std::cout << event->GetPrimaryVertex()->GetX0() << " "
            << event->GetPrimaryVertex()->GetY0() << std::endl;

  // Saving Event information.
  format->beam_center_x = generatorAction->GetBeamX();
  format->beam_center_y = generatorAction->GetBeamY();
  format->beam_width    = generatorAction->GetWidth();

  // This will be the center of the beam position.
  format->beam_x = event->GetPrimaryVertex()->GetX0();
  format->beam_y = event->GetPrimaryVertex()->GetY0();

  format->dimple_radius = DetectorConstruction->GetDimpleRadius();
  format->dimple_indent = DetectorConstruction->GetDimpleIndent();
  format->nphotons      = -1;
}

void
LYSimAnalysis::EndOfEvent( const G4Event* event )
{
  // Update number of photons if exists.
  format->nphotons = GetNPhotons( event );

  // Filling the three
  tree->Fill();
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
  TFile* file = TFile::Open( filename.c_str(), "RECREATE" );
  tree->Write();
  photon_energy_hist->Write();
  file->Close();

  delete tree;
  delete format;
  delete photon_energy_hist;
}

int
LYSimAnalysis::GetNPhotons( const G4Event* event )
{
  static G4SDManager* SDman = G4SDManager::GetSDMpointer();
  static G4int hitCollID    = SDman->GetCollectionID( "PMTHitsCollection" );

  G4HCofThisEvent* hitsCollections = event->GetHCofThisEvent();
  if( !hitsCollections ){
    G4cerr << "hitsCollection not found" << G4endl;
    return -1;
  }

  LYSimPMTHitsCollection* hits
    = static_cast<LYSimPMTHitsCollection*>(
        hitsCollections->GetHC( hitCollID ) );

  unsigned EventPhotonCount = 0;
  PhotonCount++;

  for( int i = 0; i < hits->entries(); ++i ){
    if( i == 0 ){ HitCount++; }
    const double HitEnergy = ( *hits )[i]->GetEnergy();
    assert( ( *hits )[i]->GetPhotonCount() == 1 );
    ++EventPhotonCount;
    photon_energy_hist->Fill( 1239.842/( HitEnergy/eV ) );
  }

  return EventPhotonCount;
}
