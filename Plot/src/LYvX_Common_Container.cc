#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "UserUtils/Common/interface/STLUtils/StringUtils.hpp"

#include "G4PhysicalConstants.hh"

bool
LYvXGraphContainer::operator()( const LYSimRunFormat& x,
                                const LYSimRunFormat& y ) const
{
#define COMPARE_MEMBER( MEMBER ) \
  if( x.MEMBER != y.MEMBER ){ return x.MEMBER < y.MEMBER;  }

  COMPARE_MEMBER( tile_x );
  COMPARE_MEMBER( tile_y );
  COMPARE_MEMBER( tile_z );

  COMPARE_MEMBER( sipm_width );
  COMPARE_MEMBER( sipm_rim );
  COMPARE_MEMBER( sipm_stand );

  COMPARE_MEMBER( dimple_type );
  COMPARE_MEMBER( dimple_rad );
  COMPARE_MEMBER( dimple_ind );

  COMPARE_MEMBER( abs_mult );
  COMPARE_MEMBER( wrap_ref );
  COMPARE_MEMBER( pcb_rad );
  COMPARE_MEMBER( pcb_ref );
  COMPARE_MEMBER( tile_alpha );
  COMPARE_MEMBER( dimple_alpha );

  COMPARE_MEMBER( beam_y );
  COMPARE_MEMBER( beam_w );

  return x.beam_y < y.beam_y;
#undef COMPARE_MEMBER
}

#define HIST1D( NBINS, XMIN, XMAX ) \
  usr::RandomString( 10 ).c_str(), "", NBINS, XMIN, XMAX
#define HIST2D( XBINS, XMIN, XMAX, YBINS, YMIN, YMAX ) \
  usr::RandomString( 10 ).c_str(), "", XBINS, XMIN, XMAX, YBINS, YMIN, YMAX

LYvXGraphContainer::LYvXGraphContainer() :
  LYvX( new TProfile( HIST1D( 1001, -50.05, +50.05 ) ) ),
  LYvX_Unbinned( new TProfile( HIST1D( 1001, -50.05, +50.05 ) ) ),
  Eff( new TProfile( HIST1D( 1001, -50.05, +50.05 ) ) ),
  Eff_Unbinned( new TProfile( HIST1D( 1001, -50.5, +50.5 ) ) ),
  OpticalLength(  new TH1D( HIST1D( 600, 0, 600 ) ) ),
  OpticalLength_Detected(  new TH1D( HIST1D( 600, 0, 600 ) ) ),
  NumWrapReflection(  new TH1D(  HIST1D( 100, 0, 200 ) ) ),
  NumWrapReflection_Detected(  new TH1D( HIST1D( 100, 0, 200 ) ) ),
  NumPCBReflection(  new TH1D( HIST1D( 25, 0, 25 ) ) ),
  NumPCBReflection_Detected(  new TH1D( HIST1D( 25, 0, 25 ) ) ),
  FinalPosition( new TH2D( HIST2D( 100, -50.0, +50.0, 100, -50.0, +50.0 ) ) ),
  FinalPosition_Detected( new TH2D( HIST2D( 70, -3.5, +3.5, 70, -3.5, 3.5 ) ) ),
  FinalPositionX_Detected( new TH1D( HIST1D( 60, -1.5, +1.5 ) ) )
{
}
#undef HIST1D
#undef HIST2D

