#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Format.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"
#include "UserUtils/PlotUtils/interface/Flat2DCanvas.hpp"

#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "TChain.h"
#include "TFile.h"
#include "TGraphErrors.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc( "Input options" );
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
  LYvXGraphContainer cont;
  int entry = -1;

  tree.Add( args.Arg<std::string>( "inputfile" ).c_str() );
  fmt.LoadBranches( &tree );
  cont.LoadBranches( &tree );

  for( int i = 0; i < tree.GetEntries(); ++i ){
    tree.GetEntry( i );
    bool pass = true;

    for( const auto& opt : options_list ){
      if( args.CheckArg( opt )
          && FormatOpt( fmt, opt ) != args.Arg<double>( opt ) ){
        pass = false;
        break;
      }
    }

    if( !pass ){
      continue;
    } else {
      entry = i;
      break;
    }
  }

  if( entry < 0 ){
    std::cout << "NO PLOTS SATISFIED" << std::endl;
    return 0;
  }

  TH2D* all_hist = cont.FinalPosition;
  TH2D* det_hist = cont.FinalPosition_Detected;

  usr::plt::Flat2DCanvas c;

  c.PlotHist( all_hist,
    usr::plt::Plot2DF( usr::plt::heat ),
    usr::plt::EntryText( "All Photons" ) );

  c.SaveAsPDF( args.MakePDFFile("FinalPosition") );
  c.Pad().Xaxis().SetTitle( "Photon final X [mm]");
  c.Pad().Yaxis().SetTitle( "Photon final Y [mm]");

  usr::plt::Flat2DCanvas c2;

  c2.PlotHist( det_hist,
    usr::plt::Plot2DF( usr::plt::heat ),
    usr::plt::EntryText( "Detected Photons" ) );
  c2.Pad().Xaxis().SetTitle( "Photon final X [mm]");
  c2.Pad().Yaxis().SetTitle( "Photon final Y [mm]");

  c2.SaveAsPDF( args.MakePDFFile("FinalPosition_Detected") );

  return 0;
}
