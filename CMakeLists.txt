##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(climatology_pi)
SET(PACKAGE_NAME climatology_pi)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "8")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_CLIMATOLOGY
            src/ClimatologyOverlayFactory.cpp
            src/climatology_pi.cpp
            src/ClimatologyUI.cpp
            src/ClimatologyDialog.cpp
            src/ClimatologyConfigDialog.cpp
            src/zuFile.cpp
            src/IsoBarMap.cpp
            src/icons.cpp

            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonwriter.cpp
)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_CLIMATOLOGY})

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

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
