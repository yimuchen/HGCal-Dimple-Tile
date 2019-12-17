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
#include "G4TransportationManager.hh"
#include "G4UnitsTable.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

#include <iostream>
#include <signal.h>

using namespace std;
using namespace CLHEP;

// Static Helper fuction
static void
CloseAll( int signal )
{
  LYSimAnalysis::GetInstance()->EndOfExperiment();
  std::exit( signal );
}

static bool IsSiPMTrajectory( G4Navigator*, const G4ThreeVector& );

// LYSimAnalysis Programs
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
  file      = TFile::Open( filename.c_str(), "RECREATE" );
  tree      = new TTree( "LYSim", "LYSim" );
  runtree   = new TTree( "LYSimRun", "LYSimRun" );
  format    = new LYSimFormat();
  runformat = new LYSimRunFormat();
  format->AddToTree( tree );
  runformat->AddToTree( runtree );

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
  runformat->tile_x     = DetectorConstruction->GetTileX();
  runformat->tile_y     = DetectorConstruction->GetTileY();
  runformat->tile_z     = DetectorConstruction->GetTileZ();
  runformat->sipm_width = DetectorConstruction->GetSiPMX();
  runformat->sipm_rim   = DetectorConstruction->GetSiPMRim();
  runformat->sipm_stand = DetectorConstruction->GetSiPMStand();
  runformat->dimple_rad = DetectorConstruction->GetDimpleRadius();
  runformat->dimple_ind = DetectorConstruction->GetDimpleIndent();
  runformat->abs_mult   = DetectorConstruction->GetTileAbsMult();
  runformat->wrap_ref   = DetectorConstruction->GetWrapReflect();
  runformat->pcb_rad    = DetectorConstruction->GetPCBRadius();
  runformat->pcb_ref    = DetectorConstruction->GetPCBReflect();
  runformat->beam_x     = generatorAction->GetBeamX();
  runformat->beam_y     = generatorAction->GetBeamY();
  runformat->beam_w     = generatorAction->GetWidth();

  runformat->start_event = tree->GetEntries();
  runformat->end_event   = tree->GetEntries();
}

void
LYSimAnalysis::PrepareNewEvent( const G4Event* event )
{
  // All primary vertex's in the event share the same x,y values
  format->beam_x = event->GetPrimaryVertex()->GetX0();
  format->beam_y = event->GetPrimaryVertex()->GetY0();
}

void
LYSimAnalysis::EndOfEvent( const G4Event* event )
{
  format->genphotons = generatorAction->NSources();
  format->nphotons   = GetNPhotons( event );

  auto trajectory_list   = event->GetTrajectoryContainer();
  G4Navigator* navigator =
    G4TransportationManager::GetTransportationManager()
    ->GetNavigator( "World" );

  assert( format->genphotons == trajectory_list->size() );

  unsigned nhits = format->nphotons;

  for( size_t i = 0; i < trajectory_list->size(); ++i ){
    auto trajectory = ( *trajectory_list )[i];

    unsigned wrapbounce = 0;
    unsigned pcbbounce  = 0;
    double tracklength  = 0;
    bool isdetected     = false;

    for( int j = 0; j < trajectory->GetPointEntries(); ++j ){
      const auto pos_end   = trajectory->GetPoint( j )->GetPosition();
      const auto pos_start = j == 0 ? pos_end :
                             trajectory->GetPoint( j-1 )->GetPosition();

      auto volume = navigator->LocateGlobalPointAndSetup( pos_end );
      if( volume->GetName() == "Wrap" ){
        ++wrapbounce;
      } else if( volume->GetName() == "PCB" ){
        ++pcbbounce;
      }

      tracklength += ( pos_end - pos_start ).mag();
    }

    const auto endpoint
      = trajectory->GetPoint( trajectory->GetPointEntries()-1 )->GetPosition();

    if( IsSiPMTrajectory( navigator, endpoint ) && nhits ){
      --nhits;
      isdetected = true;
    }

    format->NumWrapReflection[i] = wrapbounce;
    format->NumPCBReflection[i]  = pcbbounce;
    format->OpticalLength[i]     = tracklength / ( 0.1*mm );
    format->IsDetected[i]        = isdetected;
    format->EndX[i]              = endpoint.x() / ( 1*um );
    format->EndY[i]              = endpoint.y() / ( 1*um );
  }

  // Filling the tree
  tree->Fill();
}


void
LYSimAnalysis::EndOfRun( const G4Run* )
{
  runformat->end_event = tree->GetEntries() ;
  runtree->Fill();
}

void
LYSimAnalysis::EndOfExperiment()
{
  runtree->Write();
  tree->Write();
  file->Close();
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

  for( int i = 0; i < hits->entries(); ++i ){
    assert( ( *hits )[i]->GetPhotonCount() == 1 );
    ++EventPhotonCount;
  }

  return EventPhotonCount;
}

// Implementation of helper functions
static bool IsSiPMTrajectory( G4Navigator* nav, const G4ThreeVector& endpoint )
{
  const G4ThreeVector zvec( 0, 0, 1 );
  const auto endvol
    = nav->LocateGlobalPointAndSetup( endpoint, &zvec, false, false );
  // Expecting SiPM hits to be pointing in the z direction.
  return endvol->GetName() == "SiPM";
}
