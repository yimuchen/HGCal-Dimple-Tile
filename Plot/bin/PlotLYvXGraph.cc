#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "TFile.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "dimplerad,r", usr::po::multivalue<double>(),
    "List of different dimple radius to include in the plot" )
    ( "dimpleind,d", usr::po::multivalue<double>(),
    "List of different dimple indents to include in the plot" )
    ( "inputfile,f", usr::po::value<std::string>(),
    "File containing the generated files" )
    ( "output,o", usr::po::defvalue<std::string>(
    (usr::resultpath( "HGCalTileSim", "Plot" )/"LYvX.pdf").string() ),
    "Output plot file" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  const std::vector<double> rlist = args.ArgList<double>( "dimplerad" );
  const std::vector<double> dlist = args.ArgList<double>( "dimpleind" );

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

  TFile file( args.Arg<std::string>( "inputfile" ).c_str() );

  usr::plt::Simple1DCanvas c;
  TH1D dummyhist( "", "", 1, -20.0, +20.0 );// dummy for setting up plot x range
  c.PlotHist( dummyhist );

  unsigned idx = 0;
  double width = 0;

  for( const auto r : rlist ){
    for( const auto d : dlist ){
      const std::string graphname = usr::fstr( "R%.1lf_D%.1lf", r, d );
      TGraph* graph               = (TGraph*)( file.Get( graphname.c_str() ) );
      c.PlotGraph( graph,
        usr::plt::PlotType( usr::plt::fittedfunc ),
        usr::plt::TrackY( usr::plt::TrackY::both ),
        usr::plt::LineColor( colorlist[idx] ),
        usr::plt::FillColor( filllist[idx] ),
        usr::plt::FillStyle( usr::plt::sty::fillsolid ),
        usr::plt::EntryText( usr::fstr( "R=%.1lfmm D=%.1lfmm", r, d ) )
        );
      width = graph->GetErrorX( 0 );
      ++idx;
    }
  }

  c.Pad().Xaxis().SetTitle( "Beam Center X [mm]" );
  c.Pad().Yaxis().SetTitle( "Detected Photons" );

  c.DrawLuminosity( usr::fstr( "Trigger Width: %.1lf", width ) );
  c.DrawCMSLabel( "Simulation", "HGCal" );
  c.SaveAsPDF( args.Arg<std::string>( "output" ) );

}
