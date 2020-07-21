##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
# Frontend2 Author: Jon Gough  
# Frontend2: v1.0.127
# ---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##
##
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
set(VERBOSE_NAME Climatology)
set(COMMON_NAME "Climatology")
set(TITLE_NAME "climatology")
set(PACKAGE_CONTACT "Sean D'Epagnier ")
set(PACKAGE "climatology")
set(SHORT_DESCRIPTION "Climatology PlugIn: 32 years of monthly NOAA Data")
set(LONG_DESCRIPTION "Climatology PlugIn for OpenCPN")

# Temporary fix
#\n\
#Provides overlay capabilities for historic weather data.\n\n\
#Supported Climatology types include:\n\
#- Average wind directions and speed\n\
#- Percentage of gale and calm conditions\n\
#- Sea Surface Currents\n\
#- Average swell and sea state (not yet implemented)\n\
#- Precipitation, Humidity, Cloud Cover, and Lightning Strikes \n\
#- Monthly average Sea Level pressure and Sea Temperature, Air Temperature\n\
#- Tropical Cyclone tracks\
#")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "4")
set(VERSION_PATCH "6")
set(VERSION_TWEAK "0")
set(VERSION_DATE "07/21/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
#set(GIT_USER "rgleason")
#set(GIT_REPOSITORY_NAME "weather_routing_pi"

# Specifies cloudsmith repository sufficies
set(PROD "prod") # Standard Repos
set(BETA "beta") # Standard Repos
set(ALPHA "alpha") # Standard Repos
# Set if your cloudsmith repository name does not match your git repository name 
#set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

# Set if Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
set(CLOUDSMITH_BASE_REPOSITORY "climatology")  #without the _pi

# Set Plugin info-url, summary and description									 
set(XML_INFO_URL "https://opencpn.org/wiki/dokuwiki/doku.php?id=opencpn:opencpn_user_manual:plugins:safety:odraw1.6_pi")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})
											
##
## ----- Modify section above to include all the details for your plugin ----- ##
##

##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

# SET(CMAKE_MACOSX_RPATH "ON")

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)
option(USE_GL "Enable OpenGL support" ON)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

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
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;")

#========================================================
include("PluginSetup")
#========================================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
#========================================================
include("PluginConfigure")
#========================================================
## ----- do not change section above - needed to configure build process ----- ##
## ---- Change below to match project requirements for source, headers, etc. ----- ##
##

add_definitions(-DUSE_S57)

# From master branch - is this working? necessary?
IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "${CMLOC}SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)

#======== Below from cmakelists.txt  climatology_pi
# ----Remember to Add your project Set(SRCS and set(HDRS  etc..
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

SET(HDRS
            src/climatology_pi.h
			src/ClimatologyConfigDialog.h
			src/ClimatologyDialog.h
			src/ClimatologyOverlayFactory.h
			src/ClimatologyUI.h
			src/gldefs.h
			src/icons.h
			src/IsoBarMap.h
			src/linmath.h
			src/zuFile.h

)

SET(LIBSRC
            src/jsoncpp/json_reader.cpp
			src/jsoncpp/json_value.cpp
			src/jsoncpp/json_writer.cpp
			src/plugingl/pi_shaders.cpp
			src/plugingl/pidc.cpp
			src/plugingl/qtstylesheet.cpp
			src/plugingl/TexFont.cpp
			src/tinyxml/tinyxml.cpp
			src/tinyxml/tinyxmlerror.cpp
			src/tinyxml/tinyxmlparser.cpp
)

SET(LIBHDRS			
			src/jsoncpp/json_batchallocator.h
			src/jsoncpp/json_tool.h
			src/jsoncpp/json/assertions.h
			src/jsoncpp/json/autolink.h
			src/jsoncpp/json/config.h
			src/jsoncpp/json/features.h
			src/jsoncpp/json/forwards.h
			src/jsoncpp/json/json.h
			src/jsoncpp/json/reader.h
			src/jsoncpp/json/value.h
			src/jsoncpp/json/writer.h
			src/plugingl/linmath.h
			src/plugingl/pi_shaders.h
			src/plugingl/pidc.h
			src/plugingl/qtstylesheet.h
			src/plugingl/TexFont.h
			src/tinyxml/tinyxml.h
			include/GL/gl.h
			include/GL/gl_private.h
			include/GL/glext.h
			include/GL/glu.h
            api-16/ocpn_plugin.h			
)


##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)
#INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/src/libaudiofile)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/src/plugingl)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/src/tinyxml)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/src/jsoncpp)
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/src/jsoncpp/json)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include/GL)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/api-16)
#INCLUDE_DIRECTORIES(api-16)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_CLIMATOLOGY} ${HDRS} ${LIBSRC} ${LIBHDRS} )

IF(NOT UNIX)
    SET(SRC_BZIP
#			src/bzip2/bzlib.h					
            src/bzip2/bzlib.c 
            src/bzip2/blocksort.c 
            src/bzip2/compress.c 
            src/bzip2/crctable.c 
            src/bzip2/decompress.c 
            src/bzip2/huffman.c 
            src/bzip2/randtable.c
#			src/bzip2/bzlib_private.h								   
    )
    ADD_LIBRARY(LIB_BZIP_CL STATIC ${SRC_BZIP})
    TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} LIB_BZIP_CL )

    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/buildwin/include)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/bzip2)
ENDIF(NOT UNIX)

#added but not used, from wxroute
#IF(NOT UNIX)
#    INCLUDE_DIRECTORIES(src/zlib-1.2.3)
#    INCLUDE_DIRECTORIES(src/bzip2)
#	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
#ENDIF(NOT UNIX)
IF(WIN32)
  IF(MSVC)
    SET(OPENCPN_IMPORT_LIB "../../${CMAKE_CFG_INTDIR}/${PARENT}")
 	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
  ELSE()
    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lz")
  ENDIF()
ENDIF(WIN32)

# aded but not used from wx-route
#IF(WIN32)
#  IF(MSVC)
#    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/buildwin/include)
#	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
#    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "${CMAKE_SOURCE_DIR}/buildwin/zlib1.lib")
#  ELSE()
#    # MINGW
##    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lwxzlib-2.8")
#	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/jsoncpp)
#    TARGET_LINK_LIBRARIES(${PACKAGE_NAME} "-lz")
#  ENDIF()
#ENDIF()

INCLUDE("PluginJSON")
INCLUDE("PluginXML")
INCLUDE("PluginGL")

##
## ----- Change above to match project requirements ----- ##
## ----- do not change next section - needed to configure build process ----- ##
##
INCLUDE("PluginInstall")
INCLUDE("PluginLocalization")
INCLUDE("PluginPackage")

#from wxroute
get_target_property(OUT ${PACKAGE_NAME} LINK_LIBRARIES)
message(STATUS "${CMLOC}Libraries: ${OUT}")
##
## ----- do not change section above - needed to configure build process ----- ##
##			 
