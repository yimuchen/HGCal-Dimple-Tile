#ifndef LYSimDetectorMessenger_h
#define LYSimDetectorMessenger_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/Analysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#else
#include "Analysis.hh"
#include "LYSimDetectorConstruction.hh"
#endif

#include "globals.hh"
#include "G4UImessenger.hh"


class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithoutParameter;

class LYSimDetectorMessenger : public G4UImessenger
{
public:

  LYSimDetectorMessenger( LYSimDetectorConstruction* );
  ~LYSimDetectorMessenger();

  void SetNewValue( G4UIcommand*, G4String );

private:

  LYSimDetectorConstruction* Detector;
  Analysis* analysis;

  G4UIdirectory* detDir;

  G4UIcmdWithoutParameter* UpdateCmd;
  G4UIcmdWithABool* SetWrappingCmd;
  G4UIcmdWithADouble* SetRefIndexCmd;
  G4UIcmdWithADoubleAndUnit* SetTileZCmd;
  G4UIcmdWithADoubleAndUnit* SetTileXCmd;
  G4UIcmdWithADoubleAndUnit* SetTileYCmd;
  G4UIcmdWithADoubleAndUnit* SetScintPMTGapThicknessCmd;
  G4UIcmdWithADoubleAndUnit* SetTileAbsLengthCmd;
  G4UIcmdWithADoubleAndUnit* SetInducedAbsLengthCmd;

  // Dimple Geometry commands
  G4UIcmdWithADoubleAndUnit* SetDimpleIndentCmd;
  G4UIcmdWithADoubleAndUnit* SetDimpleRadiusCmd;
  G4UIcmdWithAString*        SetDimpleTypeCmd;
};

#endif
