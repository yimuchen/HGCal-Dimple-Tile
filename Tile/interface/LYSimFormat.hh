#ifndef LYSIMFORMAT_HH
#define LYSIMFORMAT_HH

#include "CLHEP/Units/SystemOfUnits.h"
#include "TTree.h"

#include <cstdint>

#ifdef CMSSW_GIT_HASH
#include "UserUtils/Common/interface/Maths.hpp"
#endif

// Cannot use constexpr or static for backwards compatibility.
#define LYSIMFORMAT_MAX_PHOTONS 10000

class LYSimDetectorConstruction;

class LYSimFormat
{
public:
  // Constant global variables for saving the units
  // (No constexpr for backware compatibility)
  static const float opt_length_unit;
  static const float end_pos_unit;

  unsigned run_hash;
  unsigned event_hash;

  double beam_x;
  double beam_y;
  unsigned nphotons;
  unsigned genphotons;
  unsigned savedphotons;

  uint16_t NumWrapReflection[LYSIMFORMAT_MAX_PHOTONS];
  uint16_t OpticalLength[LYSIMFORMAT_MAX_PHOTONS];
  int16_t EndX[LYSIMFORMAT_MAX_PHOTONS];
  int16_t EndY[LYSIMFORMAT_MAX_PHOTONS];
  uint8_t NumPCBReflection[LYSIMFORMAT_MAX_PHOTONS];
  bool IsDetected[LYSIMFORMAT_MAX_PHOTONS];

  void
  AddToTree( TTree* tree )
  {
    tree->Branch( "RunHash",      &run_hash     );
    tree->Branch( "EventHash",    &event_hash   );
    tree->Branch( "BeamX",        &beam_x       );
    tree->Branch( "BeamY",        &beam_y       );
    tree->Branch( "genphotons",   &genphotons   );
    tree->Branch( "nphotons",     &nphotons     );
    tree->Branch( "savedphotons", &savedphotons );

    tree->Branch( "NumWrapReflection"
                , NumWrapReflection
                , "NumWrapReflection[savedphotons]/s" );
    tree->Branch( "OpticalLength"
                , OpticalLength
                , "OpticalLength[savedphotons]/s" );
    tree->Branch( "NumPCBReflection"
                , NumPCBReflection
                , "NumPCBReflection[savedphotons]/b" );
    tree->Branch( "EndX"
                ,  EndX
                , "EndX[savedphotons]/S" );
    tree->Branch( "EndY"
                ,  EndY
                , "EndY[savedphotons]/S" );
    tree->Branch( "IsDetected"
                , IsDetected
                , "IsDetected[savedphotons]/O" );
  }

  void
  LoadBranches( TTree* tree )
  {
    tree->SetBranchAddress( "RunHash",           &run_hash     );
    tree->SetBranchAddress( "EventHash",         &event_hash   );
    tree->SetBranchAddress( "BeamX",             &beam_x       );
    tree->SetBranchAddress( "BeamY",             &beam_y       );
    tree->SetBranchAddress( "genphotons",        &genphotons   );
    tree->SetBranchAddress( "nphotons",          &nphotons     );
    tree->SetBranchAddress( "savedphotons",      &savedphotons );

    tree->SetBranchAddress( "NumWrapReflection", NumWrapReflection );
    tree->SetBranchAddress( "OpticalLength",     OpticalLength     );
    tree->SetBranchAddress( "NumPCBReflection",  NumPCBReflection  );
    tree->SetBranchAddress( "EndX",              EndX              );
    tree->SetBranchAddress( "EndY",              EndY              );
    tree->SetBranchAddress( "IsDetected",        IsDetected        );

    tree->BuildIndex( "RunHash", "EventHash" );
  }

#ifdef CMSSW_GIT_HASH
  void
  UpdateHash()
  {
    event_hash = 0;
    event_hash = usr::Hash32Join( event_hash, usr::HashValue32( beam_x     ) );
    event_hash = usr::Hash32Join( event_hash, usr::HashValue32( beam_y     ) );
    event_hash = usr::Hash32Join( event_hash, usr::HashValue32( nphotons   ) );
    event_hash = usr::Hash32Join( event_hash, usr::HashValue32( genphotons ) );
  }
#endif

};


class LYSimRunFormat
{
public:
  double tile_x;
  double tile_y;
  double tile_z;

  double cover_ref ;

  double sipm_refmult;
  double sipm_refalpha;
  double sipm_stand_ref;

  double sipm_width;
  double sipm_rim;
  double sipm_stand;

