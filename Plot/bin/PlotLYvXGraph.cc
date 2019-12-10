#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Format.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "TFile.h"


double      FormatOpt( const LYSimFormat&, const std::string& );
std::string FormatOptString( const LYSimFormat&, const std::string& );

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria, Explicitly stated "
    "options with just one entry would be listed in the plot. If the input file "
    "only includes a entry for a certain parameter, it will automatically be "
    "determined" );
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
    ( "inputfile,f", usr::po::value<std::string>(),
    "File containing the generated files" )
    ( "output,o", usr::po::defvalue<std::string>(
    ( usr::resultpath( "HGCalTileSim", "Plot" )/"LYvX.pdf" ).string() ),
    "Output plot file" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  TFile file( args.Arg<std::string>( "inputfile" ).c_str() );
  const std::vector<std::string> options_list = {
    "dimplerad", "dimpleind",
    "tilewidth",
    "absmult",   "wrapreflect",
    "sipmwidth", "sipmstand",
    "PCBRadius", "PCBRef"
  };
  std::vector<LYSimFormat> fmtlist;
  std::vector<std::string> speclist;
  std::vector<std::string> difflist;
  double xmax = 0;

  for( const auto&& key : *file.GetListOfKeys() ){
    bool pass       = true;
    LYSimFormat fmt = FromLYGraphName( key->GetName() );

    for( const auto& opt : options_list ){
      if( args.CheckArg( opt )
          && FormatOpt( fmt, opt ) != args.Arg<double>( opt ) ){
        pass = false;
        break;
      }
    }

    if( !pass ){continue;}

    fmtlist.push_back( fmt );
    xmax = std::max( fmt.tile_width/2, xmax );

    for( const auto& opt : options_list ){
      if( FormatOpt( fmt, opt ) != FormatOpt( fmtlist.front(), opt ) ){
        difflist.push_back( opt );
      }
    }
  }

  if( fmtlist.size() == 0 ){
    std::cout << "NO PLOTS SATISFIED" << std::endl;
    return 0;
  }

  difflist = usr::RemoveDuplicate( difflist );

  const std::vector<int> colorlist = {
    usr::plt::col::black,
    usr::plt::col::darkblue,
    usr::plt::col::darkred,
    usr::plt::col::darkgreen,
    usr::plt::col::darkorange,
    usr::plt::col::darkviolet
  };

  const std::vector<int> filllist = {
    usr::plt::col::gray,
    usr::plt::col::lightblue,
    usr::plt::col::pink,
    usr::plt::col::lightgreen,
    usr::plt::col::lightgoldenrodyellow,
    usr::plt::col::palevioletred
  };


  usr::plt::Simple1DCanvas c;
  TH1D dummyhist( "", "", 1, -xmax - 5, xmax + 5 );
  c.PlotHist( dummyhist, usr::plt::PlotType( usr::plt::hist ) );

  unsigned idx = 0;
  double width = 0;

  for( const auto& fmt : fmtlist ){
    if( fmt.pcb_reflect == 0.3 ){ continue; }
    const std::string graphname = LYGraphName( fmt );
    std::cout << graphname << std::endl;
    TGraph* graph = (TGraph*)( file.Get( graphname.c_str() ) );
    if( !graph ){ continue; }

    std::string entry;

    for( const auto opt : difflist ){
      entry += FormatOptString( fmt, opt ) + ";";
    }

    entry.pop_back();

    c.PlotGraph( graph,
      usr::plt::PlotType( usr::plt::fittedfunc ),
      usr::plt::TrackY( usr::plt::TrackY::max ),
      usr::plt::LineColor( colorlist[idx] ),
      usr::plt::FillColor( filllist[idx] ),
      usr::plt::FillStyle( usr::plt::sty::fillsolid ),
      usr::plt::EntryText( entry ) );

    width = graph->GetErrorX( 0 );
    ++idx;
    std::cout << "Plot" << std::endl;
  }

  if( !usr::FindValue( difflist,  std::string( "dimplerad" ) ) ){
    c.Pad().DrawVLine( fmtlist.front().dimple_radius,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
    c.Pad().DrawVLine( -fmtlist.front().dimple_radius,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  }

  if( !usr::FindValue( difflist, std::string( "tilewidth" ) ) ){
    c.Pad().DrawVLine( fmtlist.front().tile_width/2,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
    c.Pad().DrawVLine( -fmtlist.front().tile_width/2,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  }

  c.Pad().SetDataMax( c.Pad().GetDataMax() * 1.3 );
  c.Pad().Xaxis().SetTitle( "Beam Center X [mm]" );
  c.Pad().Yaxis().SetTitle( "Detected Photons" );

  c.DrawLuminosity( usr::fstr( "Trigger Width: %.1lf[mm]", width ) );
  c.DrawCMSLabel( "Simulation", "HGCal" );

  for( const auto opt : options_list ){
    if( args.CheckArg( opt ) ){
      c.Pad().WriteLine( FormatOptString( fmtlist.front(), opt ) );
    }
  }

  c.SaveAsPDF( args.Arg<std::string>( "output" ) );

}

double FormatOpt( const LYSimFormat& fmt, const std::string& opt )
{
  return opt == "dimplerad" ?   fmt.dimple_radius :
         opt == "dimpleind" ?   fmt.dimple_indent :
         opt == "tilewidth" ?   fmt.tile_width    :
         opt == "absmult" ?     fmt.abs_mult      :
         opt == "wrapreflect" ? fmt.wrap_reflect  :
         opt == "sipmwidth" ?   fmt.sipm_width    :
         opt == "sipmstand" ?   fmt.sipm_stand    :
         opt == "PCBRadius" ?   fmt.pcb_radius    :
         opt == "PCBRef" ?      fmt.pcb_reflect   :
         0.0;
}

std::string FormatOptString( const LYSimFormat& fmt, const std::string& opt )
{
  if( opt == "dimplerad" ){
    return usr::fstr( "Radius=%.1lf[mm]", fmt.dimple_radius );
  } else if( opt ==  "dimpleind" ){
    return usr::fstr( "Indent=%.1lf[mm]", fmt.dimple_indent );
  } else if( opt == "tilewidth" ){
    return usr::fstr( "Tile=%.0lf[mm]", fmt.tile_width   );
  } else if( opt == "absmult" ){
    return usr::fstr( "Abs. =%.1lf[cm]", fmt.abs_mult*380 );
  } else if( opt == "wrapreflect" ){
    return usr::fstr( "Wrap Ref. =%.1lf[%%]", fmt.wrap_reflect *100 );
  } else if( opt == "sipmwidth" ){
    return usr::fstr( "SiPM Width =%.1lf[mm]", fmt.sipm_width   );
  } else if( opt ==  "sipmstand" ){
    return usr::fstr( "SiPM Stand =%0.2lf[mm]", fmt.sipm_stand   );
  } else if( opt == "PCBRadius" ){
    return usr::fstr( "PCB Rad. = %.2lf[mm]", fmt.pcb_radius   );
  } else if( opt ==  "PCBRef" ){
    return usr::fstr( "PCB Ref. = %.1lf[%%]", fmt.pcb_reflect*100  );
  } else {
    return "";
  }
}
