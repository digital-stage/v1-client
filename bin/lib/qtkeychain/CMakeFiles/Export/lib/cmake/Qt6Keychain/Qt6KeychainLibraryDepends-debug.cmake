#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "qt6keychain" for configuration "Debug"
set_property(TARGET qt6keychain APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(qt6keychain PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libqt6keychain.0.12.90.dylib"
  IMPORTED_SONAME_DEBUG "/usr/local/lib/libqt6keychain.1.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS qt6keychain )
list(APPEND _IMPORT_CHECK_FILES_FOR_qt6keychain "${_IMPORT_PREFIX}/lib/libqt6keychain.0.12.90.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
