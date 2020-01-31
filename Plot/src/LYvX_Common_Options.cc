#include "UserUtils/Common/interface/ArgumentExtender.hpp"
// #include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"
#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

usr::po::options_description GeometryOptions()
{
  usr::po::options_description desc(
    "Explicitly stated options with just one entry would be listed in the plot. "
    "If the input file only includes a entry for a certain parameter, it will "
    "automatically be determined" );
  desc.add_options()
    ( "dimplerad,r", usr::po::value<double>(), "Dimple radius [mm]" )
    ( "dimpleind,d", usr::po::value<double>(), "Dimple indent [mm]" )
    ( "tilewidth,L", usr::po::value<double>(), "Tile width [mm]"    )
    ( "absmult,a",   usr::po::value<double>(),
    "Multple of inbuilt absorption length" )
    ( "wrapreflect,m", usr::po::value<double>(),
    "Wrap reflectivity" )
    ( "sipmwidth,W", usr::po::value<double>(), "SiPM Width [mm]" )
    ( "sipmstand,S",   usr::po::value<double>(), "SiPM stand height [mm]" )
    ( "PCBRadius,b", usr::po::value<double>(), "PCB Radius" )
    ( "PCBRef,P", usr::po::value<double>(), "PCB Reflectivity" )
  ;

  return desc;
}


double FormatOpt( const LYSimRunFormat& fmt, const std::string& opt )
{
  return opt == "dimplerad" ?   fmt.dimple_rad :
         opt == "dimpleind" ?   fmt.dimple_ind :
         opt == "tilewidth" ?   fmt.tile_x     :
         opt == "absmult" ?     fmt.abs_mult   :
         opt == "wrapreflect" ? fmt.wrap_ref   :
         opt == "sipmwidth" ?   fmt.sipm_width :
         opt == "sipmstand" ?   fmt.sipm_stand :
         opt == "PCBRadius" ?   fmt.pcb_rad    :
         opt == "PCBRef" ?      fmt.pcb_ref    :
         0.0;
}

std::string FormatOptString( const LYSimRunFormat& fmt, const std::string& opt )
{
  if( opt == "dimplerad" ){
    return usr::fstr( "Radius=%.1lf[mm]", fmt.dimple_rad );
  } else if( opt ==  "dimpleind" ){
    return usr::fstr( "Indent=%.1lf[mm]", fmt.dimple_ind );
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
  } else {
    return "";
  }
}
