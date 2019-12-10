#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"


bool
LYGraphCompare( const LYSimFormat& x, const LYSimFormat& y )
{
#define COMPARE_MEMBER( MEMBER ) \
  if( x.MEMBER != y.MEMBER ){ return x.MEMBER < y.MEMBER;  }

  COMPARE_MEMBER( beam_center_y );
  COMPARE_MEMBER( tile_width    );
  COMPARE_MEMBER( beam_width    );
  COMPARE_MEMBER( dimple_radius );
  COMPARE_MEMBER( dimple_indent );
  COMPARE_MEMBER( abs_mult      );
  COMPARE_MEMBER( wrap_reflect  );
  COMPARE_MEMBER( sipm_width    );
  COMPARE_MEMBER( sipm_stand    );
  COMPARE_MEMBER( sipm_rim      );
  COMPARE_MEMBER( pcb_radius    );
  COMPARE_MEMBER( pcb_reflect   );

  return x.beam_center_y < y.beam_center_y;
#undef COMPARE_MEMBER
}

// Generating ther graphname
static const char name_format[]
  = "Y%.1lf_L%.1lf"
    "_W%.1lf_R%.1lf_I%.1lf_A%.0lf_WR%.1lf"
    "_SIPMW%.1lf_SIPMS%.2lf"
    "_PCBRad%.1lf_PCBRef%0.1lf";
static const char scan_format[]
  = "Y%lf_L%lf"
    "_W%lf_R%lf_I%lf_A%lf_WR%lf"
    "_SIPMW%lf_SIPMS%lf"
    "_PCBRad%lf_PCBRef%lf";


std::string
LYGraphName( const LYSimFormat& x )
{
  return usr::fstr( name_format
                  , x.beam_center_y
                  , x.tile_width
                  , x.beam_width
                  , x.dimple_radius
                  , x.dimple_indent
                  , x.abs_mult * 100
                  , x.wrap_reflect *100
                  , x.sipm_width
                  , x.sipm_stand
                  , x.pcb_radius
                  , x.pcb_reflect * 100    );
}

// Generating a format
LYSimFormat
FromLYGraphName( const std::string& x )
{
  LYSimFormat ans;
  sscanf( x.c_str(), scan_format
        , &ans.beam_center_y
        , &ans.tile_width
        , &ans.beam_width
        , &ans.dimple_radius
        , &ans.dimple_indent
        , &ans.abs_mult
        , &ans.wrap_reflect
        , &ans.sipm_width
        , &ans.sipm_stand
        , &ans.pcb_radius
        , &ans.pcb_reflect    );
  ans.abs_mult     /= 100.0;
  ans.wrap_reflect /= 100.0;
  ans.pcb_reflect  /= 100.0;
  return ans;
}
