#ifndef LYSIMFORMAT_HH
#define LYSIMFORMAT_HH

#include "TTree.h"

class LYSimFormat
{
public:
  double beam_center_x;
  double beam_center_y;
  double beam_width;
  double dimple_radius;
  double dimple_indent;

  double beam_x;
  double beam_y;

  double abs_mult;
  double wrap_reflect;

  unsigned genphotons;
  unsigned nphotons;

  void
  AddToTree( TTree* tree )
  {
    tree->Branch( "BeamCenterX",  &beam_center_x );
    tree->Branch( "BeamCenterY",  &beam_center_y );
    tree->Branch( "BeamWidth",    &beam_width    );
    tree->Branch( "DimpleRadius", &dimple_radius );
    tree->Branch( "DimpleIndent", &dimple_indent );
    tree->Branch( "BeamX",        &beam_x        );
    tree->Branch( "BeamY",        &beam_y        );
    tree->Branch( "absmult",      &abs_mult      );
    tree->Branch( "Reflectivity", &wrap_reflect  );
    tree->Branch( "genphotons",   &genphotons    );
    tree->Branch( "nphotons",     &nphotons      );
  }

  void
  LoadBranches( TTree* tree )
  {
    tree->SetBranchAddress( "BeamCenterX",  &beam_center_x );
    tree->SetBranchAddress( "BeamCenterY",  &beam_center_y );
    tree->SetBranchAddress( "BeamWidth",    &beam_width    );
    tree->SetBranchAddress( "DimpleRadius", &dimple_radius );
    tree->SetBranchAddress( "DimpleIndent", &dimple_indent );
    tree->SetBranchAddress( "BeamX",        &beam_x        );
    tree->SetBranchAddress( "BeamY",        &beam_y        );
    tree->SetBranchAddress( "absmult",      &abs_mult      );
    tree->SetBranchAddress( "Reflectivity", &wrap_reflect  );
    tree->SetBranchAddress( "genphotons",   &genphotons    );
    tree->SetBranchAddress( "nphotons",     &nphotons      );
  }
};

#endif