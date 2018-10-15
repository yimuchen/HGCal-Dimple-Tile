// LYSimEventAction.cc

#include "LYSimEventAction.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "Analysis.hh"
#include "g4root.hh"
#include "G4EventManager.hh"
#include "Analysis.hh"
#include "AnalysisMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4PhysicalConstants.hh"
#include "LYSimDetectorConstruction.hh"
#include "G4ios.hh"
#include "g4root.hh"
#include "G4GeneralParticleSource.hh"
#include "LYSimTrajectoryPoint.hh"
#include "LYSimScintillation.hh"
#include "G4Event.hh"
#include "LYSimEventAction.hh"
#include "G4EventManager.hh"
#include "G4PrimaryVertex.hh"
#include "LYSimPrimaryGeneratorAction.hh"
#include "G4RootAnalysisManager.hh"


LYSimEventAction::LYSimEventAction()
{
G4AnalysisManager::Instance();
}


void LYSimEventAction::BeginOfEventAction(const G4Event* anEvent )
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    G4PrimaryVertex* primaryVertex = anEvent->GetPrimaryVertex();
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
    G4double ke = primaryParticle->GetKineticEnergy();
    if ( anEvent->GetEventID() % 100 == 0 )
    {
        G4cout<<"Starting Event: "<<anEvent->GetEventID()<<G4endl;
    }
    Analysis::GetInstance()->PrepareNewEvent(anEvent);
}

void LYSimEventAction::EndOfEventAction(const G4Event* anEvent)
{
    /*
    G4String hitCollName = "PMTHitsCollection";
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    static G4int hitCollID = -1;
    if ( hitCollID < 0 )
        hitCollID = SDman->GetCollectionID( hitCollName );

    G4HCofThisEvent* hitsCollections = 0;
    hitsCollections = anEvent->GetHCofThisEvent();

    LYSimPMTHitsCollection* hits = 0;
    if ( hitsCollections )
    {
        hits = static_cast<LYSimPMTHitsCollection*> ( hitsCollections->GetHC(hitCollID) );
    }
    else
    {
        G4cerr << "hitsCollection not found" << G4endl;
        return;
    }
    G4double nHits = hits->entries();
    if ( nHits == 1 ){
    	G4EventManager::GetEventManager()->KeepTheCurrentEvent();
    }
    */
    Analysis::GetInstance()->EndOfEvent(anEvent);   


}

