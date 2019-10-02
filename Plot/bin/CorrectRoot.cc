#include "UserUtils/Common/interface/ArgumentExtender.hpp"

#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include "TFile.h"
#include "TChain.h"

int main(int argc, char* argv[])
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "inputfiles,f", usr::po::multivalue<std::string>(),
    "List of input root files" )
    ( "dimplerad,r", usr::po::value<double>(), "Force Dimple radius [mm]" )
    ( "dimpleind,d", usr::po::value<double>(), "Force Dimple indent [mm]" )
    ( "absmult,a",   usr::po::value<double>(),
    "Multple of inbuilt absorption length" )
    ( "wrapreflect,m", usr::po::value<double>(),
    "Wrap reflectivity" )
    ( "output,o", usr::po::defvalue<std::string>( "test.root" ),
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
  TTree newtree("LYSim", "LYSim" );

  for( const auto file : filelist ){
    tree.Add( file.c_str() );
  }

  LYSimFormat format;
  format.LoadBranches( &tree );
  format.AddToTree( &newtree );

  for( int i = 0; i < tree.GetEntries(); ++i ){
    tree.GetEntry(i);
    std::cout << "\rEntry: " << i+1 << std::flush;

    if( args.CheckArg("dimplerad") ){
      format.dimple_radius = args.Arg<double>("dimplerad");
    }
    if( args.CheckArg("dimpleind") ){
      format.dimple_indent = args.Arg<double>("dimpleind");
    }
    if( args.CheckArg("absmult") ){
      format.abs_mult = args.Arg<double>("absmult");
    }
    if( args.CheckArg("wrapreflect") ){
      format.wrap_reflect = args.Arg<double>("wrapreflect");
    }

    newtree.Fill();
  }

  TFile newfile( output.c_str(), "RECREATE" );
  newtree.Write();

  return 0;
}
