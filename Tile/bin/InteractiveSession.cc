#include "HGCalTileSim/Tile/interface/Analysis.hh"

// User initialization
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPhysicsList.hh"

// User action
#include "HGCalTileSim/Tile/interface/LYSimEventAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimRunAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimSteppingAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimTrackingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

#include "Randomize.hh"

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
template<class T>
std::string
toString( const T & value )
{
  std::ostringstream os;
  os << value;
  return os.str();
}


int
main( int argc, char** argv )
{
  G4cout << "[LYSIM]\n"
         << "If first argument exists, it is taken as the execution macro\n"
         << "If second argument exists, it is taken as the output file name "
         << "(defaults to Analysis_<RandomInt> if not specifed)\n"
         << "All other arguments will be ignored.\n"
         << "\nIf no arguments are provided, start interactive session.\n"
         << G4endl;

  struct timespec ts;
  clock_gettime( CLOCK_MONOTONIC, &ts );
  srand( (time_t)ts.tv_nsec );
  const unsigned rint    = rand() % 100000;
  const std::string rstr = toString( rint );

  const G4String macfile    = argc > 1 ? argv[1] : "data/macro/init_vis.mac";
  const G4String outputfile = argc > 2 ? argv[2] : "Analysis" + rstr;

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  // Pointer to GeneratorAction class for access to source properties
  // Set mandatory initialization classes
  LYSimDetectorConstruction* detector = new LYSimDetectorConstruction();

  runManager->SetUserInitialization( detector );
  LYSimPhysicsList* physlist = new LYSimPhysicsList();
  runManager->SetUserInitialization( physlist );

  // Construct Analysis class
  Analysis::GetInstance()->SetDetector( detector );
  LYSimPrimaryGeneratorAction* genaction
    = new LYSimPrimaryGeneratorAction( detector );
  Analysis::GetInstance()->SetOutputFile( outputfile + ".txt" );
  Analysis::GetInstance()->SetROOTFile( outputfile + ".root" );
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

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();


  if( macfile != "data/macro/init_vis.mac" ){
    G4cout << ">>> Executing file [" << macfile << "]..." << G4endl;
    const G4String command = "/control/execute " + macfile;
    UImanager->ApplyCommand( command );
  } else {
    G4cout << ">>> Starting interactive session..." << G4endl;
    G4UIExecutive* ui = new G4UIExecutive( argc, argv );
    UImanager->ApplyCommand( "/control/execute mac/init_vis.mac" );
    ui->SessionStart();
    delete ui;
  }

  // Job termination Free the store: user actions, physics_list and
  // detector_description are owned and deleted by the run manager, so they
  // should not be deleted in the main() program !
  delete visManager;
  delete runManager;

  return 0;
}

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
