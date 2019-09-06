#ifdef CMSSW_GIT_HASH
#include "HGCalTileSim/Tile/interface/Analysis.hh"
#include "HGCalTileSim/Tile/interface/LYSimRunAction.hh"
#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#else
#include "Analysis.hh"
#include "LYSimRunAction.hh"
#include "LYSimDetectorConstruction.hh"
#endif

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "g4root.hh"
#include <iostream>

LYSimRunAction::LYSimRunAction(
  LYSimDetectorConstruction* ipDetectorConstruction )
{
//     runMessenger = new LYSimRunActionMessenger(this);
  pDetectorConstruction = ipDetectorConstruction;
  G4AnalysisManager::Instance();

  G4AnalysisManager* man = G4AnalysisManager::Instance();
  outFileName = Analysis::GetInstance()->GetROOTFileName();
  G4cout << "Output filename: " << outFileName << G4endl;
  man->OpenFile( outFileName.c_str() );
  man->SetFirstHistoId( 1 );

  // Create histogram(s) (avoid non-integer bins)
  man->CreateH1( "ogammaE",   "Optical photons Wavelength [nm]",       100, 200.,  1200. );
  man->CreateH1( "Ndetect",   "Number of detected photons per event",  500,   0.,    500. );
}

LYSimRunAction::~LYSimRunAction()
{
  // Save histograms and tuples
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();


  //  delete runMessenger;
  delete G4AnalysisManager::Instance();
  delete Analysis::GetInstance();
}

void
LYSimRunAction::BeginOfRunAction( const G4Run* aRun )
{
  Analysis::GetInstance()->PrepareNewRun( aRun );
}

void
LYSimRunAction::EndOfRunAction( const G4Run* aRun )
{
  Analysis::GetInstance()->EndOfRun( aRun );
}
