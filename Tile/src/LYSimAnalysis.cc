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
#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

#include <signal.h>

using namespace std;
using namespace CLHEP;

static void
CloseAll( int signal )
{
  LYSimAnalysis::GetInstance()->EndOfExperiment();
  std::exit( signal );
}


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
  photon_energy_hist     = new TH1D( "PhotonWLE", "", 100, 300., 600. );
  photon_track_hist      = new TH1D( "PhotonTrack", "", 500, 0.01, 500 );
  photon_all_track_hist  = new TH1D( "AllPhotonTrack", "", 500, 0.01, 500 );
  photon_bounce_hist     = new TH1D( "PhotonBounce", "", 30, 0, 300);
  photon_all_bounce_hist = new TH1D( "AllPhotonBounce", "", 30, 0, 300);

  // Preparing the signal termination function
  signal( SIGABRT, CloseAll );
  // signal( SIGFPE,  CloseAll );
  // signal( SIGILL,  CloseAll );
  signal( SIGINT,  CloseAll );
  // signal( SIGSEGV, CloseAll );
  signal( SIGTERM, CloseAll );
}

void
LYSimAnalysis::PrepareNewRun( const G4Run* )
{
  // Reset variables relative to the run
  PhotonCount = 0;
  HitCount    = 0;
  std::cout << "Average number of generated photons: "
            << generatorAction->NPhotons() << std::endl;

  // generatorAction->RandomizePosition();
  // generatorAction->RandomizePolarization();
}

void
LYSimAnalysis::PrepareNewEvent( const G4Event* event )
{
  // Saving Event information.
  format->beam_center_x = generatorAction->GetBeamX();
  format->beam_center_y = generatorAction->GetBeamY();
  format->beam_width    = generatorAction->GetWidth();

  // All primary vertex's in the event share the same x,y values
  format->beam_x = event->GetPrimaryVertex()->GetX0();
  format->beam_y = event->GetPrimaryVertex()->GetY0();

  // Get tile parameters
  format->tile_width    = DetectorConstruction->GetTileX();

  // Getting the SiPM parameters
  format->sipm_width = DetectorConstruction->GetSiPMX();
  format->sipm_rim   = DetectorConstruction->GetSiPMRim();
  format->sipm_stand = DetectorConstruction->GetSiPMStand();

  format->abs_mult     = DetectorConstruction->GetTileAbsMult();
  format->wrap_reflect = DetectorConstruction->GetWrapReflect();

  format->dimple_radius = DetectorConstruction->GetDimpleRadius();
  format->dimple_indent = DetectorConstruction->GetDimpleIndent();

  format->nphotons = -1;
}

void
LYSimAnalysis::EndOfEvent( const G4Event* event )
{
  // Update number of photons if exists.
  format->genphotons = generatorAction->NSources();
  format->nphotons   = GetNPhotons( event );

  // Filling the three
  tree->Fill();

  std::cout << "\rFinishing Event " << tree->GetEntries()
            << " | Detected/Generated:"
            << format->nphotons << "/" << format->genphotons
            << std::flush;
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
  std::cout << "Writing to file " << filename << "("
            << tree->GetEntries() << ")" << std::endl;
  tree->Write();
  photon_energy_hist->Write();
  photon_track_hist->Write();
  photon_bounce_hist->Write();
  photon_all_track_hist->Write();
  photon_all_bounce_hist->Write();
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
    const double TrackLength = ( *hits )[i]->GetLength();
    const int bounce = (*hits)[i]->GetBounceCount();
    photon_track_hist->Fill( TrackLength / cm );
    photon_bounce_hist->Fill( bounce/3 );
  }

  return EventPhotonCount;
}
