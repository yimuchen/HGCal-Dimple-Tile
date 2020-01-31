#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/LYSimPMTHit.hh"
#else
#include "LYSimPMTHit.hh"
#endif

#include "G4VisManager.hh"

G4Allocator<LYSimPMTHit> LYSimPMTHitAllocator;

G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();


LYSimPMTHit::LYSimPMTHit()
{
  fEnergy      = 0.;
  fPhotonCount = 0;
  fTime        = 0.;
  fLength      = 0.;
  fBounceCount = 0;
}

LYSimPMTHit::~LYSimPMTHit(){}

LYSimPMTHit::LYSimPMTHit( const LYSimPMTHit & right )
  : G4VHit()
{
  *this = right;
}

const LYSimPMTHit&
LYSimPMTHit::operator=( const LYSimPMTHit & right )
{
  fEnergy      = right.fEnergy;
  fPhotonCount = right.fPhotonCount;
  fTime        = right.fTime;
  fLength      = right.fLength;
  fBounceCount = right.fBounceCount;
  return *this;
}

G4int
LYSimPMTHit::operator==( const LYSimPMTHit& right ) const
{
  return fEnergy == right.fEnergy           &&
         fPhotonCount == right.fPhotonCount &&
         fTime == right.fTime               &&
         fBounceCount == right.fBounceCount  &&
         fLength == right.fLength;
}

void
LYSimPMTHit::Draw()
{
}

void
LYSimPMTHit::Print(){}