void
LYvXGraphContainer::Fill( const LYSimRunFormat& run,
                          const LYSimFormat&    evt )
{
  for( unsigned i = 0; i < evt.savedphotons; ++i ){
    const double opt = (double)evt.OpticalLength[i]
                       / ( CLHEP::cm / LYSimFormat::opt_length_unit );
    const double x = (double)evt.EndX[i] * LYSimFormat::end_pos_unit;
    const double y = (double)evt.EndY[i] * LYSimFormat::end_pos_unit;
    OpticalLength->Fill( opt );
    NumWrapReflection->Fill( evt.NumWrapReflection[i] );
    NumPCBReflection->Fill( evt.NumPCBReflection[i] );
    FinalPosition->Fill( x, y );

    if( evt.IsDetected[i] ){
      OpticalLength_Detected->Fill( opt );
      NumWrapReflection_Detected->Fill( evt.NumWrapReflection[i] );
      NumPCBReflection_Detected->Fill( evt.NumPCBReflection[i] );
      FinalPosition_Detected->Fill( x, y );
      FinalPositionX_Detected->Fill( x );
    }
  }


  LYvX->Fill( run.beam_x, evt.nphotons );
  LYvX_Unbinned->Fill( evt.beam_x, evt.nphotons );
  LYvX->Fill( -run.beam_x, evt.nphotons );
  LYvX_Unbinned->Fill( -evt.beam_x, evt.nphotons );

  Eff->Fill( run.beam_x, (double)evt.nphotons/(double)evt.genphotons );
  Eff_Unbinned->Fill( evt.beam_x, (double)evt.nphotons/(double)evt.genphotons );
  Eff->Fill( -run.beam_x, (double)evt.nphotons/(double)evt.genphotons );
  Eff_Unbinned->Fill( -evt.beam_x, (double)evt.nphotons/(double)evt.genphotons );
}

void
LYvXGraphContainer::AddToTree( TTree* tree )
{
  tree->Branch( "LYvX",                       "TProfile", &LYvX );
  tree->Branch( "LYvX_Unbinned",              "TProfile", &LYvX_Unbinned );
  tree->Branch( "Eff",                        "TProfile", &Eff );
  tree->Branch( "Eff_Unbinned",               "TProfile", &Eff_Unbinned );
  tree->Branch( "OpticalLength",              "TH1D",     &OpticalLength );
  tree->Branch( "OpticalLength_Detected",     "TH1D",     &OpticalLength_Detected );
  tree->Branch( "NumWrapReflection",          "TH1D",     &NumWrapReflection );
  tree->Branch( "NumWrapReflection_Detected", "TH1D",     &NumWrapReflection_Detected );
  tree->Branch( "NumPCBReflection",           "TH1D",     &NumPCBReflection );
  tree->Branch( "NumPCBReflection_Detected",  "TH1D",     &NumPCBReflection_Detected );
  tree->Branch( "FinalPosition",              "TH2D",     &FinalPosition );
  tree->Branch( "FinalPosition_Detected",     "TH2D",     &FinalPosition_Detected );
  tree->Branch( "FinalPositionX_Detected",    "TH1D",     &FinalPositionX_Detected );
}

void
LYvXGraphContainer::LoadBranches( TTree* tree )
{
  tree->SetBranchAddress( "LYvX",                       &LYvX );
  tree->SetBranchAddress( "LYvX_Unbinned",              &LYvX_Unbinned );
  tree->SetBranchAddress( "Eff",                        &Eff );
  tree->SetBranchAddress( "Eff_Unbinned",               &Eff_Unbinned );
  tree->SetBranchAddress( "OpticalLength",              &OpticalLength );
  tree->SetBranchAddress( "OpticalLength_Detected",     &OpticalLength_Detected );
  tree->SetBranchAddress( "NumWrapReflection",          &NumWrapReflection );
  tree->SetBranchAddress( "NumWrapReflection_Detected", &NumWrapReflection_Detected );
  tree->SetBranchAddress( "NumPCBReflection",           &NumPCBReflection );
  tree->SetBranchAddress( "NumPCBReflection_Detected",  &NumPCBReflection_Detected );
  tree->SetBranchAddress( "FinalPosition",              &FinalPosition );
  tree->SetBranchAddress( "FinalPosition_Detected",     &FinalPosition_Detected );
  tree->SetBranchAddress( "FinalPositionX_Detected",    &FinalPositionX_Detected );
}
