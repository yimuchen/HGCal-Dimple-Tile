#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"
#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Maths.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"
#include "UserUtils/Common/interface/STLUtils/VectorUtils.hpp"

#include "TChain.h"
#include "TError.h"
#include "TFile.h"
#include "TGraphErrors.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "inputfile,f", usr::po::multivalue<std::string>(),
    "Input root file" )
    ( "output,o", usr::po::defvalue<std::string>(
    ( usr::resultpath( "HGCalTileSim", "Plot" )/"LYvX.root" ).string() ),
    "Output root file" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  const std::vector<std::string> filelist
    = args.ArgList<std::string>( "inputfile" );
  const std::string output = args.Arg<std::string>( "output" );

  std::map<LYSimRunFormat, LYvXGraphContainer, LYvXGraphContainer> container;

  TChain tree( "LYSim", "LYSim" );
  TChain runtree( "LYSimRun", "LYSimRun" );
  LYSimFormat format;
  LYSimRunFormat runformat;

  for( const auto& file : filelist ){
    tree.Add( file.c_str() );
    runtree.Add( file.c_str() );
  }

  format.LoadBranches( &tree );
  runformat.LoadBranches( &runtree );

  std::map<unsigned, unsigned> hash_map;

  for( int entry = 0; entry < runtree.GetEntries(); ++entry ){
    runtree.GetEntry( entry );
    hash_map[runformat.run_hash]  = entry;
  }


  for( int entry = 0; entry < tree.GetEntries(); ++entry ){
    if( ( entry+1 ) % 100 == 0 ){
      std::cout <<
        usr::fstr( "\rEvent ID [%10d/%10d] | %10u | %10u | %5u"
                 , entry+1, tree.GetEntries()
                 , format.run_hash
                 , format.event_hash
                 , container.size() )
                << std::flush;
    }
    tree.GetEntry( entry );
    if( hash_map.count(format.run_hash) ){
      runtree.GetEntry( hash_map.at(format.run_hash) );
       container[runformat].Fill( runformat, format );
    } else {
    }
  }


  // Writing collected graphs to File
  LYSimRunFormat outputformat;
  LYvXGraphContainer graphcontainer;

  TTree outputtree( "LYSimRun", "LYSimRun" );
  outputformat.AddToTree( &outputtree );
  graphcontainer.AddToTree( &outputtree );

  for( const auto p : container ){
    outputformat   = p.first;
    graphcontainer = p.second;

    outputformat.beam_x = 0;// Forcing to be zero
    outputformat.UpdateHash();

    outputtree.Fill();
  }

  TFile* outputfile = TFile::Open( output.c_str(), "RECREATE" );
  outputtree.Write();
  outputfile->Close();

  return 0;
}
