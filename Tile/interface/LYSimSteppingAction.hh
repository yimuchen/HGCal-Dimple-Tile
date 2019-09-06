#ifndef LYSimSteppingAction_h
#define LYSimSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class G4LogicalVolume;

/// Stepping action class
/// Checks if track length is too long. Kill if track length greater than 1000mm.

class LYSimSteppingAction : public G4UserSteppingAction
{
public:
  LYSimSteppingAction();
  virtual ~LYSimSteppingAction();

  // static access method
  static LYSimSteppingAction* Instance();

  // method from the base class
  virtual void UserSteppingAction( const G4Step* );

  // reset count
  void Reset();

  // set methods
  void
  SetVolume( G4LogicalVolume* volume ){ fVolume = volume; }

  // get methods
  G4LogicalVolume*
  GetVolume() const { return fVolume; }
  G4int
  GetHitCount() const { return PhotonHitCount; }

private:
  static LYSimSteppingAction* fgInstance;

  G4LogicalVolume* fVolume;
  G4int PhotonHitCount;
};

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
