#----------------------------------------------------------------
# Generated CMake target import file for configuration "debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Centrality" for configuration "debug"
set_property(TARGET Centrality APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Centrality PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libCore.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libImt.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libRIO.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libNet.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libHist.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libGraf.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libGraf3d.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libGpad.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libTree.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libTreePlayer.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libRint.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libPostscript.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libMatrix.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libPhysics.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libMathCore.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libThread.so;/cvmfs/fairroot.gsi.de/fairsoft/may18/lib/root/libMultiProc.so"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libCentrality.so.1.0"
  IMPORTED_SONAME_DEBUG "libCentrality.so.1.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS Centrality )
list(APPEND _IMPORT_CHECK_FILES_FOR_Centrality "${_IMPORT_PREFIX}/lib/libCentrality.so.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
