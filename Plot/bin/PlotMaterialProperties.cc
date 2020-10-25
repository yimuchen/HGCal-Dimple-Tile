#include "HGCalTileSim/Tile/interface/LYSimDetectorConstruction.hh"
#include "HGCalTileSim/Tile/interface/ProjectPath.hh"

#include "UserUtils/PlotUtils/interface/PlotCommon.hpp"
#include "UserUtils/PlotUtils/interface/Simple1DCanvas.hpp"

#include "G4Material.hh"
#include "G4OpticalSurface.hh"

#include <fstream>
#include <sstream>

TGraph MakeGraph( const std::map<double, double> data );
double ScaleGraph( TGraph& );

int
main( int argc, char const* argv[] )
{
  LYSimDetectorConstruction* detector = new LYSimDetectorConstruction();

  std::map<double, double> data;
  auto* list = detector->GetEJ200()->GetMaterialPropertiesTable()
               ->GetProperty( "ABSLENGTH" );

  for( unsigned i = 0; i < list->GetVectorLength(); ++i ){
    const double energy = list->Energy( i );
    const double absval = list->Value( energy );
    data[ 1239.842/( energy/CLHEP::eV ) ] = absval/CLHEP::mm;
  }

  TGraph absgraph( MakeGraph( data ) );
  data.clear();

  list = detector->GetSiPMSurface()->GetMaterialPropertiesTable()
         ->GetProperty( "EFFICIENCY" );

  for( unsigned i = 0; i < list->GetVectorLength(); ++i ){
    const double energy = list->Energy( i );
    const double eff    = list->Value( energy );
    data[ 1239.842/( energy/CLHEP::eV ) ] = eff/CLHEP::mm;
  }

  TGraph effgraph( MakeGraph( data ) );
  data.clear();

  std::fstream enfile( project_base + "/data/PhotonSpectrum.dat", std::ios::in );
  std::string line;

  while( getline( enfile, line ) ){
    std::stringstream ss( line );
    double energy, amount;
    ss >>energy >> amount;
    data[ 1239.842 / ( energy/CLHEP::eV ) ] = amount;
  }

  const double xmin = data.begin()->first / 1.2;
  const double xmax = ( --data.end() )->first * 1.2;
  data[xmin] = data[xmax] = 0;
  TGraph scintgraph( MakeGraph( data ) );

  const double scintscale = ScaleGraph( scintgraph );
  const double absscale   = ScaleGraph( absgraph );
  const double effscale   = ScaleGraph( effgraph );

  usr::plt::Simple1DCanvas c( usr::plt::len::a4textwidth_default(),
                              usr::plt::len::a4textwidth_default(),
                              usr::plt::FontSet( 12 ) );
  c.PlotGraph( scintgraph,
    usr::plt::PlotType( usr::plt::simplefunc ),
    usr::plt::LineColor( usr::plt::col::darkblue ),
    usr::plt::EntryText( "Scintillation spectrum" ) );
  c.PlotGraph( absgraph,
    usr::plt::PlotType( usr::plt::simplefunc ),
    usr::plt::LineColor( usr::plt::col::darkred ),
    usr::plt::EntryText( "Absorption Length" ) );
  c.PlotGraph( effgraph,
    usr::plt::PlotType( usr::plt::simplefunc ),
    usr::plt::LineColor( usr::plt::col::darkgreen ),
    usr::plt::EntryText( "SiPM Efficiency" ) );

  c.Pad().SetYaxisMax( 1.5 );

  for( const auto y : {1.0, 0.75, 0.5, 0.25} ){
    c.Pad().DrawHLine( y,
      usr::plt::LineColor( usr::plt::col::darkgray ),
      usr::plt::LineStyle( usr::plt::sty::lindashed ) );
    c.Pad().SetTextAlign( usr::plt::font::align::bottom_left )
    .WriteAtData(
      xmin, y, usr::fstr( "%.0lfmm", y*absscale ),
      usr::plt::TextColor( usr::plt::col::darkred ) )
    .SetTextAlign( usr::plt::font::align::bottom_right )
    .WriteAtData(
      xmax, y, usr::fstr( "%.0f%%", y*effscale*100 ),
      usr::plt::TextColor( usr::plt::col::darkgreen ) );
  }

  c.Pad().DrawVLine( 425,
    usr::plt::LineColor( usr::plt::col::darkgray ),
    usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  c.Pad().DrawHLine( 3800 / absscale,
    usr::plt::LineColor( usr::plt::col::darkgray ),
    usr::plt::LineStyle( usr::plt::sty::lindotted ) );
  c.Pad().WriteAtData(
    xmin, 3800/absscale, usr::fstr( "%.0lfmm", 3800 ),
    usr::plt::TextColor( usr::plt::col::red ) );


  c.Pad().Yaxis().SetLabelSize( 0 );
  c.Pad().Xaxis().SetTitle( "Optical photon wavelength [nm]" );

  c.Pad().DrawCMSLabel( "Simulation", "HGCal" );

  c.SaveAsPDF( "MaterialsGraph.pdf" );

  return 0;
}

TGraph
MakeGraph( const std::map<double, double> data )
{
  std::vector<double> x;
  std::vector<double> y;

  for( const auto p : data ){
    x.push_back( p.first );
    y.push_back( p.second );
  }

  return TGraph( x.size(), x.data(), y.data() );
}

double
ScaleGraph( TGraph& graph )
{
  const double ans = usr::plt::GetYmax( graph );

  for( int i = 0; i < graph.GetN(); ++i ){
    graph.GetY()[i] /= ans;
  }

  return ans;
}
