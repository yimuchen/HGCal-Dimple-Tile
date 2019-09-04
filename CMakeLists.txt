#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(LYSim)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_compile_options("-g")
add_compile_options("-O3")
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
find_package(Geant4 REQUIRED ui_all vis_all)
# find_package(Boost REQUIRED
#     COMPONENTS program_options)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package) # Already done in Geant4
#
#find_package(ROOT)
#if(NOT ROOT_FOUND)
#  message(STATUS "G4 Examples: ROOT package not found. Build disabled.")
#  return()
#endif()


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/Tile/interface
                    ${Geant4_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIR}
                    ${ROOTSYS}/include/)

file(GLOB Tile_src ${PROJECT_SOURCE_DIR}/Tile/src/*.cc)
add_library(Tile SHARED ${Tile_src})
target_link_libraries(Tile ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Get examples sources from common and shared
#
#list(APPEND sources
  # uncomment the line below to include HBOOK analysis
  # ${PROJECT_SOURCE_DIR}/../../common/analysis/src/ExG4HbookAnalysisManager.cc
#)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(LYSim run/LYSim.cc)
target_link_libraries(LYSim Tile)

add_executable(LY_SquareTrigger run/LY_SquareTrigger.cc)
target_link_libraries(LY_SquareTrigger)
target_link_libraries(LY_SquareTrigger Tile )


#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build AnaEx02. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
#set(LYSim_SCRIPTS
#	vis.mac photontest.mac init_vis.mac init.mac HeatMap.C AnalysisOutput.C photontest_000.mac condor-jobs.jdl condor-executable.sh
#  )

# foreach(_script ${LYSim_SCRIPTS})
#   configure_file(
#     ${PROJECT_SOURCE_DIR}/${_script}
#     ${PROJECT_BINARY_DIR}/${_script}
#     COPYONLY
#     )
# endforeach()

#----------------------------------------------------------------------------
# Copy ROOT macros used for Analysis.
#
#file(COPY macros DESTINATION ${PROJECT_BINARY_DIR})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS LYSim DESTINATION bin)

