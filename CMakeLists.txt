##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)
cmake_policy(SET CMP0012 NEW)

PROJECT(climatology_pi)

SET(PACKAGE_NAME climatology_pi)
SET(VERBOSE_NAME Climatology)
SET(TITLE_NAME CLIMATOLOGY)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

SET(BUNDLE_DATA TRUE)

INCLUDE( VERSION.cmake )
SET(PLUGIN_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}" )

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_CLIMATOLOGY
            src/ClimatologyOverlayFactory.cpp
            src/climatology_pi.cpp
            src/ClimatologyUI.cpp
            src/ClimatologyDialog.cpp
            src/ClimatologyConfigDialog.cpp
            src/zuFile.cpp
            src/IsoBarMap.cpp
            src/TexFont.cpp
            src/icons.cpp
)

IF(WIN32)
SET(SRC_JSON
            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonwriter.cpp)
ENDIF(WIN32)

ADD_DEFINITIONS(-DTIXML_USE_STL)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_CLIMATOLOGY} ${SRC_JSON})

IF(NOT UNIX)
    SET(SRC_BZIP
            src/bzip2/bzlib.c 
            src/bzip2/blocksort.c 
            src/bzip2/compress.c 
            src/bzip2/crctable.c 
            src/bzip2/decompress.c 
            src/bzip2/huffman.c 
            src/bzip2/randtable.c
    )
    ADD_LIBRARY(LIB_BZIP STATIC ${SRC_BZIP})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP )

    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/zlib-1.2.3)
    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)

IF(MINGW)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lwxzlib-2.8")
ENDIF(MINGW)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
