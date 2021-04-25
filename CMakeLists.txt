## ---------------------------------------------------------------------------
## Author:      Sean d'Epagnier
## ***************************************************************************
## *   Copyright (C) 2010 by Sean d'Epagnier                              *
## *   License:     GPLv3+
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 3 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   Free Software Foundation, Inc.,                                       *
## *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
## ---------------------------------------------------------------------------
## Frontend2 Author: Jon Gough   v1.0.161
## ---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur   ----- ##
## ----- Changes should only be made between the section blocks that identify where ---- ##
## ----- these changes should be. The whole configuration process relies on this   ----- ##
## ----- sequence to be successful                                                 ----- ##


## ----- Modify section below to include all the details for your plugin ----- ##

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
set(VERBOSE_NAME Climatology)
set(COMMON_NAME "Climatology")
set(TITLE_NAME "climatology")
set(PACKAGE_CONTACT "Sean D'Epagnier")
set(PACKAGE "climatology")
set(SHORT_DESCRIPTION "Climatology PlugIn: 32 years of monthly NOAA Data")
set(LONG_DESCRIPTION "Climatology provides overlay capabilities for historic weather data. Supported Climatology types include: - Average wind directions and speed- Percentage of gale and calm conditions - Sea Surface Currents - Average swell and sea state (not yet implemented) - Precipitation, Humidity, Cloud Cover, and Lightning Strikes - Monthly average Sea Level pressure and Sea Temperature, Air Temperature - Tropical Cyclone tracks")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "4")
set(VERSION_PATCH "23")
set(VERSION_TWEAK "1")
set(VERSION_DATE "19/04/2021") # DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
#Now set in plugin configure, needed to build flatpak, also locally.
#set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
# set(GIT_USER "rgleason")
# set(GIT_REPOSITORY_NAME "climatology_pi")

## Specifies Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
# set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "climatology")  #without the _pi

#set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/climatology.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

## ----- Modify section above to include all the details for your plugin ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
# set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
# set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" ON)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##

project(${PACKAGE})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# =======================================================
include("PluginSetup")
# =======================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin -----

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
# ========================================================
include("PluginConfigure")
# ========================================================
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##

add_definitions(-DUSE_S57)

IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "${CMLOC}SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)

#======== Below from cmakelists.txt  climatology_pi
# ----Remember to Add your project Set(SRCS and set(HDRS  etc..

SET(SRC_CLIMATOLOGY
            src/climatology_pi.cpp
            src/ClimatologyConfigDialog.cpp
            src/ClimatologyDialog.cpp
            src/ClimatologyOverlayFactory.cpp
            src/ClimatologyUI.cpp
            src/icons.cpp
            src/IsoBarMap.cpp
            src/zuFile.cpp
)

SET(HDRS
            include/climatology_pi.h
			include/ClimatologyConfigDialog.h
			include/ClimatologyDialog.h
			include/ClimatologyOverlayFactory.h
			include/ClimatologyUI.h
			include/defs.h
			include/gldefs.h
			include/icons.h
			include/IsoBarMap.h
			include/linmath.h
			include/zuFile.h
)

set(LIBSRC
    libs/jasoncpp/src/json_reader.cpp
    libs/jasoncpp/src/json_value.cpp
    libs/jasoncpp/src/json_writer.cpp
)

set(LIBHDRS
    libs/GL/gl.h
    libs/GL/gl_private.h
    libs/GL/glext.h
    libs/GL/glu.h
    libs/jasoncpp/include/json/json.h
    libs/jasoncpp/include/json/reader.h
    libs/jasoncpp/include/json/value.h
    libs/jasoncpp/include/json/writer.h
    libs/jasoncpp/include/json/forwards.h
    libs/jasoncpp/include/json/features.h
    libs/jasoncpp/include/json/config.h
    libs/jasoncpp/include/json/autolink.h
    libs/jasoncpp/include/json/assertions.h
)

add_definitions(-DPLUGIN_USE_SVG)

# =============================================================
SET(SRC  ${SRC_CLIMATOLOGY} ${HDRS} ${LIBSRC} ${LIBHDRS})
# =============================================================
# SET(SRC  ${SRC_CLIMATOLOGY} ${HDRS} ${LIBSRC} ${LIBHDRS} )

##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/GL)  # does not find gl.h when on, need as the next line 
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include/GL)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/plugingl)  #why is the no
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/jsoncpp)  #why is this needed when has cmakelists.txt
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/jsoncpp/include) # to find json.h  -why? same as above
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/ocpn-api)

if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

##=================================================
target_sources(${PACKAGE_NAME} PUBLIC ${SRC})
##=================================================
## ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_CLIMATOLOGY} ${HDRS} ${LIBSRC} ${LIBHDRS} )

add_definitions(-DTIXML_USE_STL)

## ----- Change above to match project requirements ----- ##

add_subdirectory(libs/tinyxml)
target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

IF(NOT UNIX)
    include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include/zlib-1.2.3)
    include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/bzip2)
    add_subdirectory(${PROJECT_SOURCE_DIR}/libs/bzip2)
    target_link_libraries(${PACKAGE_NAME} ocpn::bzip2)
ENDIF(NOT UNIX)

IF(WIN32)
  IF(MSVC)
    SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
    include_directories(BEFORE ${CMAKE_SOURCE_DIR}/buildwin/include)
    target_link_libraries(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE()
    # MINGW
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lz")
  ENDIF()
ENDIF(WIN32)


#------- Change below to match project requirements --------------

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/jsoncpp)
add_subdirectory(libs/jsoncpp)
target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/plugingl/include)
add_subdirectory(libs/plugingl)
target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

# Added for android builds
if(QT_ANDROID)
    include_directories(BEFORE ${qt_android_include})
endif(QT_ANDROID)

## ----- Change above to match project requirements ----- ##
## ----- do not change next section - needed to configure build process ----- ##
## ========================================================
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
## ========================================================
## ----- do not change section above - needed to configure build process ----- ##

message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
