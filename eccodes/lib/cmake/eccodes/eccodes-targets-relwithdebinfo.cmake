#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "eccodes_memfs" for configuration "RelWithDebInfo"
set_property(TARGET eccodes_memfs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(eccodes_memfs PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libeccodes_memfs.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libeccodes_memfs.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS eccodes_memfs )
list(APPEND _IMPORT_CHECK_FILES_FOR_eccodes_memfs "${_IMPORT_PREFIX}/lib/libeccodes_memfs.so" )

# Import target "eccodes" for configuration "RelWithDebInfo"
set_property(TARGET eccodes APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(eccodes PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "eccodes_memfs"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libeccodes.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libeccodes.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS eccodes )
list(APPEND _IMPORT_CHECK_FILES_FOR_eccodes "${_IMPORT_PREFIX}/lib/libeccodes.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
