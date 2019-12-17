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

  std::map<LYSimRunFormat, LYvXGraphContainer, RunFormatCompare>
  container( LYvXGraphCompare );

  for( const auto& file : filelist ){
    LYSimFormat format;
    LYSimRunFormat runformat;

    TChain tree( "LYSim", "LYSim" );
    TChain runtree( "LYSimRun", "LYSimRun" );
    tree.Add( file.c_str() );
    runtree.Add( file.c_str() );
    format.LoadBranches( &tree );
    runformat.LoadBranches( &runtree );

    for( int runid = 0; runid < runtree.GetEntries(); ++runid ){
      runtree.GetEntry( runid );

      for( unsigned evt = runformat.start_event;
           evt < runformat.end_event; ++evt ){
        tree.GetEntry( evt );

        container[ runformat ].Fill( runformat, format );
      }
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

    outputtree.Fill();
  }

  TFile* outputfile = TFile::Open( output.c_str(), "RECREATE" );
  outputtree.Write();
  outputfile->Close();

  return 0;
}
