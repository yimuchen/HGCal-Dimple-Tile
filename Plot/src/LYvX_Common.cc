#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"


bool LYGraphCompare( const LYSimFormat& x, const LYSimFormat& y )
{
#define COMPARE_MEMBER( MEMBER )\
  if( x.MEMBER != y.MEMBER ){ return x.MEMBER < y.MEMBER;  }

  COMPARE_MEMBER( beam_center_y );
  COMPARE_MEMBER( beam_width );
  COMPARE_MEMBER( dimple_radius );
  COMPARE_MEMBER( dimple_indent );
  COMPARE_MEMBER( abs_mult );
  COMPARE_MEMBER( wrap_reflect );
  COMPARE_MEMBER( sipm_width );
  COMPARE_MEMBER( sipm_stand );
  COMPARE_MEMBER( sipm_rim );

  return x.beam_center_y < y.beam_center_y;
#undef COMPARE_MEMBER
}

// Generating ther graphname
std::string LYGraphName( const LYSimFormat& x )
{
  return usr::fstr(
    "Y%.1lf_W%.1lf_R%.1lf_I%.1lf_A%.0lf_WR%.0lf_SIPMW%.1lf_SIPMS%.2lf"
                 , x.beam_center_y
                 , x.beam_width
                 , x.dimple_radius
                 , x.dimple_indent
                 , x.abs_mult * 100
                 , x.wrap_reflect *100
                 , x.sipm_width
                 , x.sipm_stand );
}
