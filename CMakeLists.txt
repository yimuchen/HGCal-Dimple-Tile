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
find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

## Passing source directory to file to enable data file finding.
configure_file(Tile/src/ProjectPath.cc.in  Tile/src/ProjectPath.cc)


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
# Add the executable, and link it to the Geant4 libraries
#
add_executable(LYSim Tile/bin/InteractiveSession.cc)
target_link_libraries(LYSim Tile)

install(TARGETS LYSim DESTINATION bin)
