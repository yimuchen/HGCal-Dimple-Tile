#ifndef LYSimPMTSD_h
#define LYSimPMTSD_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimPMTHit.hh"
#else
#include "LYSimPMTHit.hh"
#endif

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class LYSimPMTSD : public G4VSensitiveDetector
{
public:

  LYSimPMTSD( G4String name );
  ~LYSimPMTSD();


  G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
  // A version of processHits that keeps aStep constant
  G4bool ProcessHits_constStep( const G4Step*       aStep,
                                G4TouchableHistory* ROhist );

  void Initialize( G4HCofThisEvent* HCE );
  void EndOfEvent( G4HCofThisEvent* HCE );

  void clear();
  void DrawAll();
  void PrintAll();

private:

  LYSimPMTHitsCollection* fPMTHitsCollection;

};

#endif
