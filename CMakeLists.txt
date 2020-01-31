#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(LYSim)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if(${CMAKE_VERSION} VERSION_GREATER "3.3.0")
  add_compile_options("-g")
  add_compile_options("-O3")
else ()
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -f -O3")
endif()

# find_package( ROOT REQUIRED)
execute_process(COMMAND "root-config" "--version"
                OUTPUT_VARIABLE ROOTCONFIG_VERSION)
string(SUBSTRING ${ROOTCONFIG_VERSION} 0 1 ROOTCONFIG_MAJOR_VERSION)
if( ${ROOTCONFIG_MAJOR_VERSION} STREQUAL "6" )
  message("BUILDING WITH ROOT.CMAKE ")
  find_package(ROOT REQUIRED)
  include_directories(${ROOT_INCLUDE_DIRS})
  add_compile_options("-std=c++17")
else()
  ## Adding ROOT the old fashioned way
  message("BUILDING WITH root-config outputs")
  execute_process(COMMAND "root-config" "--cflags"
                  OUTPUT_VARIABLE  ROOT_CONFIG )
  string(REPLACE " " ";" ROOTCONFIG_LIST ${ROOT_CONFIG})
  foreach(CONFIG ${ROOTCONFIG_LIST})
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CONFIG}")
  endforeach()

  execute_process(COMMAND "root-config" "--incdir"
                  OUTPUT_VARIABLE ROOT_INC )
  include_directories(${ROOT_INC})
  add_definitions(-std=c++0x)
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  message("${CMAKE_CXX_FLAGS}")
endif()

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
include_directories(${PROJECT_SOURCE_DIR}/Tile/interface
                    ${Geant4_INCLUDE_DIR})

file(GLOB Tile_src ${PROJECT_SOURCE_DIR}/Tile/src/*.cc)
add_library(Tile SHARED ${Tile_src})

if( ${ROOTCONFIG_MAJOR_VERSION} STREQUAL "6" )
  target_link_libraries( Tile ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
else()
  target_link_libraries(Tile ${Geant4_LIBRARIES}  "Core"  "Hist" "Tree")
endif()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(LYSim Tile/bin/InteractiveSession.cc)
target_link_libraries(LYSim Tile ${Geant4_LIBRARIES})

install(TARGETS LYSim DESTINATION bin)
