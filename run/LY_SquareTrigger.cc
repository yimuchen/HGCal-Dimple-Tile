#include "Analysis.hh"

// User initialization
#include "LYSimDetectorConstruction.hh"
#include "LYSimPhysicsList.hh"

// User action
#include "LYSimEventAction.hh"
#include "LYSimPrimaryGeneratorAction.hh"
#include "LYSimRunAction.hh"
#include "LYSimSteppingAction.hh"
#include "LYSimTrackingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

template<typename T>
T string_to( const std::string& s )
{
  T ans;
  std::stringstream ss( s );
  ss >> ans;
  return ans;
}


int
main( int argc, char** argv )
{
  std::srand( time( NULL ) );
  const double x_center      = string_to<double>( argv[1] );
  const double y_center      = string_to<double>( argv[2] );
  const double width         = string_to<double>( argv[3] );
  const unsigned N           = string_to<unsigned>( argv[4] );
  const std::string filename = argv[5];


  G4RunManager* runManager            = new G4RunManager;
  LYSimDetectorConstruction* detector = new LYSimDetectorConstruction();
  runManager->SetUserInitialization( detector );
  LYSimPhysicsList* physlist = new LYSimPhysicsList();
  runManager->SetUserInitialization( physlist );

  // Construct Analysis class
  Analysis::GetInstance()->SetDetector( detector );
  LYSimPrimaryGeneratorAction* genaction
    = new LYSimPrimaryGeneratorAction( detector );
  Analysis::GetInstance()->SetOutputFile( filename + ".txt" );
  Analysis::GetInstance()->SetROOTFile( filename + ".root" );
  Analysis::GetInstance()->SetGeneratorAction( genaction );

  // Set user action classes
  runManager->SetUserAction( genaction );
  runManager->SetUserAction( new LYSimRunAction( detector ) );
  runManager->SetUserAction( new LYSimEventAction() );
  runManager->SetUserAction( new LYSimTrackingAction() );
  runManager->SetUserAction( new LYSimSteppingAction() );

  // Initialize G4 kernel
  runManager->Initialize();
  G4VisManager* visManager = new G4VisExecutive( "Quiet" );
  visManager->Initialize();
  G4UImanager* UIManager = G4UImanager::GetUIpointer();

  // Initializing the source only after the detector has been constructed
  genaction->InitSource();

  char cmd[1024];
  UIManager->ApplyCommand( "/control/verbose 1" );
  UIManager->ApplyCommand( "/control/saveHistory" );
  UIManager->ApplyCommand( "/run/verbose 0" );
  UIManager->ApplyCommand( "/process/setVerbose 0" );
  UIManager->ApplyCommand( "/tracking/verbose 0" );

  srand( time( NULL ) );

  sprintf( cmd, "/random/setSeeds %d %d", rand(), rand() );
  UIManager->ApplyCommand( cmd );

  for( unsigned i = 0; i < N; ++i ){
    genaction->MoveSource( x_center,y_center,width );
    UIManager->ApplyCommand( "/run/beamOn 1" );
  }

  // Job termination Free the store: user actions, physics_list and
  // detector_description are owned and deleted by the run manager, so they
  // should not be deleted in the main() program !
  delete visManager;
  delete runManager;

  return 0;
}

