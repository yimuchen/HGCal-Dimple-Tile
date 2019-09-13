#ifndef LYSimSteppingMessenger_h
#define LYSimSteppingMessenger_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimSteppingAction.hh"
#else
#include "LYSimSteppingAction.hh"
#endif

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

class LYSimSteppingMessenger : public G4UImessenger
{
public:
  LYSimSteppingMessenger( LYSimSteppingAction* );
  ~LYSimSteppingMessenger();

  void SetNewValue( G4UIcommand*, G4String );

private:
  LYSimSteppingAction* action;
  G4UIdirectory* detDir;

  G4UIcmdWithADoubleAndUnit* SetMaxTrackLengthCmd;
};

#endif

