#ifndef HGCALTILESIM_PLOT_LYVX_COMMON_HPP
#define HGCALTILESIM_PLOT_LYVX_COMMON_HPP

#include "HGCalTileSim/Tile/interface/LYSimFormat.hh"

#include <string>

// Typedef for simpler declaration
typedef bool(* LYFormatCompare)( const LYSimFormat&, const LYSimFormat&);

// Comparison function required for std::map.
bool LYGraphCompare( const LYSimFormat& x, const LYSimFormat& y );

// Generating ther graphname
extern std::string LYGraphName( const LYSimFormat& x );
extern LYSimFormat FromLYGraphName( const std::string& x );

#endif