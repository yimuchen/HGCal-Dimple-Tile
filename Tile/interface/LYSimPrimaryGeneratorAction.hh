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

  void GeneratePrimaries( G4Event* );

  inline double
  GetBeamX() const { return _beamx; }
  inline void
  SetBeamX( const double x ){ _beamx = x; }
  inline double
  GetBeamY() const { return _beamy; }
  inline void
  SetBeamY( const double x ){ _beamy = x; }
  inline double
  GetWidth() const { return _width; }
  inline void
  SetWidth( const double x ){ _width = x; }
  inline double
  GetPhotonMult() const { return _photon_multiplier; }
  inline void
  SetPhotonMult( const double x ){ _photon_multiplier = x; }

  void RandomizePosition();

private:
  G4GeneralParticleSource* particleSource;
  LYSimDetectorConstruction* fDetector;
  LYSimPrimaryGeneratorMessenger* messenger;

  // Defining the particle source geometry.
  double _beamx;
  double _beamy;
  double _width;
  double _photon_multiplier;
};

#endif/*LYSimPrimaryGeneratorAction_h*/
