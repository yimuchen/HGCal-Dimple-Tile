#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimDetectorMessenger.hh"
#else
#include "LYSimDetectorMessenger.hh"
#endif

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

LYSimDetectorMessenger::LYSimDetectorMessenger( LYSimDetectorConstruction* Det )
  : Detector( Det )
{
  detDir = new G4UIdirectory( "/LYSim/" );
  detDir->SetGuidance( " Geometry Setup " );

  UpdateCmd = new G4UIcmdWithoutParameter( "/LYSim/Update", this );
  UpdateCmd->SetGuidance( "Update geometry" );
  UpdateCmd->SetGuidance( "This command MUST be applied before \"beamOn\" " );
  UpdateCmd->SetGuidance( "if you changed geometrical value(s)." );
  UpdateCmd->AvailableForStates( G4State_Idle );

  SetTileXCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetTileX", this );
  SetTileXCmd->SetGuidance( "Set the scintillator transverse dimenstions" );
  SetTileXCmd->SetParameterName( "TileX", false );
  SetTileXCmd->SetUnitCategory( "Length" );
  SetTileXCmd->SetDefaultUnit( "mm" );
  SetTileXCmd->SetRange( "TileX>=0." );
  SetTileXCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetTileYCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetTileY", this );
  SetTileYCmd->SetGuidance( "Set the scintillator transverse dimenstions" );
  SetTileYCmd->SetParameterName( "TileY", false );
  SetTileYCmd->SetUnitCategory( "Length" );
  SetTileYCmd->SetDefaultUnit( "mm" );
  SetTileYCmd->SetRange( "TileY>=0." );
  SetTileYCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetTileZCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetTileZ", this );
  SetTileZCmd->SetGuidance( "Set the scintillator thickness" );
  SetTileZCmd->SetParameterName( "TileZ", false );
  SetTileZCmd->SetUnitCategory( "Length" );
  SetTileZCmd->SetDefaultUnit( "mm" );
  SetTileZCmd->SetRange( "TileZ>=0." );
  SetTileZCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetTileAbsMultCmd = new G4UIcmdWithADouble(
    "/LYSim/SetTileAbsMult", this );
  SetTileAbsMultCmd->SetGuidance(
    "Set the light attenuation length multiplier" );
  SetTileAbsMultCmd->SetParameterName( "TileAbsMult", false );
  SetTileAbsMultCmd->SetRange( "TileAbsMult>=0." );
  SetTileAbsMultCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  // Dimple Geometry commands
  SetDimpleIndentCmd = new G4UIcmdWithADoubleAndUnit(
    "/LYSim/SetDimpleIndent", this );
  SetDimpleIndentCmd->SetGuidance(
    "Setting the reccess depth of the dimple [mm]" );
  SetDimpleIndentCmd->SetParameterName( "DimpleIndent", false );
  SetDimpleIndentCmd->SetUnitCategory( "Length" );
  SetDimpleIndentCmd->SetDefaultUnit( "mm" );
  SetDimpleIndentCmd->SetRange( "DimpleIndent>=0." );
  SetDimpleIndentCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetDimpleRadiusCmd = new G4UIcmdWithADoubleAndUnit(
    "/LYSim/SetDimpleRadius", this );
  SetDimpleRadiusCmd->SetGuidance(
    "Setting the reccess depth of the dimple [mm]" );
  SetDimpleRadiusCmd->SetParameterName( "DimpleIndent", false );
  SetDimpleRadiusCmd->SetUnitCategory( "Length" );
  SetDimpleRadiusCmd->SetDefaultUnit( "mm" );
  SetDimpleRadiusCmd->SetRange( "DimpleIndent>=0." );
  SetDimpleRadiusCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetDimpleTypeCmd = new G4UIcmdWithAString("/LYSim/SetDimpleType", this );
  SetDimpleTypeCmd->SetGuidance( "Setting the type of dimple to use");
  SetDimpleTypeCmd->SetCandidates( "Spherical Parabolic Pyramid" );
  SetDimpleTypeCmd->AvailableForStates( G4State_PreInit, G4State_Idle );
}

LYSimDetectorMessenger::~LYSimDetectorMessenger()
{
  delete detDir;
  delete UpdateCmd;
  delete SetTileZCmd;
  delete SetTileXCmd;
  delete SetTileYCmd;
  delete SetTileAbsMultCmd;

  delete SetDimpleIndentCmd;
  delete SetDimpleRadiusCmd;
  delete SetDimpleTypeCmd;
}

void
LYSimDetectorMessenger::SetNewValue( G4UIcommand* command, G4String val )
{
  if( command == UpdateCmd ){
    Detector->UpdateGeometry();
  } else if( command == SetTileZCmd ){
    Detector->
    SetTileZ( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetTileXCmd ){
    Detector->
    SetTileX( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetTileYCmd ){
    Detector->SetTileY(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetTileAbsMultCmd ){
    Detector->UpdateAbs( G4UIcmdWithADouble::GetNewDoubleValue( val ) );
  } else if( command == SetDimpleIndentCmd ){
    Detector->SetDimpleIndent(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetDimpleRadiusCmd ){
    Detector->SetDimpleRadius(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetDimpleTypeCmd ){
    if( val == "Spherical" ){
      Detector->SetDimpleType( LYSimDetectorConstruction::SPHERICAL );
    } else if (val == "Parabolic" ){
      Detector->SetDimpleType( LYSimDetectorConstruction::PARABOLIC );
    } else if ( val == "Pyramid" ){
      Detector->SetDimpleType( LYSimDetectorConstruction::PARABOLIC );
    }
  }
}
