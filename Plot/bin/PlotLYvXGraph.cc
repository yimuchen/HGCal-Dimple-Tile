#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Format.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "TChain.h"
#include "TFile.h"
#include "TGraphErrors.h"

double      FormatOpt( const LYSimRunFormat&, const std::string& );
std::string FormatOptString( const LYSimRunFormat&, const std::string& );
TGraph*     MakeGraph( const LYSimRunFormat&, const LYvXGraphContainer& );

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
    usr::resultpath( "HGCalTileSim", "Plot" ).string() ),
    "Output plot directory" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );
  args.SetFilePrefix( args.Arg( "output" ) );
  args.AddNameScheme( {
    {"dimplerad", "r"}, {"dimpleind", "d"},
    {"tilewidth", "L"}, {"absmult", "a"}, {"wrapreflect", "m"},
    {"sipmwidth", "W"}, {"sipmstand", "S"}, {"PCBRadius", "b"}, {"PCBRef", "P"}
  } );

  const std::vector<std::string> options_list = {
    "dimplerad", "dimpleind",
    "tilewidth",
    "absmult",   "wrapreflect",
    "sipmwidth", "sipmstand",
    "PCBRadius", "PCBRef"
  };
  TChain tree( "LYSimRun", "LYSimRun" );
  LYSimRunFormat fmt;
  LYSimRunFormat first_fmt;
  LYvXGraphContainer graph_container;
  std::vector<std::string> speclist;
  std::vector<std::string> difflist;
  std::vector<int> entrylist;
  double xmax = 0;

  tree.Add( args.Arg<std::string>( "inputfile" ).c_str() );
  fmt.LoadBranches( &tree );

  for( int i = 0; i < tree.GetEntries(); ++i ){
    tree.GetEntry( i );
    if( i == 0 ){ first_fmt = fmt; }
    bool pass = true;

    for( const auto& opt : options_list ){
      if( args.CheckArg( opt )
          && FormatOpt( fmt, opt ) != args.Arg<double>( opt ) ){
        pass = false;
        break;
      }
    }

    if( !pass ){continue;}

    entrylist.push_back( i );
    xmax = std::max( fmt.tile_x/2, xmax );

    for( const auto& opt : options_list ){
      if( FormatOpt( fmt, opt ) != FormatOpt( first_fmt, opt ) ){
        difflist.push_back( opt );
      }
    }
  }

  if( entrylist.size() == 0 ){
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

  graph_container.LoadBranches( &tree );

  for( const auto& runentry : entrylist ){
    tree.GetEntry( runentry );

    TGraph* graph = MakeGraph( fmt, graph_container );

    std::string entry= "Mytest";

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
    tree.GetEntry( entrylist.front() );
    c.Pad().DrawVLine( fmt.dimple_rad,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
    c.Pad().DrawVLine( -fmt.dimple_rad,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  }

  if( !usr::FindValue( difflist, std::string( "tilewidth" ) ) ){
    tree.GetEntry( entrylist.front() );
    c.Pad().DrawVLine( fmt.tile_x/2,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
    c.Pad().DrawVLine( -fmt.tile_x/2,
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  }

  c.Pad().SetDataMax( c.Pad().GetDataMax() * 1.3 );
  c.Pad().Xaxis().SetTitle( "Beam Center X [mm]" );
  c.Pad().Yaxis().SetTitle( "Detected Photons" );

  c.DrawLuminosity( usr::fstr( "Trigger Width: %.1lf[mm]", width ) );
  c.DrawCMSLabel( "Simulation", "HGCal" );

  for( const auto opt : options_list ){
    tree.GetEntry( entrylist.front() );
    if( args.CheckArg( opt ) ){
      c.Pad().WriteLine( FormatOptString( fmt, opt ) );
    }
  }

  c.SaveAsPDF( args.MakePDFFile( "LYvX" ) );

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

TGraph* MakeGraph( const LYSimRunFormat& fmt, const LYvXGraphContainer& cont )
{
  const double width = fmt.beam_w;

  std::vector<double> x_list;
  std::vector<double> y_list;
  std::vector<double> yerr_list;
  std::vector<double> xerr_list;

  for( int i = 1; i <= cont.LYvX->GetNcells(); ++i ){
    if( cont.LYvX->GetBinContent( i ) != 0 ){
      xerr_list.push_back( width );
      x_list.push_back( cont.LYvX->GetBinCenter( i ) );
      y_list.push_back( cont.LYvX->GetBinContent( i ) );
      yerr_list.push_back( cont.LYvX->GetBinError( i ) );
      std::cout << x_list.back() << " "
                << y_list.back() << " "
                << yerr_list.back() << std::endl;
    }
  }

  return new TGraphErrors( x_list.size()
                         , x_list.data(), y_list.data()
                         , xerr_list.data(), yerr_list.data() );
}
