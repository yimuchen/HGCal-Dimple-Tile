#ifndef LYSimPrimaryGeneratorMessenger_h
#define LYSimPrimaryGeneratorMessenger_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimPrimaryGeneratorAction.hh"
#else
#include "LYSimPrimaryGeneratorAction.hh"
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

class LYSimPrimaryGeneratorMessenger : public G4UImessenger
{
public:

  LYSimPrimaryGeneratorMessenger( LYSimPrimaryGeneratorAction* );
  ~LYSimPrimaryGeneratorMessenger();

  void SetNewValue( G4UIcommand*, G4String );

private:

  LYSimPrimaryGeneratorAction* genaction;
  G4UIdirectory* detDir;

  G4UIcmdWithADoubleAndUnit* SetBeamXCmd;
  G4UIcmdWithADoubleAndUnit* SetBeamYCmd;
  G4UIcmdWithADoubleAndUnit* SetWidthCmd;
  G4UIcmdWithADoubleAndUnit* SetOpenAngleCmd;
  G4UIcmdWithADouble*        SetPhotonMultCmd;
};

#endif
