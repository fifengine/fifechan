#
# Copyright 2024 Jens A. Koch.
# SPDX-License-Identifier: MIT
# This file is part of fifengine/fifechan.
#

#
# SetupVcpkg
#
# This CMake script configures VCPKG to automatically install the dependencies
# defined in vcpkg.json during the configure step of CMake (vcpkg install).
# It relies on environment variables instead of command-line options.
#

set(VCPKG_VERBOSE ON)

if(DEFINED ENV{VCPKG_VERBOSE} AND NOT DEFINED VCPKG_VERBOSE)
    set(VCPKG_VERBOSE "$ENV{VCPKG_VERBOSE}" CACHE BOOL "")
endif()

# Use aria2 for faster downloads. By default curl is used.
set(VCPKG_INSTALL_OPTIONS "--x-use-aria2")

#
# Automatic installation of vcpkg dependencies.
#
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
set(VCPKG_APPLOCAL_DEPS_INSTALL ON)

# Copy dependencies into the output directory for executables.
if(DEFINED ENV{VCPKG_APPLOCAL_DEPS} AND NOT DEFINED VCPKG_APPLOCAL_DEPS)
    set(VCPKG_APPLOCAL_DEPS "$ENV{VCPKG_APPLOCAL_DEPS}" CACHE BOOL "")
endif()

# Copy dependencies into the install target directory for executables.
if(DEFINED ENV{X_VCPKG_APPLOCAL_DEPS_INSTALL} AND NOT DEFINED VCPKG_APPLOCAL_DEPS)
    # X_VCPKG_APPLOCAL_DEPS_INSTALL depends on CMake policy CMP0087
    if(POLICY CMP0087)
        cmake_policy(SET CMP0087 NEW)
    endif()
    set(X_VCPKG_APPLOCAL_DEPS_INSTALL "$ENV{X_VCPKG_APPLOCAL_DEPS_INSTALL}" CACHE BOOL "")
endif()

#
# VCPKG_ROOT
#
# Please set VCPKG_ROOT on your env: export VCPKG_ROOT=/opt/vcpkg/bin
# This avoids passing it on the configure line: -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
#
# If VCPKG_ROOT is not set, but the toolchain, we set root using the CMAKE_TOOLCHAIN_FILE.
# If CMAKE_TOOLCHAIN_FILE is not set, but VCPKG_ROOT, we set the toolchain using the VCPKG_ROOT.
#
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    if(NOT DEFINED ENV{VCPKG_ROOT})
        string(REPLACE "/scripts/buildsystems/vcpkg.cmake" "" _VCPKG_ROOT "${CMAKE_TOOLCHAIN_FILE}")
        set(ENV{VCPKG_ROOT} "${_VCPKG_ROOT}")
    endif()
elseif (DEFINED ENV{VCPKG_ROOT})
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    endif()
else()
    message(FATAL_ERROR "One of -DCMAKE_TOOLCHAIN_FILE or the VCPKG_ROOT environment variable must be set.")
endif()

#
# VCPKG_INSTALLED_DIR
#
# VCPKG_INSTALLED_DIR is the root folder for all compiled packages, e.g.
# this is the project local /workspace/vcpkg_installed/x64-windows.
#
# You might use this variable to point to the locations of your packages.
# This is useful for packages that do not provide a proper package config file
# for find_package().
#
# set(spdlog_DIR "${VCPKG_INSTALLED_DIR}/share/spdlog")
# find_package(spdlog CONFIG REQUIRED)
#
if(NOT DEFINED VCPKG_INSTALLED_DIR)
    set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}")
endif()

#
# VCPKG_FEATURE_FLAGS
#
# This env var can be set to a comma-separated list of off-by-default features in vcpkg.
# Available features are: manifests, binarycaching
# manifests     -> use the project-local manifest file "vcpkg.json" to build dependencies
# binarycaching -> use prebuild packages from cache to avoid rebuilds
#
if(NOT DEFINED VCPKG_FEATURE_FLAGS)
    set(VCPKG_FEATURE_FLAGS "manifests")
endif()

iF(NOT DEFINED VCPKG_MANIFEST_FILE)
    set(VCPKG_MANIFEST_FILE "${CMAKE_SOURCE_DIR}/vcpkg.json")
endif()

# create "vcpkg" source group for Visual Studio IDE
source_group("vcpkg" FILES
    "${CMAKE_SOURCE_DIR}/cmake/SetupVcpkg.cmake"
    "${CMAKE_SOURCE_DIR}/vcpkg.json"
)

#
# Check to make sure the VCPKG_TARGET_TRIPLET matches BUILD_SHARED_LIBS
#
if (DEFINED VCPKG_TARGET_TRIPLET AND "${VCPKG_TARGET_TRIPLET}" MATCHES ".*-static")
    if (BUILD_SHARED_LIBS)
        message(FATAL_ERROR "When the VCPKG_TARGET_TRIPLET ends with '-static' the BUILD_SHARED_LIBS must be 'OFF'.")
    endif()
else()
    if (NOT BUILD_SHARED_LIBS)
        # if VCPKG_TARGET_TRIPLET does not end with '-static' then set BUILD_SHARED_LIBS to 'ON' by default
        set(BUILD_SHARED_LIBS ON)
    endif()
endif()

#
# Print VCPKG configuration overview
#
message(STATUS "")
message(STATUS "[VCPKG]  Configuration Overview:")
message(STATUS "")
message(STATUS "[INFO]   ENV.VCPKG_ROOT                -> '$ENV{VCPKG_ROOT}'")
message(STATUS "[INFO]   BUILD_SHARED_LIBS             -> '${BUILD_SHARED_LIBS}'")
message(STATUS "[INFO]   CMAKE_TOOLCHAIN_FILE          -> '${CMAKE_TOOLCHAIN_FILE}'")

message(STATUS "")
message(STATUS "[VCPKG]  VCPKG_VERBOSE                 -> '${VCPKG_VERBOSE}'")
message(STATUS "[VCPKG]  VCPKG_INSTALL_OPTIONS         -> '${VCPKG_INSTALL_OPTIONS}'")
message(STATUS "[VCPKG]  VCPKG_APPLOCAL_DEPS           -> '${VCPKG_APPLOCAL_DEPS}'")
message(STATUS "[VCPKG]  VCPKG_FEATURE_FLAGS           -> '${VCPKG_FEATURE_FLAGS}'")
message(STATUS "[VCPKG]  VCPKG_MANIFEST_FILE           -> '${VCPKG_MANIFEST_FILE}'")
message(STATUS "[VCPKG]  VCPKG_INSTALLED_DIR           -> '${VCPKG_INSTALLED_DIR}'")
message(STATUS "[VCPKG]  VCPKG_DEFAULT_TRIPLET         -> '${VCPKG_DEFAULT_TRIPLET}'")
message(STATUS "[VCPKG]  VCPKG_TARGET_TRIPLET          -> '${VCPKG_TARGET_TRIPLET}'")
message(STATUS "")
