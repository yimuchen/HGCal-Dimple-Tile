#include "UserUtils/Common/interface/ArgumentExtender.hpp"
#include "UserUtils/Common/interface/Format.hpp"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "TFile.h"

int
main( int argc, char** argv )
{
  usr::po::options_description desc(
    "Generating TGraphs For a certain selection criteria" );
  desc.add_options()
    ( "dimplerad,r", usr::po::multivalue<double>(), "Dimple radius [mm]" )
    ( "dimpleind,d", usr::po::multivalue<double>(), "Dimple indent [mm]" )
    ( "tilewidth,L", usr::po::multivalue<double>(), "Tile width [mm]"    )
    ( "absmult,a",   usr::po::multivalue<double>(),
    "Multple of inbuilt absorption length" )
    ( "wrapreflect,m", usr::po::multivalue<double>(),
    "Wrap reflectivity" )
    ( "sipmwidth,W", usr::po::value<double>(), "SiPM Width [mm]" )
    ( "sipmstand,S",   usr::po::multivalue<double>(), "SiPM stand height [mm]" )
    ( "inputfile,f", usr::po::value<std::string>(),
    "File containing the generated files" )
    ( "output,o", usr::po::defvalue<std::string>(
    ( usr::resultpath( "HGCalTileSim", "Plot" )/"LYvX.pdf" ).string() ),
    "Output plot file" )
  ;

  usr::ArgumentExtender args;
  args.AddOptions( desc );
  args.ParseOptions( argc, argv );

  const std::vector<double> rlist = args.ArgList<double>( "dimplerad"   );
  const std::vector<double> dlist = args.ArgList<double>( "dimpleind"   );
  const std::vector<double> Llist = args.ArgList<double>( "tilewidth"   );
  const std::vector<double> alist = args.ArgList<double>( "absmult"     );
  const std::vector<double> wlist = args.ArgList<double>( "wrapreflect" );
  const std::vector<double> Slist = args.ArgList<double>( "sipmstand" );
  const double sipmwidth          = args.Arg<double>( "sipmwidth" );

  const std::vector<double>& varlist = rlist.size() > 1 ? rlist :
                                       dlist.size() > 1 ? dlist :
                                       Llist.size() > 1 ? Llist :
                                       alist.size() > 1 ? alist :
                                       wlist.size() > 1 ? wlist :
                                       Slist;

  // Dummy for insersion.
  LYSimFormat fmt;
  fmt.beam_width = 1.5;
  std::vector<LYSimFormat> fmtlist;

  for( const auto r : rlist ){
    for( const auto d : dlist ){
      for( const auto L : Llist ){
        for( const auto a : alist ){
          for( const auto w : wlist ){
            for( const auto S : Slist ){
              fmt.dimple_radius = r;
              fmt.dimple_indent = d;
              fmt.tile_width    = L;
              fmt.abs_mult      = a;
              fmt.wrap_reflect  = w;
              fmt.sipm_width    = sipmwidth;
              fmt.sipm_stand    = S;
              fmtlist.push_back( fmt );
            }
          }
        }
      }
    }
  }

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

  const double xmax = (*std::max_element(Llist.begin(), Llist.end())) / 2;

  usr::plt::Simple1DCanvas c;
  TH1D dummyhist( "", "", 1, -xmax - 5, xmax + 5 );
  c.PlotHist( dummyhist, usr::plt::PlotType( usr::plt::hist ) );

  unsigned idx = 0;
  double width = 0;


  for( const auto fmt : fmtlist ){
    const std::string graphname = LYGraphName( fmt );
    std::cout << graphname << std::endl;
    TGraph* graph = (TGraph*)( file.Get( graphname.c_str() ) );
    if( !graph ){ continue; }

    const int precision = &varlist == &rlist ? 2 :
                          &varlist == &dlist ? 2 :
                          &varlist == &Llist ? 0 :
                          &varlist == &alist ? 0 :
                          &varlist == &wlist ? 1 :
                          &varlist == &Slist ? 2 :
                          0;

    const std::string var = &varlist == &rlist ? "Rad."  :
                            &varlist == &dlist ? "Ind."  :
                            &varlist == &Llist ? "Tile"  :
                            &varlist == &alist ? "Abs."  :
                            &varlist == &wlist ? "Refl." :
                            &varlist == &Slist ? "Stand" :
                            "";
    const std::string unit = &varlist == &rlist ? "[mm]" :
                             &varlist == &dlist ? "[mm]" :
                             &varlist == &Llist ? "[cm]" :
                             &varlist == &alist ? "[cm]" :
                             &varlist == &wlist ? "%" :
                             &varlist == &Slist ? "[mm]" :
                             "";
    const double val = &varlist == &rlist ? fmt.dimple_radius :
                       &varlist == &dlist ? fmt.dimple_indent :
                       &varlist == &Llist ? fmt.tile_width / 10 :
                       &varlist == &alist ? fmt.abs_mult *380 :
                       &varlist == &wlist ? fmt.wrap_reflect *100 :
                       &varlist == &Slist ? fmt.sipm_stand :
                       0;

    const std::string entry = usr::fstr( "%s=%s%s", var
                                       , usr::fmt::base::decimal( val, precision )
                                       , unit );

    c.PlotGraph( graph,
      usr::plt::PlotType( usr::plt::fittedfunc ),
      usr::plt::TrackY( usr::plt::TrackY::max ),
      usr::plt::LineColor( colorlist[idx] ),
      usr::plt::FillColor( filllist[idx] ),
      usr::plt::FillStyle( usr::plt::sty::fillsolid ),
      usr::plt::EntryText( entry ) );
    width = graph->GetErrorX( 0 );
    ++idx;
    std::cout << "Plot" << std::endl;
  }

  if( &varlist != &rlist ){
    c.Pad().DrawVLine( rlist.front(),
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
    c.Pad().DrawVLine( -rlist.front(),
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  }

  if( &varlist != &Llist ){
    c.Pad().DrawVLine( Llist.front(),
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
    c.Pad().DrawVLine( -Llist.front(),
      usr::plt::LineColor( usr::plt::col::gray ),
      usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  }

  c.Pad().SetDataMax( c.Pad().GetDataMax() * 1.3 );
  c.Pad().Xaxis().SetTitle( "Beam Center X [mm]" );
  c.Pad().Yaxis().SetTitle( "Detected Photons" );

  c.DrawLuminosity( usr::fstr( "Trigger Width: %.1lf[mm], SiPM Width: %.1lf[mm]"
                             , width *2
                             , sipmwidth ) );
  c.DrawCMSLabel( "Simulation", "HGCal" );
  if( &varlist != &rlist ){
    c.Pad().WriteLine( usr::fstr( "Radius=%.1lf[mm]", rlist.front() ) );
  }
  if( &varlist != &dlist ){
    c.Pad().WriteLine( usr::fstr( "Indent=%.1lf[mm]", dlist.front() ) );
  }
  if( &varlist != &alist ){
    c.Pad().WriteLine( usr::fstr( "Abs. =%.1lf[cm]", alist.front()*380 ) );
  }
  if( &varlist != &wlist ){
    c.Pad().WriteLine( usr::fstr( "Refl =%.1lf[%%]", wlist.front()*100 ) );
  }
  if( &varlist != &Slist ){
    c.Pad().WriteLine( usr::fstr( "Stand =%.1lf[mm]", Slist.front() ) );
  }


  c.SaveAsPDF( args.Arg<std::string>( "output" ) );

}
