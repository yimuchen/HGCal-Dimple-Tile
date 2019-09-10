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


std::string SetRandomCmd();
double      CalcThickness( const LYSimDetectorConstruction*,
                           const double x, const double y );
double CalcNumPhotons( const double thickness );

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
  UIManager->ApplyCommand( "/gps/source/intensity 1" );
  UIManager->ApplyCommand( "/gps/particle opticalphoton" );
  UIManager->ApplyCommand( "/gps/pos/type Volume" );
  UIManager->ApplyCommand( "/gps/pos/shape Cylinder" );
  UIManager->ApplyCommand( "/gps/pos/radius 0.001 mm" );
  UIManager->ApplyCommand( "/gps/ang/type iso" );
  UIManager->ApplyCommand( "/gps/ene/type Arb" );
  UIManager->ApplyCommand( "/gps/hist/type arb" );
  UIManager->ApplyCommand( "/gps/hist/file /data/users/yichen/HGCalTileSim/Tile/data/PhotonSpectrum.dat" );
  UIManager->ApplyCommand( "/gps/hist/inter Lin" );
  UIManager->ApplyCommand( "/LYSim/Update" );

  srand( time( NULL ) );

  sprintf( cmd, "/random/setSeeds %d %d", rand(), rand() );
  UIManager->ApplyCommand( cmd );

  for( unsigned i = 0; i < N; ++i ){
    const float x = ( 2*double(rand()%1000000)/double(1000000) -1 )* width + x_center;
    const float y = ( 2*double(rand()%1000000)/double(1000000) - 1 ) * width + y_center;
    const float t = CalcThickness( detector, x, y );
    const int np  = CalcNumPhotons( t );



    sprintf( cmd, "/gps/pos/halfz  %.5f mm", t/2 );
    UIManager->ApplyCommand( cmd );

    sprintf( cmd, "/gps/pos/center %.5f %.5f %.5f mm", x, y, t/2 );
    UIManager->ApplyCommand( cmd );

    sprintf( cmd, "/gps/number %d", 1000 );
    UIManager->ApplyCommand( cmd );

    UIManager->ApplyCommand( "/run/beamOn 1" );

  }

  // Job termination Free the store: user actions, physics_list and
  // detector_description are owned and deleted by the run manager, so they
  // should not be deleted in the main() program !
  delete visManager;
  delete runManager;

  return 0;
}

std::string
SetRandomCmd()
{
  char cmd[1024];
  const int s1 = rand();
  const int s2 = rand();
  sprintf( cmd, "/random/setSeeds %d %d", s1, s2 );
  return cmd;
}

double
CalcThickness( const LYSimDetectorConstruction* detector,
               const double x, const double y )
{
  const double dimple_r  = detector->GetDimpleRadius();
  const double dimple_i  = detector->GetDimpleIndent();
  const double beam_r    = sqrt( x*x+y*y );
  const double big_r     = detector->GetDimpleSizeRadius();
  const double thickness = detector->GetTileThickness();
  const int type         = detector->GetDimpleType();

  if( beam_r <= dimple_r ){
    if( type == 0 ){
      return thickness - ( sqrt( big_r*big_r - beam_r*beam_r ) -
                           ( big_r - dimple_i ) );
    } else if( type == 2 ){//  # Parabolic
      return thickness - abs( 1.35 * beam_r*beam_r - dimple_i );
    } else if( type == 1 ){
      if( fabs( y ) <= fabs( x ) ){
        return thickness - ( dimple_i - ( dimple_i / dimple_r ) * fabs( x ) );
      } else {
        return thickness - ( dimple_i - ( dimple_i / dimple_r ) * fabs( y ) );
      }
    }  else {
      return thickness;
    }
  } else {
    return thickness;
  }
}

double
CalcNumPhotons( const double thickness )
{
  return int(std::floor( (double)6677 * thickness/3.0 ) );
}
