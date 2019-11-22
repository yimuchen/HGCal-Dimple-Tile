#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"
#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Maths.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"

#include "TChain.h"
#include "TFile.h"
#include "TGraphErrors.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "inputfiles,f", usr::po::multivalue<std::string>(),
    "List of input root files" )
    ( "output,o", usr::po::defvalue<std::string>(
    ( usr::resultpath( "HGCalTileSim", "Plot" )/"LYvX.root" ).string() ),
    "Output root file" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  const std::string output = args.Arg<std::string>( "output" );
  const std::vector<std::string> filelist
    = args.ArgList<std::string>( "inputfiles" );

  // Loading input files
  TChain tree( "LYSim", "LYSim" );

  for( const auto file : filelist ){
    tree.Add( file.c_str() );
  }

  typedef std::map<double, std::vector<double> > SinglePointResult;

  typedef std::map<LYSimFormat, SinglePointResult, LYFormatCompare> ResultsMap;
  ResultsMap nphotons( &LYGraphCompare );
  ResultsMap genphotons( &LYGraphCompare );

  LYSimFormat format;
  format.LoadBranches( &tree );

  // Creating the List of results in memory
  for( int i = 0; i < tree.GetEntries(); ++i ){
    tree.GetEntry( i );

    if( i % 97 == 0 || tree.GetEntries() % (i+1) == 0 )
    std::cout << usr::fstr( "\rReading Entry: %10d [%.0lf%%]..."
                          , i+1, double(i+1)/tree.GetEntries()*100 )
              << std::flush;

    nphotons[format][ format.beam_center_x].push_back( format.nphotons );
    nphotons[format][-format.beam_center_x].push_back( format.nphotons );
    genphotons[format][ format.beam_center_x].push_back( format.genphotons );
    genphotons[format][-format.beam_center_x].push_back( format.genphotons );
  }
  std::cout << "Done" << std::endl;

  // Creating a single TGraph for each setup
  TFile* file = TFile::Open( output.c_str(), "RECREATE" );

  for( const auto p : nphotons ){
    const std::string graphname = LYGraphName( p.first );
    std::vector<double> x_list;
    std::vector<double> v_list;
    std::vector<double> unc_list;
    std::vector<double> w_list;

    for( const auto v : p.second ){
      x_list.push_back( v.first );
      v_list.push_back( usr::Mean( v.second ) );
      unc_list.push_back( usr::StdDev( v.second )/sqrt( v.second.size() ) );
      w_list.push_back( p.first.beam_width );
    }

    TGraphErrors g( x_list.size(),
                    x_list.data(), v_list.data(),
                    w_list.data(), unc_list.data() );
    g.SetName( graphname.c_str() );
    g.Write();
  }

  file->Close();

  // Printing out all unique values of parameter for easier plotting commands.
  std::map<double, std::vector<double> > rlist;
  std::map<double, std::vector<double> > dlist;
  std::map<double, std::vector<double> > llist ;
  std::map<double, std::vector<double> > alist;
  std::map<double, std::vector<double> > wlist;
  std::map<double, std::vector<double> > Slist;

  for( const auto& p : nphotons ){
    const auto& fmt = p.first;

    rlist[fmt.sipm_width].push_back( fmt.dimple_radius );
    dlist[fmt.sipm_width].push_back( fmt.dimple_indent );
    llist[fmt.sipm_width].push_back( fmt.tile_width    );
    alist[fmt.sipm_width].push_back( fmt.abs_mult      );
    wlist[fmt.sipm_width].push_back( fmt.wrap_reflect  );
    Slist[fmt.sipm_width].push_back( fmt.sipm_stand    );
  }

  for( const auto& p : rlist ){
    const auto& sipm_width = p.first;

    auto print_vec = []( const std::string& head,
                         const std::vector<double>& vec ){
                       std::cout << head << std::flush;

                       for( const auto x : vec ){
                         std::cout << " " << x;
                       }

                       std::cout << std::endl;
                     };


    std::cout << "Parameters for SiPM Width "
              << sipm_width << " [mm]:" << std::endl;
    print_vec( "Dimple Radius :", usr::RemoveDuplicate( rlist[sipm_width] ) );
    print_vec( "Dimple Indent :", usr::RemoveDuplicate( dlist[sipm_width] ) );
    print_vec( "Tile Size     :", usr::RemoveDuplicate( llist[sipm_width] ) );
    print_vec( "Abs Multiplier:", usr::RemoveDuplicate( alist[sipm_width] ) );
    print_vec( "Wrap reflect. :", usr::RemoveDuplicate( wlist[sipm_width] ) );
    print_vec( "SiPM Stand    :", usr::RemoveDuplicate( Slist[sipm_width] ) );

    std::cout << "\n\n" << std::endl;

  }

  return 0;
}
