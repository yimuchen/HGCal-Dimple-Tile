#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"
#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Maths.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"

#include "TChain.h"
#include "TFile.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "inputfiles,f", usr::po::multivalue<std::string>(),
    "List of input root files" )
    ( "pdf,o", usr::po::defvalue<std::string>(
    ( usr::resultpath( "HGCalTileSim", "Plot" )/"Track.pdf" ).string() ),
    "Output pdf file" )
  ;
  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  const std::vector<std::string> filelist
    = args.ArgList<std::string>( "inputfiles" );
  const std::string pdf = args.Arg<std::string>( "pdf" );

  typedef std::map<double, TH1D*> HistMap;

  HistMap lengthlist;
  HistMap bouncelist;
  HistMap all_lengthlist;
  HistMap all_bouncelist;

  for( const auto file : filelist ){
    std::cout << file << std::endl;
    TFile* infile = TFile::Open( file.c_str(), "READ" );
    // Loading input files
    TChain tree( "LYSim", "LYSim" );
    tree.Add( file.c_str() );
    LYSimFormat format;
    format.LoadBranches( &tree );

    tree.GetEntry( 0 );

    const double r = format.wrap_reflect;

    TH1D* length     = (TH1D*)( infile->Get( "PhotonTrack" ) );
    TH1D* bounce     = (TH1D*)( infile->Get( "PhotonBounce" ) );
    TH1D* all_length = (TH1D*)( infile->Get( "AllPhotonTrack" ) );
    TH1D* all_bounce = (TH1D*)( infile->Get( "AllPhotonBounce" ) );
    length->SetDirectory( nullptr );
    bounce->SetDirectory( nullptr );
    all_length->SetDirectory( nullptr );
    all_bounce->SetDirectory( nullptr );

    lengthlist[r]     = length;
    bouncelist[r]     = bounce;
    all_lengthlist[r] = all_length;
    all_bouncelist[r] = all_bounce;

    const std::string title = usr::fstr( "Tile:%.0lf[cm], R:%0.1lf%%"
                                       , format.tile_width
                                       , format.wrap_reflect*100 );

    length->SetTitle( title.c_str() );
    bounce->SetTitle( title.c_str() );
    all_bounce->SetTitle( title.c_str() );
    all_length->SetTitle( title.c_str() );

    length->Rebin( 5 );
    all_length->Rebin( 5 );

    infile->Close();
  }

  const std::vector<int> colorlist = {
    usr::plt::col::blue,
    usr::plt::col::darkred,
    usr::plt::col::green,
    usr::plt::col::darkorange
  };


  auto PlotComp
    = [&]( const HistMap& histlist,
           const std::string& xaxis,
           const std::string& tag,
           const std::string& postfix ){
        {
          usr::plt::Simple1DCanvas c;
          usr::plt::Simple1DCanvas c2;
          unsigned i = 0;

          for( const auto p : histlist ){
            TH1D* ch = (TH1D*)p.second->GetCumulative( kFALSE );
            ch->Scale( 1/ch->GetBinContent(1) );
            const std::string title = p.second->GetTitle();
            c.PlotHist( p.second,
              usr::plt::PlotType( usr::plt::hist ),
              usr::plt::LineColor( colorlist[i] ),
              usr::plt::EntryText( title ),
              usr::plt::TrackY( usr::plt::TrackY::both )
              );
            c2.PlotHist( ch,
              usr::plt::PlotType( usr::plt::hist ),
              usr::plt::LineColor( colorlist[i] ),
              usr::plt::EntryText( title ),
              usr::plt::TrackY( usr::plt::TrackY::both )
              );
            ++i;
          }

          c.Pad().SetLogy( kTRUE );
          c.Pad().Xaxis().SetTitle( xaxis.c_str() );
          c.Pad().Yaxis().SetTitle( "Number of photons" );
          c.Pad().DrawCMSLabel( "Simulation", "HGCal" );
          c.Pad().DrawLuminosity( tag );

          c2.Pad().Xaxis().SetTitle( xaxis.c_str() );
          c2.Pad().Yaxis().SetTitle( "Remaining photon fractions" );
          c2.Pad().DrawCMSLabel( "Simulation", "HGCal" );
          c2.Pad().DrawLuminosity( tag );

          std::string filename = args.Arg<std::string>( "pdf" );
          filename.insert( filename.length()-4, postfix );
          c.SaveAsPDF( filename );

          filename.insert( filename.length()-4, std::string( "_accum" ) );
          c2.SaveAsPDF( filename );
        }
      };

  PlotComp( lengthlist
          , "Photon Path Length [cm]", "Detected Photons",  "_length" );
  PlotComp( bouncelist
          , "Number of reflections", "Detected Photons", "_bounce" );
  PlotComp( all_lengthlist
          , "Photon Path Length [cm]", "All Photons",  "_all_length" );
  PlotComp( all_bouncelist
          , "Number of reflections", "all Photons", "_all_bounce" );

}
