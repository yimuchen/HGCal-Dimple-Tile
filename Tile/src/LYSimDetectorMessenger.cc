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
  analysis = Analysis::GetInstance();

  detDir = new G4UIdirectory( "/LYSim/" );
  detDir->SetGuidance( " Geometry Setup " );

  UpdateCmd = new G4UIcmdWithoutParameter( "/LYSim/Update", this );
  UpdateCmd->SetGuidance( "Update geometry" );
  UpdateCmd->SetGuidance( "This command MUST be applied before \"beamOn\" " );
  UpdateCmd->SetGuidance( "if you changed geometrical value(s)." );
  UpdateCmd->AvailableForStates( G4State_Idle );

  SetWrappingCmd = new G4UIcmdWithABool( "/LYSim/SetWrapping", this );
  SetWrappingCmd->SetGuidance( "Set Wrapping (Tyvek around scintillator) toggle true or false" );
  SetWrappingCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetRefIndexCmd = new G4UIcmdWithADouble( "/LYSim/SetRefIndex", this );
  SetRefIndexCmd->SetGuidance( "Set the refractive index PMT scint gap" );
  SetRefIndexCmd->SetParameterName( "RefIndex", false );
  SetRefIndexCmd->SetRange( "RefIndex>=1." );
  SetRefIndexCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetTileZCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetTileZ", this );
  SetTileZCmd->SetGuidance( "Set the scintillator thickness" );
  SetTileZCmd->SetParameterName( "TileZ", false );
  SetTileZCmd->SetUnitCategory( "Length" );
  SetTileZCmd->SetDefaultUnit( "mm" );
  SetTileZCmd->SetRange( "TileZ>=0." );
  SetTileZCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetTileXCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetTileX", this );
  SetTileXCmd->SetGuidance( "Set the scintillator transverse dimenstions" );
  SetTileXCmd->SetParameterName( "TileZ", false );
  SetTileXCmd->SetUnitCategory( "Length" );
  SetTileXCmd->SetDefaultUnit( "mm" );
  SetTileXCmd->SetRange( "TileZ>=0." );
  SetTileXCmd->AvailableForStates( G4State_PreInit, G4State_Idle );


  SetTileYCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetTileY", this );
  SetTileYCmd->SetGuidance( "Set the scintillator transverse dimenstions" );
  SetTileYCmd->SetParameterName( "TileY", false );
  SetTileYCmd->SetUnitCategory( "Length" );
  SetTileYCmd->SetDefaultUnit( "mm" );
  SetTileYCmd->SetRange( "TileY>=0." );
  SetTileYCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetScintPMTGapThicknessCmd = new G4UIcmdWithADoubleAndUnit( "/LYSim/SetGapThickness", this );
  SetScintPMTGapThicknessCmd->SetGuidance( "Set the thickness of the gap between the scintillator and PMT" );
  SetScintPMTGapThicknessCmd->SetParameterName( "GapThickness", false );
  SetScintPMTGapThicknessCmd->SetUnitCategory( "Length" );
  SetScintPMTGapThicknessCmd->SetDefaultUnit( "mm" );
  SetScintPMTGapThicknessCmd->SetRange( "GapThickness>=0." );
  SetScintPMTGapThicknessCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetTileAbsLengthCmd = new G4UIcmdWithADoubleAndUnit(
    "/LYSim/SetTileAbsLength", this );
  SetTileAbsLengthCmd->SetGuidance(
    "Set the light attenuation length in the tile" );
  SetTileAbsLengthCmd->SetParameterName( "TileAbsLength", false );
  SetTileAbsLengthCmd->SetUnitCategory( "Length" );
  SetTileAbsLengthCmd->SetDefaultUnit( "cm" );
  SetTileAbsLengthCmd->SetRange( "TileAbsLength>=0." );
  SetTileAbsLengthCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

  SetInducedAbsLengthCmd = new G4UIcmdWithADoubleAndUnit(
    "/LYSim/SetInducedAbsLength", this );
  SetInducedAbsLengthCmd->SetGuidance(
    "Set the induced absorption length (cm)" );
  SetInducedAbsLengthCmd->SetParameterName( "InducedMuTile", false );
  SetInducedAbsLengthCmd->SetUnitCategory( "Length" );
  SetInducedAbsLengthCmd->SetDefaultUnit( "cm" );
  SetInducedAbsLengthCmd->SetRange( "InducedMuTile>=0." );
  SetInducedAbsLengthCmd->AvailableForStates( G4State_PreInit, G4State_Idle );

}

LYSimDetectorMessenger::~LYSimDetectorMessenger()
{
  delete detDir;
  delete UpdateCmd;
  delete SetWrappingCmd;
  delete SetRefIndexCmd;
  delete SetTileZCmd;
  delete SetTileXCmd;
  delete SetTileYCmd;
  delete SetScintPMTGapThicknessCmd;
  delete SetTileAbsLengthCmd;
  delete SetInducedAbsLengthCmd;
}

void
LYSimDetectorMessenger::SetNewValue( G4UIcommand* command, G4String val )
{
  if( command == UpdateCmd ){
    Detector->UpdateGeometry();
  } else if( command == SetWrappingCmd ){
    Detector->SetWrapping( G4UIcmdWithABool::GetNewBoolValue( val ) );
  } else if( command == SetRefIndexCmd ){
    Detector->SetRefIndex( G4UIcmdWithADouble::GetNewDoubleValue( val ) );
  } else if( command == SetTileZCmd ){
    Detector->
    SetTileZ( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetTileXCmd ){
    Detector->
    SetTileX( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetTileYCmd ){
    Detector->SetTileY(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetScintPMTGapThicknessCmd ){
    Detector->SetScintPMTGapThickness(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetTileAbsLengthCmd ){
    Detector->SetTileAbsLength(
      G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val ) );
  } else if( command == SetInducedAbsLengthCmd ){
    G4double value = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue( val );
    Detector->SetInducedMuTile( 1/value );
    analysis->SetInducedMuTile( 1/value );
  }
}
