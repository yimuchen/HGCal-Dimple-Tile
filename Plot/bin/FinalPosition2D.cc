#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Format.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/interface/Flat2DCanvas.hpp"
#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "TBox.h"
#include "TChain.h"
#include "TEllipse.h"
#include "TFile.h"
#include "TGraphErrors.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc( "Input output options" );
  desc.add_options()
    ( "inputfile,f", usr::po::value<std::string>(),
    "File containing the generated files" )
    ( "output,o", usr::po::defvalue<std::string>(
    usr::resultpath( "HGCalTileSim", "Plot" ).string() ),
    "Output plot directory" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( GeometryOptions() );
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );
  args.SetFilePrefix( args.Arg( "output" ) );
  args.AddNameScheme( {
    {"dimplerad", "r"}, {"dimpleind", "d"}, {"dimpletype", "T"},
    {"tilewidth", "L"}, {"absmult", "a"},
    {"wrapreflect", "m"},
    {"sipmrefmult", "M"},
    {"sipmstandref", "s"},
    {"sipmwidth", "W"}, {"sipmstand", "S"},
    {"tilealpha", "A"}, {"dimplealpha", "D"},
    {"PCBRadius", "b"}, {"PCBRef", "P"}, {"coverref", "c"}, {"simprefalpha", "F"}
  } );

  const std::vector<std::string> options_list = {
    "dimplerad",    "dimpleind",        "dimpletype",
    "tilewidth",
    "absmult",      "wrapreflect",
    "sipmstandref", "sipmrefmult",
    "sipmwidth",    "sipmstand",
    "PCBRadius",    "PCBRef",
    "tilealpha",    "dimplealpha",      "coverref", "sipmrefalpha"
  };
  TChain tree( "LYSimRun", "LYSimRun" );
  LYSimRunFormat fmt;
  LYSimRunFormat first_fmt;
  LYvXGraphContainer graph_container;
  std::vector<std::string> difflist;
  std::vector<int> entrylist;
  double xmax = 0;

  tree.Add( args.Arg<std::string>( "inputfile" ).c_str() );
  fmt.LoadBranches( &tree );

  for( int i = 0; i < tree.GetEntries(); ++i ){
    tree.GetEntry( i );
    bool pass = true;

    for( const auto& opt : options_list ){
      if( args.CheckArg( opt )
          && FormatOpt( fmt, opt ) != args.Arg<double>( opt ) ){
        std::cout << "Failed!" << opt << " : "
                  << FormatOpt( fmt, opt ) << " "
                  << args.Arg<double>( opt ) << std::endl;
        pass = false;
        break;
      }
    }

    if( !pass ){continue;}

    if( entrylist.size() == 0 ){
      first_fmt = fmt;
      xmax      = std::max( fmt.tile_x/2, xmax );
    }
    entrylist.push_back( i );

    for( const auto& opt : options_list ){
      if( entrylist.size() > 0 &&
          FormatOpt( fmt, opt ) != FormatOpt( first_fmt, opt ) ){
        difflist.push_back( opt );
      }
    }
  }

  if( entrylist.size() == 0 ){
    std::cout << "NO PLOTS SATISFIED" << std::endl;
    return 0;
  }



  graph_container.LoadBranches( &tree );
  tree.GetEntry( entrylist.at( 0 ) );

  TH2D* hist = (TH2D*)graph_container.FinalPosition->Clone();
  std::cout <<hist->GetEntries() << std::endl;


  usr::plt::Flat2DCanvas c;

  c.PlotHist( hist,
    usr::plt::Plot2DF( usr::plt::heat ),
    usr::plt::EntryText( "Final photon position" ) );

  c.Pad().SetLogz( kTRUE );
  TEllipse PCBHole( 0, 0, first_fmt.pcb_rad );
  PCBHole.SetFillColorAlpha( 0, 0 );
  PCBHole.Draw();

  double sipm_hwdith = first_fmt.sipm_width/2;
  double sipm_fwidth = first_fmt.sipm_width/2 + first_fmt.sipm_rim;
  TBox SiPMActive( -sipm_hwdith, -sipm_hwdith, sipm_hwdith, sipm_hwdith );
  TBox SiPMArea( -sipm_fwidth, -sipm_fwidth, sipm_fwidth, sipm_fwidth );
  SiPMActive.SetFillColorAlpha( 0, 0 );
  SiPMArea.SetFillColorAlpha( 0, 0 );

  SiPMActive.Draw();
  SiPMArea.Draw();

  std::cout << args.MakePDFFile( "FinalPosition2D" ) << std::endl;
  c.SaveAsPDF( args.MakePDFFile( "FinalPosition2D" ) );

  return 0;

}
