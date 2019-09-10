#include "HGCalTileSim/Tile/interface/Analysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPhysicsList.hh"
#include "HGCalTileSim/Tile/interface/LYSimEventAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimRunAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimSteppingAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrackingAction.hh"

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
double
stof( const std::string& s )
{
  float ans;
  std::stringstream ss( s );
  ss >> ans;
  return ans;
}

int
stoi( const std::string& s )
{
  int ans;
  std::stringstream ss( s );
  ss >> ans;
  return ans;
}

int
main( int argc, char** argv )
{
  std::srand( time( NULL ) );
  const double x_center      = stof( argv[1] );
  const double y_center      = stof( argv[2] );
  const double width         = stof( argv[3] );
  const unsigned N           = stoi( argv[4] );
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
  // Primary generator action
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

  char cmd[1024];
  UIManager->ApplyCommand( "/control/verbose 1" );
  UIManager->ApplyCommand( "/control/saveHistory" );
  UIManager->ApplyCommand( "/run/verbose 0" );
  UIManager->ApplyCommand( "/process/setVerbose 0" );
  UIManager->ApplyCommand( "/tracking/verbose 0" );

  srand( time( NULL ) );

  sprintf( cmd, "/random/setSeeds %d %d", rand(), rand() );
  UIManager->ApplyCommand( cmd );

  genaction->SetBeamX( x_center );
  genaction->SetBeamY( y_center );
  genaction->SetWidth( width );
  runManager->BeamOn( N );

  // Job termination Free the store: user actions, physics_list and
  // detector_description are owned and deleted by the run manager, so they
  // should not be deleted in the main() program !
  delete visManager;
  delete runManager;

  return 0;
}
