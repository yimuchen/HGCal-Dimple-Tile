#include "HGCalTileSim/Plot/interface/LYvX_Common.hpp"

#include "TGraphErrors.h"
#include "TH1D.h"

TGraph*
MakeGraph( const LYSimRunFormat&     fmt,
           const LYvXGraphContainer& cont,
           const std::string&        name )
{
  const double width      = fmt.beam_w;
  const TProfile* profile = name == "LYvX_Unbinned" ? cont.LYvX_Unbinned  :
                            name == "Eff" ? cont.Eff :
                            name == "Eff_Unbinned" ? cont.Eff_Unbinned :
                            cont.LYvX;

  std::vector<double> x_list;
  std::vector<double> y_list;
  std::vector<double> yerr_list;
  std::vector<double> xerr_list;

  for( int i = 1; i <= profile->GetNcells(); ++i ){
    if( profile->GetBinContent( i ) != 0 ){
      xerr_list.push_back( width );
      x_list.push_back( profile->GetBinCenter( i ) );
      y_list.push_back( profile->GetBinContent( i ) );
      yerr_list.push_back( profile->GetBinError( i ) );
    }
  }

  return new TGraphErrors( x_list.size()
                         , x_list.data(), y_list.data()
                         , xerr_list.data(), yerr_list.data() );
}

TH1D*
MakeDetXHist( const LYSimRunFormat&     fmt,
              const LYvXGraphContainer& cont )
{
  TH1D* ans = (TH1D*)(cont.FinalPositionX_Detected->Clone());
  ans->Rebin(2);
  ans->Scale(100.0/ans->Integral());

  return ans;
}


TH1D*
MakePhotonHist( const LYSimRunFormat&     fmt,
                const LYvXGraphContainer& cont,
                const std::string&        name
                )
{
  TH1D* orig = name == "OpticalLength" ? cont.OpticalLength :
               name == "OpticalLength_Detected" ? cont.OpticalLength_Detected :
               name == "NumWrapReflection" ? cont.NumWrapReflection :
               name == "NumWrapReflection_Detected" ? cont.NumWrapReflection_Detected :
               name == "NumPCBReflection" ? cont.NumPCBReflection :
               cont.NumPCBReflection_Detected;

  TH1D* newhist = name.find( "OpticalLength" ) != std::string::npos ?
                  (TH1D*)( orig->Rebin( 5 ) ) : (TH1D*)orig->Clone();
  newhist->Scale( 1/orig->Integral() );
  return newhist;
}

TH1D*
MakePhotonAccum( const LYSimRunFormat&     fmt,
                 const LYvXGraphContainer& cont,
                 const std::string&        name  )
{
  TH1D* orig = name == "OpticalLength" ? cont.OpticalLength :
               name == "OpticalLength_Detected" ? cont.OpticalLength_Detected :
               name == "NumWrapReflection" ? cont.NumWrapReflection :
               name == "NumWrapReflection_Detected" ? cont.NumWrapReflection_Detected :
               name == "NumPCBReflection" ? cont.NumPCBReflection :
               cont.NumPCBReflection_Detected;

  TH1D* newhist = (TH1D*)( orig->GetCumulative( kFALSE ) );
  newhist->Scale( 1/newhist->GetBinContent( 1 ) );

  return newhist;
}
