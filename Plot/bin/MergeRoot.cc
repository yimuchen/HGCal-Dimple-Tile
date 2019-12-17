#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"
#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

int main( int argc, char* argv[] )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "inputfile,f", usr::po::multivalue<std::string>(),
    "Input root files" )
    ( "output,o", usr::po::defvalue<std::string>(
    ( usr::resultpath( "HGCalTileSim", "Plot" )/"tree.root" ).string() ),
    "Output root file" )
  ;
  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  LYSimFormat format;
  LYSimFormat oldformat;
  LYSimRunFormat runformat;
  LYSimRunFormat oldrunformat;
  TTree newtree( "LYSim", "LYSim" );
  TTree newruntree( "LYSimRun", "LYSimRun" );

  format.AddToTree( &newtree );
  runformat.AddToTree( &newruntree );

  const std::string outputfile = args.Arg( "output" );
  const std::vector<std::string> filelist
    = args.ArgList<std::string>( "inputfile" );

  for( const auto& file : filelist ){
    TChain oldtree( "LYSim", "LYSim" );
    TChain oldruntree( "LYSimRun", "LYSimRun" );

    oldtree.Add( file.c_str() );
    oldruntree.Add( file.c_str() );
    oldformat.LoadBranches( &oldtree );
    oldrunformat.LoadBranches( &oldruntree );

    for( int runid = 0; runid < oldruntree.GetEntries(); ++runid ){
      oldruntree.GetEntry( runid );

      runformat             = oldrunformat;
      runformat.start_event = newtree.GetEntries();

      for( unsigned long long evt = oldrunformat.start_event;
           evt < oldrunformat.end_event; ++evt ){
        oldtree.GetEntry( evt );
        format = oldformat;
        newtree.Fill();
      }

      runformat.end_event = newtree.GetEntries();
      newruntree.Fill();
    }
  }

  TFile* newfile = TFile::Open( outputfile.c_str(), "RECREATE" );
  newruntree.Write();
  newtree.Write();
  newfile->Close();
}
