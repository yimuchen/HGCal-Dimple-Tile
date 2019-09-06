#ifndef LYSimPhysicsListMessenger_h
#define LYSimPhysicsListMessenger_h

#include "globals.hh"
#include "G4UImessenger.hh"

class LYSimPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class LYSimPhysicsListMessenger : public G4UImessenger
{
public:

  LYSimPhysicsListMessenger( LYSimPhysicsList* );
  virtual ~LYSimPhysicsListMessenger();

  void SetNewValue( G4UIcommand*, G4String );

private:

  LYSimPhysicsList* pPhysicsList;

  G4UIdirectory* physDir;
  G4UIcmdWithAString* pListCmd;
  G4UIcmdWithADoubleAndUnit* gammaCutCmd;
  G4UIcmdWithADoubleAndUnit* electCutCmd;
  G4UIcmdWithADoubleAndUnit* protoCutCmd;
  G4UIcmdWithADoubleAndUnit* allCutCmd;
};

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
