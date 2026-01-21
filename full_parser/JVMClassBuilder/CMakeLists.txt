cmake_minimum_required(VERSION 3.12)

project(jvm-class-builder
        VERSION 0.1.0
        DESCRIPTION "Structured JVM class file builder."
        LANGUAGES CXX
)

option(BUILD_SHARED_LIBS "Build libraries as shared" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

find_program(MAVEN_EXECUTABLE mvn REQUIRED)

set(JAVA_INTERNAL_PROJECT_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/java-internal
)
set(JAVA_INTERNAL_BUILD_DIR
        ${CMAKE_BINARY_DIR}/java
)
set(JAVA_INTERNAL_JAR
        ${JAVA_INTERNAL_BUILD_DIR}/fix_code_attribute.jar
)
add_custom_command(
        OUTPUT ${JAVA_INTERNAL_JAR}
        COMMAND ${MAVEN_EXECUTABLE}
        -q
        -DskipTests
        clean package
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${JAVA_INTERNAL_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
        ${JAVA_INTERNAL_PROJECT_DIR}/target/fix_code_attribute.jar
        ${JAVA_INTERNAL_JAR}
        WORKING_DIRECTORY ${JAVA_INTERNAL_PROJECT_DIR}
        COMMENT "Building internal Java helper (Maven)"
)

# new target for build java
add_custom_target(java-internal ALL
        DEPENDS ${JAVA_INTERNAL_JAR}
)


set(JAVA_HOME $ENV{JAVA_HOME})

if (NOT JAVA_HOME)
    message(FATAL_ERROR "Can't find JAVA_HOME env variable. Please set it.")
endif ()

message(STATUS "Using JAVA_HOME: ${JAVA_HOME}")

# Set paths for include and libraries based on JAVA_HOME
set(JAVA_INCLUDE_PATH ${JAVA_HOME}/include)
if(WIN32)
    set(JAVA_INCLUDE_PATH2 "${JAVA_HOME}/include/win32")

    if(EXISTS "${JAVA_HOME}/lib/server/jvm.lib")
        set(JAVA_LIBRARIES "${JAVA_HOME}/lib/server/jvm.lib")
    elseif(EXISTS "${JAVA_HOME}/lib/jvm.lib")
        set(JAVA_LIBRARIES "${JAVA_HOME}/lib/jvm.lib")
    elseif(EXISTS "${JAVA_HOME}/lib/server/jvm.dll")
        set(JAVA_LIBRARIES "${JAVA_HOME}/lib/server/jvm.dll")
    else()
        message(FATAL_ERROR "Could not find JVM library under JAVA_HOME='${JAVA_HOME}'. Tried: lib/server/jvm.lib, lib/jvm.lib, lib/server/jvm.dll")
    endif()
elseif(APPLE)
    set(JAVA_INCLUDE_PATH2 ${JAVA_HOME}/include/darwin)
    set(JAVA_LIBRARIES ${JAVA_HOME}/lib/server/libjvm.dylib)
elseif(UNIX)
    set(JAVA_INCLUDE_PATH2 ${JAVA_HOME}/include/linux)
    set(JAVA_LIBRARIES ${JAVA_HOME}/lib/server/libjvm.so)
endif()

# Check if libjvm.so exists and is accessible
if (NOT EXISTS ${JAVA_LIBRARIES})
    message(FATAL_ERROR "Can't find libjvm.so in ${JAVA_LIBRARIES}. Please ensure the JDK is correctly installed.")
endif ()

# generate paths for internal java project
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/java-internal-paths.h.in.h
        ${CMAKE_CURRENT_SOURCE_DIR}/generated/java-internal-paths.h
)

add_library(jvm-class-builder
        include/jvm/serializable.h
        include/jvm/owner-aware.h
        src/class.cpp
        src/constant.cpp
        src/constant-utf-8-info.cpp
        src/constant-class.cpp
        src/constant-name-and-type.cpp
        src/constant-fieldref.cpp
        src/constant-methodref.cpp
        src/constant-interface-methodref.cpp
        src/constant-string.cpp
        src/constant-integer.cpp
        src/constant-float.cpp
        src/constant-double.cpp
        src/constant-long.cpp
        src/field.cpp
        src/method.cpp
        src/attribute.cpp
        src/attribute-code.cpp
        src/instruction.cpp
        src/instruction-jump.cpp
        src/instruction-ldc.cpp
        src/instruction-with-constant.cpp
        src/label.cpp
        src/exception-handler.cpp
        src/internal/utils.cpp
        src/descriptor-field.cpp
        src/descriptor-method.cpp
)

target_compile_features(jvm-class-builder PUBLIC cxx_std_20)
set_target_properties(jvm-class-builder PROPERTIES
        CXX_EXTENSIONS OFF
        EXPORT_NAME ClassBuilder
)

target_include_directories(jvm-class-builder
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
        ${JAVA_INCLUDE_PATH}
        ${JAVA_INCLUDE_PATH2}
)

add_library(jvm::ClassBuilder ALIAS jvm-class-builder)

# add dependence to main project
add_dependencies(jvm-class-builder java-internal)

target_link_libraries(jvm-class-builder
        PRIVATE
        ${JAVA_LIBRARIES}
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS jvm-class-builder
        EXPORT jvm-class-builderTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT jvm-class-builderTargets
        FILE jvm-class-builderTargets.cmake
        NAMESPACE jvm::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jvm-class-builder
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/jvm-class-builderConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/jvm-class-builderConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/jvm-class-builderConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jvm-class-builder
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/jvm-class-builderConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/jvm-class-builderConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jvm-class-builder
)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
