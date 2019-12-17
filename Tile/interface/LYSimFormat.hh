#ifndef LYSIMFORMAT_HH
#define LYSIMFORMAT_HH

#include "TH1D.h"
#include "TTree.h"

class LYSimDetectorConstruction;

class LYSimFormat
{
public:
  double beam_x;
  double beam_y;
  unsigned nphotons;
  unsigned genphotons;

  uint16_t NumWrapReflection[10000];
  uint16_t OpticalLength[10000];
  int16_t EndX[10000];
  int16_t EndY[10000];
  uint8_t NumPCBReflection[10000];
  bool IsDetected[10000];

  void
  AddToTree( TTree* tree )
  {
    tree->Branch( "BeamX",      &beam_x     );
    tree->Branch( "BeamY",      &beam_y     );
    tree->Branch( "genphotons", &genphotons );
    tree->Branch( "nphotons",   &nphotons   );

    tree->Branch( "NumWrapReflection"
                , NumWrapReflection
                , "NumWrapReflection[genphotons]/s" );
    tree->Branch( "OpticalLength"
                , OpticalLength
                , "OpticalLength[genphotons]/s" );
    tree->Branch( "NumPCBReflection"
                , NumPCBReflection
                , "NumPCBReflection[genphotons]/b" );
    tree->Branch( "EndX"
                ,  EndX
                , "EndX[genphotons]/S" );
    tree->Branch( "EndY"
                ,  EndY
                , "EndY[genphotons]/S" );
    tree->Branch( "IsDetected"
                , IsDetected
                , "IsDetected[genphotons]/O" );
  }

  void
  LoadBranches( TTree* tree )
  {
    tree->SetBranchAddress( "BeamX",             &beam_x     );
    tree->SetBranchAddress( "BeamY",             &beam_y     );
    tree->SetBranchAddress( "genphotons",        &genphotons );
    tree->SetBranchAddress( "nphotons",          &nphotons   );

    tree->SetBranchAddress( "NumWrapReflection", NumWrapReflection );
    tree->SetBranchAddress( "OpticalLength",     OpticalLength     );
    tree->SetBranchAddress( "NumPCBReflection",  NumPCBReflection  );
    tree->SetBranchAddress( "EndX",              EndX              );
    tree->SetBranchAddress( "EndY",              EndY              );
    tree->SetBranchAddress( "IsDetected",        IsDetected        );
  }
};


class LYSimRunFormat
{
public:
  double tile_x;
  double tile_y;
  double tile_z;

  double sipm_width;
  double sipm_rim;
  double sipm_stand;

  double dimple_rad;
  double dimple_ind;

  double abs_mult;
  double wrap_ref;
  double pcb_rad;
  double pcb_ref;

  double beam_x;
  double beam_y;
  double beam_w;

  unsigned long long start_event;
  unsigned long long end_event;

  void AddToTree( TTree* tree )
  {
    tree->Branch( "TileX",     &tile_x      );
    tree->Branch( "TileY",     &tile_y      );
    tree->Branch( "TileZ",     &tile_y      );
    tree->Branch( "SiPMW",     &sipm_width  );
    tree->Branch( "SiPMR",     &sipm_rim    );
    tree->Branch( "SiPMS",     &sipm_stand  );
    tree->Branch( "DimpleRad", &dimple_rad  );
    tree->Branch( "DimpleInd", &dimple_ind  );
    tree->Branch( "AbsMult",   &abs_mult    );
    tree->Branch( "WrapRef",   &wrap_ref    );
    tree->Branch( "PCBRad",    &pcb_rad     );
    tree->Branch( "PCBRef",    &pcb_ref     );
    tree->Branch( "BeamX",     &beam_x      );
    tree->Branch( "BeamY",     &beam_y      );
    tree->Branch( "BeamZ",     &beam_w      );
    tree->Branch( "Start",     &start_event );
    tree->Branch( "End",       &end_event   );
  }

  void LoadBranches( TTree* tree )
  {
    tree->SetBranchAddress( "TileX",     &tile_x      );
    tree->SetBranchAddress( "TileY",     &tile_y      );
    tree->SetBranchAddress( "TileZ",     &tile_y      );
    tree->SetBranchAddress( "SiPMW",     &sipm_width  );
    tree->SetBranchAddress( "SiPMR",     &sipm_rim    );
    tree->SetBranchAddress( "SiPMS",     &sipm_stand  );
    tree->SetBranchAddress( "DimpleRad", &dimple_rad  );
    tree->SetBranchAddress( "DimpleInd", &dimple_ind  );
    tree->SetBranchAddress( "AbsMult",   &abs_mult    );
    tree->SetBranchAddress( "WrapRef",   &wrap_ref    );
    tree->SetBranchAddress( "PCBRad",    &pcb_rad     );
    tree->SetBranchAddress( "PCBRef",    &pcb_ref     );
    tree->SetBranchAddress( "BeamX",     &beam_x      );
    tree->SetBranchAddress( "BeamY",     &beam_y      );
    tree->SetBranchAddress( "BeamZ",     &beam_w      );
    tree->SetBranchAddress( "Start",     &start_event );
    tree->SetBranchAddress( "End",       &end_event   );
  }
};

#endif
