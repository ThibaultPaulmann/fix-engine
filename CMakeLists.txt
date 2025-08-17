cmake_minimum_required(VERSION 3.20)

# ---- Project ----
project(FixEngine
  VERSION 0.1.0
  LANGUAGES CXX
)

# Helpful defaults
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(FIX_ENABLE_LTO "Enable interprocedural optimization/LTO if supported" OFF)
option(FIX_BUILD_TESTS "Enable building tests (add your own later)" OFF)

# ---- Global compile options (warnings etc.) ----
function(fix_set_warnings target)
  if (MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive- /Zc:__cplusplus)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
  endif()
endfunction()

# ---- Paths ----
set(FIX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---- fix-core ----
add_library(fix-core
  # core sources
  src/core/Field.cpp
  src/core/Message.cpp
  src/core/Parser.cpp
)

add_library(fix::core ALIAS fix-core)

target_include_directories(fix-core
  PUBLIC
    ${FIX_INCLUDE_DIR}
)

target_compile_features(fix-core PUBLIC cxx_std_23)
set_target_properties(fix-core PROPERTIES POSITION_INDEPENDENT_CODE ON)
fix_set_warnings(fix-core)

# ---- fix-protocol-fixt11 ----
add_library(fix-protocol-fixt11
  src/protocol/fixt11/HeaderTrailerRules.cpp
  # (AdminViews.hpp and Constants.hpp are headers-only)
)

add_library(fix::protocol::fixt11 ALIAS fix-protocol-fixt11)

target_include_directories(fix-protocol-fixt11
  PUBLIC
    ${FIX_INCLUDE_DIR}
)

target_compile_features(fix-protocol-fixt11 PUBLIC cxx_std_23)
set_target_properties(fix-protocol-fixt11 PROPERTIES POSITION_INDEPENDENT_CODE ON)
fix_set_warnings(fix-protocol-fixt11)
target_link_libraries(fix-protocol-fixt11 PUBLIC fix-core)

# ---- fix-engine ----
add_library(fix-engine
  src/engine/InMemoryStore.cpp
  # Session.hpp, ISessionHandler.hpp, SessionConfig.hpp are headers-only for now
)

add_library(fix::engine ALIAS fix-engine)

target_include_directories(fix-engine
  PUBLIC
    ${FIX_INCLUDE_DIR}
)

target_compile_features(fix-engine PUBLIC cxx_std_23)
set_target_properties(fix-engine PROPERTIES POSITION_INDEPENDENT_CODE ON)
fix_set_warnings(fix-engine)
target_link_libraries(fix-engine PUBLIC fix-protocol-fixt11 fix-core)

# ---- Optional: Enable LTO ----
if (FIX_ENABLE_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported OUTPUT ipo_msg)
  if (ipo_supported)
    set_property(TARGET fix-core fix-protocol-fixt11 fix-engine PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(STATUS "IPO/LTO not supported: ${ipo_msg}")
  endif()
endif()

# ---- Install / Export (optional, but handy) ----
include(GNUInstallDirs)

install(TARGETS fix-core fix-protocol-fixt11 fix-engine
  EXPORT FixTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${FIX_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT FixTargets
  FILE FixTargets.cmake
  NAMESPACE fix::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FixEngine
)

# Basic config file so downstreams can use find_package(FixEngine)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/FixEngineConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/FixEngineConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/FixEngineConfig.cmake"
  @ONLY
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/FixEngineConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/FixEngineConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FixEngine
)

# ---- Tests hook (you can add later) ----
if (FIX_BUILD_TESTS)
  enable_testing()
  # add_subdirectory(tests)  # when you have tests
endif()
