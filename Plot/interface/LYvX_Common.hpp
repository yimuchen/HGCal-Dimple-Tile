#ifndef HGCALTILESIM_PLOT_LYVX_COMMON_HPP
#define HGCALTILESIM_PLOT_LYVX_COMMON_HPP

#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"
// #include "UserUtils/Common/interface/ArgumentExtender.hpp"

#include <boost/program_options.hpp>
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TGraph.h"

class LYvXGraphContainer
{
public:
  LYvXGraphContainer();

  // For writing
  TProfile* LYvX;
  TProfile* LYvX_Unbinned;
  TProfile* Eff;
  TProfile* Eff_Unbinned;
  TH1D* OpticalLength;
  TH1D* OpticalLength_Detected;
  TH1D* NumWrapReflection;
  TH1D* NumWrapReflection_Detected;
  TH1D* NumPCBReflection;
  TH1D* NumPCBReflection_Detected;
  TH2D* FinalPosition;
  TH2D* FinalPosition_Detected;
  TH1D* FinalPositionX_Detected;

  void AddToTree( TTree* );
  void LoadBranches( TTree* );

  void Fill( const LYSimRunFormat&, const LYSimFormat& );
  bool operator() (const LYSimRunFormat&, const LYSimRunFormat& ) const;
};

// List of options for plotting.
extern boost::program_options::options_description GeometryOptions();

double      FormatOpt( const LYSimRunFormat&, const std::string& );
std::string FormatOptString( const LYSimRunFormat&, const std::string& );

// Additional graph processing.
extern TGraph* MakeGraph( const LYSimRunFormat&,
                          const LYvXGraphContainer&,
                          const std::string& );
extern TH1D* MakeDetXHist( const LYSimRunFormat&,
                           const LYvXGraphContainer& );
extern TH1D* MakePhotonHist( const LYSimRunFormat&,
                        const LYvXGraphContainer&,
                        const std::string& );
extern TH1D* MakePhotonAccum( const LYSimRunFormat&,
                        const LYvXGraphContainer&,
                        const std::string& );



#endif
