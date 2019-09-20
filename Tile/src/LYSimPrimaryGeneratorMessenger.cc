#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorMessenger.hh"
#else
#include "LYSimPrimaryGeneratorMessenger.hh"
#endif

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

LYSimPrimaryGeneratorMessenger::LYSimPrimaryGeneratorMessenger( LYSimPrimaryGeneratorAction* act )
  : genaction( act )
{
  detDir = new G4UIdirectory( "/LYSim/" );
  detDir->SetGuidance( " Geometry Setup " );

  SetBeamXCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetBeamX", this );
  SetBeamXCmd->SetGuidance( "Set central X position of the beam" );
  SetBeamXCmd->SetParameterName( "BeamX", false );
  SetBeamXCmd->SetUnitCategory( "Length" );
  SetBeamXCmd->SetDefaultUnit( "mm" );
  SetBeamXCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetBeamYCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetBeamY", this );
  SetBeamYCmd->SetGuidance( "Set central Y position of the beam" );
  SetBeamYCmd->SetParameterName( "BeamY", false );
  SetBeamYCmd->SetUnitCategory( "Length" );
  SetBeamYCmd->SetDefaultUnit( "mm" );
  SetBeamYCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetWidthCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetBeamWidth", this );
  SetWidthCmd->SetGuidance( "Set Width of Beam" );
  SetWidthCmd->SetParameterName( "BeamWidth", false );
  SetWidthCmd->SetUnitCategory( "Length" );
  SetWidthCmd->SetDefaultUnit( "mm" );
  SetWidthCmd->SetRange( "BeamWidth>=0" );
  SetWidthCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetOpenAngleCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetOpenAngle", this );
  SetOpenAngleCmd->SetGuidance( "Set Width of Beam" );
  SetOpenAngleCmd->SetParameterName( "OpenAngle", false );
  SetOpenAngleCmd->SetUnitCategory( "Angle" );
  SetOpenAngleCmd->SetDefaultUnit( "deg" );
  SetOpenAngleCmd->SetRange( "OpenAngle>=0" );
  SetOpenAngleCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetPhotonMultCmd = new G4UIcmdWithADouble( "/LYSim/SetPhotonMult", this );
  SetPhotonMultCmd->SetGuidance(
    "Set Photon count multipliter (for visualization)" );
  SetPhotonMultCmd->SetParameterName( "PhotonMult", false );
  SetPhotonMultCmd->AvailableForStates( G4State_PreInit, G4State_Idle );
}

LYSimPrimaryGeneratorMessenger::~LYSimPrimaryGeneratorMessenger()
{
  delete detDir;
  delete SetBeamXCmd;
  delete SetBeamYCmd;
  delete SetWidthCmd;
  delete SetOpenAngleCmd;
  delete SetPhotonMultCmd;
}

void
LYSimPrimaryGeneratorMessenger::SetNewValue( G4UIcommand* command, G4String val )
{
  if( command == SetBeamXCmd ){
    genaction->SetBeamX( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetBeamYCmd ){
    genaction->SetBeamY( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetWidthCmd ){
    genaction->SetWidth( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetPhotonMultCmd ){
    genaction->SetPhotonMult( G4UIcmdWithADouble::GetNewDoubleValue( val ) );
  } else if( command == SetOpenAngleCmd ){
    genaction->SetOpenAngle(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  }
}
