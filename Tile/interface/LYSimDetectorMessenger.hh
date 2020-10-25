#ifndef LYSimDetectorMessenger_h
#define LYSimDetectorMessenger_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimAnalysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#else
#include "LYSimAnalysis.hh"
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

  G4UIdirectory* detDir;

  G4UIcmdWithoutParameter* UpdateCmd;
  G4UIcmdWithADoubleAndUnit* SetTileXCmd;
  G4UIcmdWithADoubleAndUnit* SetTileYCmd;
  G4UIcmdWithADoubleAndUnit* SetTileZCmd;

  G4UIcmdWithADoubleAndUnit* SetSiPMXCmd;
  G4UIcmdWithADoubleAndUnit* SetSiPMYCmd;
  G4UIcmdWithADoubleAndUnit* SetSiPMRimCmd;
  G4UIcmdWithADoubleAndUnit* SetSiPMStandCmd;

  G4UIcmdWithADoubleAndUnit* SetPCBRadiusCmd;
  G4UIcmdWithADouble*        SetPCBReflectCmd;

  G4UIcmdWithADouble* SetTileAbsMultCmd;
  G4UIcmdWithADouble* SetWrapReflectCmd;
  G4UIcmdWithADouble* SetTileAlphaCmd;
  G4UIcmdWithADouble* SetDimpleAlphaCmd;

  // Dimple Geometry commands
  G4UIcmdWithADoubleAndUnit* SetDimpleIndentCmd;
  G4UIcmdWithADoubleAndUnit* SetDimpleRadiusCmd;
  G4UIcmdWithAString*        SetDimpleTypeCmd;
};

#endif
