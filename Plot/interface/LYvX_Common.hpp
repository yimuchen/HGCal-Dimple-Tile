#ifndef HGCALTILESIM_PLOT_LYVX_COMMON_HPP
#define HGCALTILESIM_PLOT_LYVX_COMMON_HPP

#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

// Typedef for simpler declaration
typedef bool (* RunFormatCompare)( const LYSimRunFormat&, const LYSimRunFormat& );

// Comparison function required for std::map.
bool LYvXGraphCompare( const LYSimRunFormat& x, const LYSimRunFormat& y );

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

  void AddToTree( TTree* );
  void LoadBranches( TTree* );

  void Fill( const LYSimRunFormat&, const LYSimFormat& );
};



#endif
