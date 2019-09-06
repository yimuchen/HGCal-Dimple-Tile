#ifndef LYSimPrimaryGeneratorAction_h
#define LYSimPrimaryGeneratorAction_h

#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#else
#include "LYSimDetectorConstruction.hh"
#endif

#include "globals.hh"
#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class LYSimDetectorConstruction;
class LYSimPrimaryGeneratorAction;
class LYSimPrimaryGeneratorMessenger;

class LYSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  LYSimPrimaryGeneratorAction( LYSimDetectorConstruction* );
  ~LYSimPrimaryGeneratorAction();

  void InitSource() ;
  void GeneratePrimaries( G4Event* );

  void MoveSource( const double x, const double y, const double width = 0 );

private:
  G4GeneralParticleSource* particleSource;
  LYSimDetectorConstruction* fDetector;
};

// ....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif/*LYSimPrimaryGeneratorAction_h*/
