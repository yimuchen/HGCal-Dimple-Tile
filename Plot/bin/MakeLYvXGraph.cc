#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Maths.hpp"
#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"

#include "TChain.h"
#include "TFile.h"
#include "TGraphErrors.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "beamy,y", usr::po::defvalue<double>( 0 ), "Fixed value of y" )
    ( "dimplerad,r", usr::po::multivalue<double>(),
    "List of different dimple radius, different dimple radii will be saved to "
    "different plot" )
    ( "dimpleind,d", usr::po::multivalue<double>(),
    "List of different dimple indents, different indent values will be saved "
    "to different plot" )
    ( "inputfiles,f", usr::po::multivalue<std::string>(),
    "List of input root files" )
    ( "output,o", usr::po::defvalue<std::string>(
    (usr::resultpath( "HGCalTileSim", "Plot" )/"LYvX.root").string() ),
    "Output root file" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  const std::string output = args.Arg<std::string>( "output" );
  const double y           = args.Arg<double>( "beamy" );
  const std::vector<double> rlist
    = args.ArgList<double>( "dimplerad" );
  const std::vector<double> dlist
    = args.ArgList<double>( "dimpleind" );
  const std::vector<std::string> filelist
    = args.ArgList<std::string >( "inputfiles" );

  // Loading input files
  TChain tree( "LYSim", "LYSim" );

  for( const auto file : filelist ){
    tree.Add( file.c_str() );
  }

  typedef std::map<double, std::vector<double> > SinglePointResult;
  typedef std::map<double, SinglePointResult>    R_Result;
  typedef std::map<double, R_Result>             Results;
  Results results;

  LYSimFormat format;
  format.LoadBranches( &tree );
  double width = 0 ;

  // Creating the List of results in memory
  for( int i = 0; i < tree.GetEntries(); ++i ){
    tree.GetEntry( i );
    std::cout << "\rEntry: " << i << std::flush;

    if( format.beam_center_y != y ){continue;}

    for( const auto r : rlist ){
      if( format.dimple_radius != r ){ continue; }

      for( const auto d : dlist ){
        if( format.dimple_indent != d ){ continue; }
        results[r][d][format.beam_center_x].push_back( format.nphotons );
        results[r][d][-format.beam_center_x].push_back( format.nphotons );
        if( i != 0 && format.beam_width != width ){
          std::cout << "Warning! Different beam width detected! "
                       "Make sure this is what you want!" << std::endl;
        }
        width = format.beam_width;
      }
    }
  }

  // Creating a single TGraph for each r/d results
  TFile* file = TFile::Open( output.c_str(), "RECREATE" );

  for( const auto r : rlist ){
    for( const auto d : dlist ){
      const std::string graphname = usr::fstr( "R%.1lf_D%.1lf", r, d );
      std::vector<double> x_list;
      std::vector<double> v_list;
      std::vector<double> unc_list;
      std::vector<double> w_list;

      for( const auto p : results[r][d] ){
        x_list.push_back( p.first );
        v_list.push_back( usr::Mean( p.second ) );
        unc_list.push_back( usr::StdDev( p.second )/sqrt(p.second.size()) );
        w_list.push_back( width );
      }

      TGraphErrors g( x_list.size(),
                      x_list.data(), v_list.data(),
                      w_list.data(), unc_list.data() );
      g.SetName( graphname.c_str() );
      g.Write();
    }
  }

  file->Close();

  return 0;
}
