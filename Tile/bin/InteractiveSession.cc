#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/Analysis.hh"

#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPhysicsList.hh"

#include "HGCalTileSim/Tile/interface/LYSimEventAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimRunAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimSteppingAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrackingAction.hh"
#else
#include "Analysis.hh"

#include "LYSimDetectorConstruction.hh"
#include "LYSimPhysicsList.hh"

#include "LYSimEventAction.hh"
#include "LYSimPrimaryGeneratorAction.hh"
#include "LYSimRunAction.hh"
#include "LYSimSteppingAction.hh"
#include "LYSimTrackingAction.hh"
#endif

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int
main( int argc, char** argv )
{
  G4cout << "[LYSIM]\n"
         << "Interactive session for geometry debugging"
         << G4endl;

  G4RunManager* runManager            = new G4RunManager;
  LYSimDetectorConstruction* detector = new LYSimDetectorConstruction();

  runManager->SetUserInitialization( detector );
  LYSimPhysicsList* physlist = new LYSimPhysicsList();
  runManager->SetUserInitialization( physlist );

  // Construct Analysis class
  Analysis::GetInstance()->SetDetector( detector );
  LYSimPrimaryGeneratorAction* genaction
    = new LYSimPrimaryGeneratorAction( detector );
  Analysis::GetInstance()->SetOutputFile( "Interactive_Session.txt" );
  Analysis::GetInstance()->SetROOTFile( "Interactive_Session.root" );
  Analysis::GetInstance()->SetGeneratorAction( genaction );

  // Set user action classes
  runManager->SetUserAction( genaction );
  runManager->SetUserAction( new LYSimRunAction( detector ) );
  runManager->SetUserAction( new LYSimEventAction() );
  runManager->SetUserAction( new LYSimTrackingAction() );
  runManager->SetUserAction( new LYSimSteppingAction() );

  runManager->Initialize();
  G4VisManager* visManager = new G4VisExecutive( "Quiet" );
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Starting applying standard commands.
  UImanager->ApplyCommand( "/control/execute mac/init_vis.mac" );
  UImanager->ApplyCommand( "/control/verbose 0" );
  UImanager->ApplyCommand( "/run/verbose 0" );
  UImanager->ApplyCommand( "/process/setVerbose 0" );

#ifndef CMSSW_GIT_HASH
  UImanager->ApplyCommand( "/vis/open OGL 800x800-0+0" );

  UImanager->ApplyCommand( "/vis/drawVolume" );
  UImanager->ApplyCommand( "/vis/viewer/set/viewpointVector -1 0 0" );
  UImanager->ApplyCommand( "/vis/viewer/set/lightsVector -1 0 0" );
  UImanager->ApplyCommand( "/vis/viewer/set/style wireframe" );
  UImanager->ApplyCommand( "/vis/viewer/set/auxiliaryEdge true" );
  UImanager->ApplyCommand( "/vis/viewer/set/lineSegmentsPerCircle 100" );
  UImanager->ApplyCommand( "/vis/scene/add/trajectories smooth" );
  UImanager->ApplyCommand( "/vis/modeling/trajectories/create/drawByCharge" );
  UImanager->ApplyCommand(
    "/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true" );
  UImanager->ApplyCommand(
    "/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2" );
  UImanager->ApplyCommand(
    "#/vis/filtering/trajectories/create/particleFilter" );
  UImanager->ApplyCommand(
    "#/vis/filtering/trajectories/particleFilter-0/add gamma" );
  UImanager->ApplyCommand(
    "#/vis/filtering/trajectories/particleFilter-0/invert true" );
  UImanager->ApplyCommand( "/vis/scene/endOfEventAction accumulate" );
  UImanager->ApplyCommand( "/vis/scene/add/scale" );
  UImanager->ApplyCommand( "/vis/scene/add/axes" );
  UImanager->ApplyCommand( "/vis/scene/add/eventID" );
  UImanager->ApplyCommand( "/vis/scene/add/date" );
  UImanager->ApplyCommand( "/vis/set/colour red" );
  UImanager->ApplyCommand( "/vis/set/lineWidth 2" );
  UImanager->ApplyCommand( "/vis/scene/add/frame" );
  UImanager->ApplyCommand( "/vis/set/colour" );
  UImanager->ApplyCommand( "/vis/set/lineWidth" );
  UImanager->ApplyCommand( "/vis/geometry/set/visibility World 0 false" );
  UImanager->ApplyCommand( "/vis/viewer/set/style surface" );
  UImanager->ApplyCommand( "/vis/viewer/set/hiddenMarker true" );
  UImanager->ApplyCommand( "/vis/viewer/set/viewpointThetaPhi 90 180" );
  UImanager->ApplyCommand( "#/vis/viewer/zoom 2" );
  UImanager->ApplyCommand( "/vis/viewer/set/autoRefresh true" );
  UImanager->ApplyCommand( "/vis/verbose warnings" );
#endif


  // Starting interactive session .
  G4UIExecutive* ui = new G4UIExecutive( argc, argv );
  ui->SessionStart();

  // Job termination Free the store: user actions, physics_list and
  // detector_description are owned and deleted by the run manager, so they
  // should not be deleted in the main() program !
  delete ui;
  delete visManager;
  delete runManager;

  return 0;
}
