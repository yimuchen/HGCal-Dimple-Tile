#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimSteppingAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimSteppingMessenger.hh"
#else
#include "LYSimSteppingAction.hh"
#include "LYSimSteppingMessenger.hh"
#endif

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

LYSimSteppingMessenger::LYSimSteppingMessenger( LYSimSteppingAction* act ) :
  action( act )
{
  detDir = new G4UIdirectory( "/LYSim/" );
  detDir->SetGuidance( " Geometry Setup " );

  SetMaxTrackLengthCmd
    = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetMaxTrackLength", this );
  SetMaxTrackLengthCmd->SetGuidance(
    "Set maximum length of tracking computations" );
  SetMaxTrackLengthCmd->SetParameterName( "TrackLength", false );
  SetMaxTrackLengthCmd->SetUnitCategory( "Length" );
  SetMaxTrackLengthCmd->SetDefaultUnit( "mm" );
  SetMaxTrackLengthCmd->AvailableForStates( G4State_PreInit, G4State_Idle );
}

LYSimSteppingMessenger::~LYSimSteppingMessenger()
{
  delete detDir;
  delete SetMaxTrackLengthCmd;
}

void
LYSimSteppingMessenger::SetNewValue( G4UIcommand* command, G4String val )
{
  if( command == SetMaxTrackLengthCmd ){
    action->SetMaxTrackLength(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  }
}
