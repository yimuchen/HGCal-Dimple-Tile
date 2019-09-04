#ifndef MATERIALS_HH
#define MATERIAL_HH

#include "G4Material.hh"

// list of functions for constructing non-standard material in Geant4
extern G4Material* Make_Bialkali();
extern G4Material* Make_Epoxy();
extern G4Material* Make_Epoxy2();
extern G4Material* Make_LYSO();
extern G4Material* Make_FiberCore();
extern G4Material* Make_FiberInner();
extern G4Material* Make_FiberOuter();
extern G4Material* Make_SCSN81( const double, const double );
extern G4Material* Make_EJ200( const double );
extern G4Material* Make_EJ260( const double, const double );

extern G4Material* Make_Custom_Air();
extern G4Material* Make_Custom_SiO2();
extern G4Material* Make_Custom_PolyStyrene();
extern G4Material* Make_Custom_Pyrex();
extern G4Material* Make_Custom_Water();


#endif
