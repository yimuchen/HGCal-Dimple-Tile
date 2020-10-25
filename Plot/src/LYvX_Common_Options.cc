#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"
#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"
#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"

boost::program_options::options_description
GeometryOptions()
{
  namespace po = boost::program_options;
  po::options_description desc(
    "Explicitly stated options with just one entry would be listed in the plot. "
    "If the input file only includes a entry for a certain parameter, it will "
    "automatically be determined" );
  desc.add_options()
    ( "dimplerad,r",   po::value<double>(), "Dimple radius [mm]"                   )
    ( "dimpleind,d",   po::value<double>(), "Dimple indent [mm]"                   )
    ( "dimpletype,T",  po::value<double>(), "Dimple Type (0:Spherical, 1:Ellipsoid, 2:FlatDome, 3:Cylinder" )
    ( "tilewidth,L",   po::value<double>(), "Tile width [mm]"                      )
    ( "absmult,a",     po::value<double>(), "Multple of inbuilt absorption length" )
    ( "wrapreflect,m", po::value<double>(), "Wrap reflectivity"                    )
    ( "tilealpha,A",   po::value<double>(), "Tile surface alpha value" )
    ( "dimplealpha,D", po::value<double>(), "Dimple surface alpha value" )
    ( "sipmwidth,W",   po::value<double>(), "SiPM Width [mm]"                      )
    ( "sipmstand,S",   po::value<double>(), "SiPM stand height [mm]"               )
    ( "PCBRadius,b",   po::value<double>(), "PCB Radius"                           )
    ( "PCBRef,P",      po::value<double>(), "PCB Reflectivity"                     )
  ;

  return desc;
}


double
FormatOpt( const LYSimRunFormat& fmt, const std::string& opt )
{
  return opt == "dimplerad"   ? fmt.dimple_rad   :
         opt == "dimpleind"   ? fmt.dimple_ind   :
         opt == "tilewidth"   ? fmt.tile_x       :
         opt == "absmult"     ? fmt.abs_mult     :
         opt == "wrapreflect" ? fmt.wrap_ref     :
         opt == "sipmwidth"   ? fmt.sipm_width   :
         opt == "sipmstand"   ? fmt.sipm_stand   :
         opt == "PCBRadius"   ? fmt.pcb_rad      :
         opt == "PCBRef"      ? fmt.pcb_ref      :
         opt == "dimpletype"  ? fmt.dimple_type  :
         opt == "tilealpha"   ? fmt.tile_alpha   :
         opt == "dimplealpha" ? fmt.dimple_alpha :
         0.0;
}

std::string
FormatOptString( const LYSimRunFormat& fmt, const std::string& opt )
{
  if( opt == "dimplerad" ){
    return usr::fstr( "Radius=%.1lf[mm]", fmt.dimple_rad );
  } else if( opt ==  "dimpleind" ){
    return usr::fstr( "Indent=%.1lf[mm]", fmt.dimple_ind );
  } else if( opt == "dimpletype" ){
    const std::string type_str = fmt.dimple_type == 0 ? "Spherical"   :
                                 fmt.dimple_type == 1 ? "Ellipsoid"   :
                                 fmt.dimple_type == 2 ? "Flat dome"   :
                                 fmt.dimple_type == 3 ? "Cylindrical" :
                                 "UNKNOWN";
    return usr::fstr( "Dim. Shape=%s", type_str );
  } else if( opt == "tilewidth" ){
    return usr::fstr( "Tile=%.0lf[mm]", fmt.tile_x   );
  } else if( opt == "absmult" ){
    return usr::fstr( "Abs. =%.1lf[cm]", fmt.abs_mult*380 );
  } else if( opt == "wrapreflect" ){
    return usr::fstr( "Wrap Ref. =%.1lf[%%]", fmt.wrap_ref *100 );
  } else if( opt == "sipmwidth" ){
    return usr::fstr( "SiPM Width =%.1lf[mm]", fmt.sipm_width   );
  } else if( opt ==  "sipmstand" ){
    return usr::fstr( "SiPM Stand =%0.2lf[mm]", fmt.sipm_stand   );
  } else if( opt == "PCBRadius" ){
    return usr::fstr( "PCB Rad. = %.2lf[mm]", fmt.pcb_rad   );
  } else if( opt ==  "PCBRef" ){
    return usr::fstr( "PCB Ref. = %.1lf[%%]", fmt.pcb_ref*100  );
  } else if( opt == "tilealpha" ){
    return usr::fstr( "Tile #alpha = %.2f[rad]", fmt.tile_alpha );
  } else if( opt == "dimplealpha" ){
    return usr::fstr( "Dimp. #alpha = %.2f[rad]", fmt.dimple_alpha );
  } else {
    return "";
  }

}
