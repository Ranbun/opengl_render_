#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "polyclipping::polyclipping" for configuration "Debug"
set_property(TARGET polyclipping::polyclipping APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(polyclipping::polyclipping PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/../../debug/lib/polyclipping.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS polyclipping::polyclipping )
list(APPEND _IMPORT_CHECK_FILES_FOR_polyclipping::polyclipping "${CMAKE_CURRENT_LIST_DIR}/../../debug/lib/polyclipping.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