  double dimple_type;

  double dimple_rad;
  double dimple_ind;

  double abs_mult;
  double wrap_ref;
  double tile_alpha;
  double dimple_alpha;
  double pcb_rad;
  double pcb_ref;

  double beam_x;
  double beam_y;
  double beam_w;


  unsigned run_hash;

  void
  AddToTree( TTree* tree )
  {
    tree->Branch( "TileX",      &tile_x         );
    tree->Branch( "TileY",      &tile_y         );
    tree->Branch( "TileZ",      &tile_z         );
    tree->Branch( "Cover",      &cover_ref      );
    tree->Branch( "SiPMAAlpha", &sipm_refalpha  );
    tree->Branch( "SiPMARef",   &sipm_refmult   );
    tree->Branch( "SiPMSRef",   &sipm_stand_ref );
    tree->Branch( "SiPMW",      &sipm_width     );
    tree->Branch( "SiPMR",      &sipm_rim       );
    tree->Branch( "SiPMS",      &sipm_stand     );
    tree->Branch( "DimpleType", &dimple_type    );
    tree->Branch( "DimpleRad",  &dimple_rad     );
    tree->Branch( "DimpleInd",  &dimple_ind     );
    tree->Branch( "AbsMult",    &abs_mult       );
    tree->Branch( "WrapRef",    &wrap_ref       );
    tree->Branch( "TileAlpha",  &tile_alpha     );
    tree->Branch( "DimpAlpha",  &dimple_alpha   );
    tree->Branch( "PCBRad",     &pcb_rad        );
    tree->Branch( "PCBRef",     &pcb_ref        );
    tree->Branch( "BeamX",      &beam_x         );
    tree->Branch( "BeamY",      &beam_y         );
    tree->Branch( "BeamZ",      &beam_w         );
    tree->Branch( "RunHash",    &run_hash       );
  }

  void
  LoadBranches( TTree* tree )
  {
    tree->SetBranchAddress( "TileX",      &tile_x         );
    tree->SetBranchAddress( "TileY",      &tile_y         );
    tree->SetBranchAddress( "TileZ",      &tile_z         );
    tree->SetBranchAddress( "Cover",      &cover_ref      );
    tree->SetBranchAddress( "SiPMAAlpha", &sipm_refalpha  );
    tree->SetBranchAddress( "SiPMARef",   &sipm_refmult   );
    tree->SetBranchAddress( "SiPMSRef",   &sipm_stand_ref );
    tree->SetBranchAddress( "SiPMW",      &sipm_width     );
    tree->SetBranchAddress( "SiPMR",      &sipm_rim       );
    tree->SetBranchAddress( "SiPMS",      &sipm_stand     );
    tree->SetBranchAddress( "DimpleType", &dimple_type    );
    tree->SetBranchAddress( "DimpleRad",  &dimple_rad     );
    tree->SetBranchAddress( "DimpleInd",  &dimple_ind     );
    tree->SetBranchAddress( "AbsMult",    &abs_mult       );
    tree->SetBranchAddress( "WrapRef",    &wrap_ref       );
    tree->SetBranchAddress( "TileAlpha",  &tile_alpha     );
    tree->SetBranchAddress( "DimpAlpha",  &dimple_alpha   );
    tree->SetBranchAddress( "PCBRad",     &pcb_rad        );
    tree->SetBranchAddress( "PCBRef",     &pcb_ref        );
    tree->SetBranchAddress( "BeamX",      &beam_x         );
    tree->SetBranchAddress( "BeamY",      &beam_y         );
    tree->SetBranchAddress( "BeamZ",      &beam_w         );
    tree->SetBranchAddress( "RunHash",    &run_hash       );

    tree->BuildIndex( "RunHash" );
  }

#ifdef CMSSW_GIT_HASH
  void
  UpdateHash()
  {
    run_hash = 0;
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( tile_x         ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( tile_x         ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( tile_y         ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( tile_z         ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( cover_ref      ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( sipm_refalpha  ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( sipm_refmult   ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( sipm_stand_ref ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( sipm_width     ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( sipm_rim       ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( sipm_stand     ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( dimple_type    ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( dimple_rad     ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( dimple_ind     ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( abs_mult       ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( wrap_ref       ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( tile_alpha     ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( dimple_alpha   ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( pcb_rad        ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( pcb_ref        ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( beam_x         ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( beam_y         ) );
    run_hash = usr::Hash32Join( run_hash, usr::HashValue32( beam_w         ) );
  }
#endif
};

#endif
