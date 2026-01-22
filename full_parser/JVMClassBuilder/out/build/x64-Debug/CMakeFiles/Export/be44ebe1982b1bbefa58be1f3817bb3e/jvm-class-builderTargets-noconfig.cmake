#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "jvm::ClassBuilder" for configuration ""
set_property(TARGET jvm::ClassBuilder APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(jvm::ClassBuilder PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libjvm-class-builder.a"
  )

list(APPEND _cmake_import_check_targets jvm::ClassBuilder )
list(APPEND _cmake_import_check_files_for_jvm::ClassBuilder "${_IMPORT_PREFIX}/lib/libjvm-class-builder.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
