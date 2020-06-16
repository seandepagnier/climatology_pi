# ---------------------------------------------------------------------------
# Frontend2 Author: jongough aka. Jon Gough    License:wxWidgets License
# ---------------------------------------------------------------------------
##---------------------------------------------------------------------------
# Author:      Sean d'Epagnier
##***************************************************************************
## *   Copyright (C) 2010 by Sean d'Epagnier                              *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,                                       *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx          *
## ***
#-----------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# ---------------------------------------------------------------------------
# CMake-CI Number: 0.0.005  
#    - 2/19/2020 10 environments needed for beta testing.
#    - 3/30/2020 MacOS build fixed 12 env
# ---------------------------------------------------------------------------
## ----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
set(VERBOSE_NAME "climatology")

set(COMMON_NAME "climatology") # Search "getcommonname"in {plugin_pi].cpp
set(TITLE_NAME "climatology")
set(PACKAGE_CONTACT "Sean d'Epagnier")
set(PACKAGE "climatology")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "4")
set(VERSION_PATCH "2")
set(VERSION_TWEAK "1")
set(VERSION_DATE "30/04/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")
#set(GIT_USER "seandepagnier")
#set(GIT_USER "jongough")
set(GIT_USER "rgleason")

# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
set(GIT_REPOSITORY_SERVER "github.com")
set(GIT_REPOSITORY_NAME "climatology_pi")

# Specifies Cloudsmith repository suffix
set(PROD "prod")    #Standard Repos
set(BETA "beta")    #Standard Repos
set(ALPHA "alpha")  #Standard Repos

#set(CLOUDSMITH_USER "jon-gough")
#set(CLOUDSMITH_USER "sean-depagnier")								   
set(CLOUDSMITH_USER "rick-gleason")

#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins") #Rick's customization

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/climatology.html")
set(XML_SUMMARY "Plugin to test examples of the ODAPI and JSON interface for ODRAW 1234#")
set(XML_DESCRIPTION "Climatology Plugin provides overlay capabilities for historic weather data for Weather_routing_pi")

##
## ----- Modify section above to include all the details for your plugin ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)
set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")
##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##
project(${PACKAGE})

#===========================================================
INCLUDE("cmake/PluginSetup.cmake")
#===========================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
##
	

# ----- Modify section above for special plugin requirements----- ##
## ----- do not change next section - needed to configure build process ----- ##
#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
##
## ----- do not change section above - needed to configure build process ----- ##
##
#
# ----- Change below to match project requirements for source, headers, etc. ----- ##
#

# Used in textplugin. Just added.
add_definitions(-DUSE_S57)

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


add_definitions(-DPLUGIN_USE_SVG)

##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##================================================================================
#  SET(SRCS ${SRC_CLIMATOLOGY}) 
##================================================================================

##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
## EG  (from Testplugin) also in cmake/Plugin_____.cmake files
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_CLIMATOLOGY})

# Used in textplugin. Just added.
# add_definitions(-DTIXML_USE_STL)

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
    ADD_LIBRARY(LIB_BZIP_CL STATIC ${SRC_BZIP})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP_CL )

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)

IF(WIN32)
  IF(MSVC)
    SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
 	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE()
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lz")
  ENDIF()
ENDIF(WIN32)

#    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
#else(APPLE)
#    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
#endif(APPLE)

#if(PI_JSON_SCHEMA_VALIDATOR)
#    message(STATUS "${CMLOC}Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
#    add_definitions(-DPI_JSON_SCHEMA_VALIDATOR)
#    message(STATUS "${CMLOC}Adding JSON Schema validation")
#	set(EXTSRC ${EXTSRC} extsrc/json-schema-validator/json-schema-draft7.json.cpp extsrc/json-schema-validator/json-uri.cpp extsrc/json-schema-validator/json-validator.cpp)
#    set(EXTINCLUDE ${EXTINCLUDE} extinclude/nlohmann/json.hpp extinclude/json-schema-validator/json-schema.hpp)
#    include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude/json-schema-validator)
#else(PI_JSON_SCHEMA_VALIDATOR)
#    message(STATUS "${CMLOC}Not using JSON Schema validation - warning invalid JSON 'may' cause unexpected crashes")
#endif(PI_JSON_SCHEMA_VALIDATOR)						

##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##================================================================================	
#    SET(EXTSRC  ${EXTSRC}   etc   Can be in an if statement as below
##================================================================================

# add_library(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSSRC})

#-----Above From Testplugin_pi ----------------

#if(UNIX AND NOT APPLE)
#    if(PI_JSON_SCHEMA_VALIDATOR)
#        # Change OS/ABI to be UNIX - System V to make it run with OCPN
#        set(lib_name lib${PACKAGE_NAME}.so)
#        message(STATUS "${CMLOC}lib-name: ${lib_name}")
#        find_file(elfedit elfedit)
#        if(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#            message(STATUS "${CMLOC}Will ensure OS/ABI is set to 0 to match OCPN")
#            add_custom_command(
#                TARGET ${PACKAGE_NAME}
#                POST_BUILD
#                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                DEPENDS ${PACKAGE_NAME}
#                COMMENT " Running post build action on ${lib_name}."
#                COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) ')
#        else(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#            message(STATUS "${CMLOC}Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
#        endif(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
#    endif(PI_JSON_SCHEMA_VALIDATOR)
#endif(UNIX AND NOT APPLE)

#-----Above From Testplugin_pi ----------------

#--------REMOVE LN 194 to LN 296 from Testplugin_pi---------------
#------- Change below to match project requirements --------------

INCLUDE("cmake/PluginJSON.cmake")
INCLUDE("cmake/PluginXML.cmake")
INCLUDE("cmake/PluginGL.cmake")

#
# ----- Do not change - needed to build app ----- ##
#
#INCLUDE CMAKE #5,6 & 7 -----

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")