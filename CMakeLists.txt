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

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)


IF(QT_ANDROID)
    SET(SRC_BZIP_ANDROID
            src/bzip2/bzlib.c
            src/bzip2/blocksort.c
            src/bzip2/compress.c
            src/bzip2/crctable.c
            src/bzip2/decompress.c
            src/bzip2/huffman.c
            src/bzip2/randtable.c
    )
    SET(SRC_GRIB ${SRC_GRIB} ${SRC_BZIP_ANDROID})

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
    INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/bzip2)

    MESSAGE (STATUS ${SRC_GRIB})
ENDIF(QT_ANDROID)

IF(WIN32)
  IF(MSVC)
    SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE(MSVC)
    SET(OPENCPN_IMPORT_LIB "${PARENT}.dll")
    SET( CMAKE_SHARED_LINKER_FLAGS "-L../../" )
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME}
      "${CMAKE_SOURCE_DIR}/buildwin/zlib.lib}
  ENDIF(MSVC)
  TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP ${OPENCPN_IMPORT_LIB})
  ADD_DEPENDENCIES(${PACKAGE_NAME} ${PARENT})
ENDIF(WIN32)


 IF(MINGW)
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib.lib")
ENDIF(MINGW)

INCLUDE("cmake/PluginJSON.cmake")
INCLUDE("cmake/PluginXML.cmake")
INCLUDE("cmake/PluginGL.cmake")

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
