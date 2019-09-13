#ifndef LYSimSteppingAction_h
#define LYSimSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class LYSimSteppingMessenger;

class LYSimSteppingAction : public G4UserSteppingAction
{
public:
  LYSimSteppingAction();
  virtual ~LYSimSteppingAction();

  // method from the base class
  virtual void UserSteppingAction( const G4Step* );

  inline double
  GetMaxTrackLength() const { return maxtracklength; }
  inline void
  SetMaxTrackLength( const double x ) { maxtracklength = x ; }

private:
  double maxtracklength;
  LYSimSteppingMessenger* messenger;
};

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
