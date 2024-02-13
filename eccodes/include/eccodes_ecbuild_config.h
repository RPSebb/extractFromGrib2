/*
 * (C) Copyright 2011- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef ECCODES_ecbuild_config_h
#define ECCODES_ecbuild_config_h

/* ecbuild info */

#ifndef ECBUILD_VERSION_STR
#define ECBUILD_VERSION_STR "3.8.2"
#endif
#ifndef ECBUILD_VERSION
#define ECBUILD_VERSION "3.8.2"
#endif
#ifndef ECBUILD_MACROS_DIR
#define ECBUILD_MACROS_DIR  "/home/rpsebb/downloads/eccodes-2.33.0-Source/cmake"
#endif

/* config info */

#define ECCODES_OS_NAME          "Linux-5.10.102.1-microsoft-standard-WSL2"
#define ECCODES_OS_BITS          64
#define ECCODES_OS_BITS_STR      "64"
#define ECCODES_OS_STR           "linux.64"
#define ECCODES_OS_VERSION       "5.10.102.1-microsoft-standard-WSL2"
#define ECCODES_SYS_PROCESSOR    "x86_64"

#define ECCODES_BUILD_TIMESTAMP  "20240123135952"
#define ECCODES_BUILD_TYPE       "RelWithDebInfo"

#define ECCODES_C_COMPILER_ID      "GNU"
#define ECCODES_C_COMPILER_VERSION "11.4.0"

#define ECCODES_CXX_COMPILER_ID      "GNU"
#define ECCODES_CXX_COMPILER_VERSION "11.4.0"

#define ECCODES_C_COMPILER       "/usr/bin/cc"
#define ECCODES_C_FLAGS          " -pipe -O2 -g -DNDEBUG"

#define ECCODES_CXX_COMPILER     "/usr/bin/c++"
#define ECCODES_CXX_FLAGS        " -pipe -Wno-write-strings -Wno-deprecated -O2 -g -DNDEBUG"

/* Needed for finding per package config files */

#define ECCODES_INSTALL_DIR       "/home/rpsebb/eccodes-2.33.0"
#define ECCODES_INSTALL_BIN_DIR   "/home/rpsebb/eccodes-2.33.0/bin"
#define ECCODES_INSTALL_LIB_DIR   "/home/rpsebb/eccodes-2.33.0/lib"
#define ECCODES_INSTALL_DATA_DIR  "/home/rpsebb/eccodes-2.33.0/share/eccodes"

#define ECCODES_DEVELOPER_SRC_DIR "/home/rpsebb/downloads/eccodes-2.33.0-Source"
#define ECCODES_DEVELOPER_BIN_DIR "/home/rpsebb/downloads/build"

/* Fortran support */

#if 0

#define ECCODES_Fortran_COMPILER_ID      ""
#define ECCODES_Fortran_COMPILER_VERSION ""

#define ECCODES_Fortran_COMPILER ""
#define ECCODES_Fortran_FLAGS    ""

#endif

#endif /* ECCODES_ecbuild_config_h */
