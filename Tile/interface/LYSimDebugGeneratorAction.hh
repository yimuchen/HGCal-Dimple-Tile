#ifndef LYSimDebugGeneratorAction_h
#define LYSimDebugGeneratorAction_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#else
#include "LYSimDetectorConstruction.hh"
#endif

#include "globals.hh"
#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"

class G4Event;
class LYSimDebugGeneratorMessenger;

class LYSimDebugGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  LYSimDebugGeneratorAction();
  ~LYSimDebugGeneratorAction();

  void GeneratePrimaries( G4Event* );

  inline void
  SetParticlePosition( const G4ThreeVector& v );
  inline void
  SetParticleDirection( const G4ThreeVector& v );

private:
  G4GeneralParticleSource* particleSource;
  LYSimDebugGeneratorMessenger* messenger;
};

class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;

class LYSimDebugGeneratorMessenger : public G4UImessenger
{
public:
  LYSimDebugGeneratorMessenger( LYSimDebugGeneratorAction* );
  ~LYSimDebugGeneratorMessenger();

  void SetNewValue( G4UIcommand*, G4String );

private:
  LYSimDebugGeneratorAction* genaction;

  G4UIdirectory* detDir;
  G4UIcmdWith3VectorAndUnit* SetParticlePosition;
  G4UIcmdWith3Vector*        SetParticleDirection;
};


#endif/*LYSimDebugGeneratorAction_h*/
