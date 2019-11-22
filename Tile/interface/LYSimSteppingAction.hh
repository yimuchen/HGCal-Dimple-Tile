#ifndef LYSimSteppingAction_h
#define LYSimSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class LYSimAnalysis;
class LYSimSteppingMessenger;

class LYSimSteppingAction : public G4UserSteppingAction
{
public:
  LYSimSteppingAction(LYSimAnalysis* analysis);
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
  LYSimAnalysis* analysis;
};

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
