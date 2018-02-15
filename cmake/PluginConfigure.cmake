##---------------------------------------------------------------------------
## Author:      Pavel Kalian (Based on the work of Sean D'Epagnier)
## Copyright:   2014
## License:     GPLv3+
##---------------------------------------------------------------------------

SET(PLUGIN_SOURCE_DIR .)

# This should be 2.8.0 to have FindGTK2 module
IF (COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 OLD)
  CMAKE_POLICY(SET CMP0005 OLD)
  CMAKE_POLICY(SET CMP0011 OLD)
ENDIF (COMMAND cmake_policy)

MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

configure_file(cmake/version.h.in ${PROJECT_SOURCE_DIR}/src/version.h)
SET(PACKAGE_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${VERSION_PATCH}" )

#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# SET(PROFILING 1)
#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT MSVC)
 IF(PROFILING)
  ADD_DEFINITIONS( "-Wall -g -fprofile-arcs -ftest-coverage -fexceptions" )
 ELSE(PROFILING)
#  ADD_DEFINITIONS( "-Wall -g -fexceptions" )
 ADD_DEFINITIONS( "-Wall -Wno-unused-result -g -O2 -fexceptions -fPIC" )
 ENDIF(PROFILING)

 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
 ENDIF(NOT APPLE)

ENDIF(NOT MSVC)

# Add some definitions to satisfy MS
IF(MSVC)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(MSVC)


SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
SET(BUILD_SHARED_LIBS "ON")

IF(QT_ANDROID)
  SET(CMAKE_BUILD_TYPE Debug)
  ADD_DEFINITIONS(-D__WXQT__)
  ADD_DEFINITIONS(-DOCPN_USE_WRAPPER)
  ADD_DEFINITIONS(-D__OCPN__ANDROID__)
  ADD_DEFINITIONS(-DANDROID)

  SET(CMAKE_CXX_FLAGS "-pthread -fPIC -s -O2")

  ## Compiler flags
 #   if(CMAKE_COMPILER_IS_GNUCXX)
 #       set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
        set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
 #   endif()
    
  INCLUDE_DIRECTORIES("${Qt_Base}/include/QtCore")
  INCLUDE_DIRECTORIES("${Qt_Base}/include")
  INCLUDE_DIRECTORIES("${Qt_Base}/include/QtWidgets")
  INCLUDE_DIRECTORIES("${Qt_Base}/include/QtGui")
  INCLUDE_DIRECTORIES("${Qt_Base}/include/QtOpenGL")
  INCLUDE_DIRECTORIES("${Qt_Base}/include/QtTest")
  INCLUDE_DIRECTORIES("${Qt_Base}/../qtandroidextras/include")

  INCLUDE_DIRECTORIES( "${wxQt_Base}/${wxQt_Build}/lib/wx/include/arm-linux-androideabi-qt-unicode-static-3.1")
  INCLUDE_DIRECTORIES("${wxQt_Base}/include")

  ADD_DEFINITIONS(-DQT_WIDGETS_LIB)

ENDIF(QT_ANDROID)

#  QT_ANDROID is a cross-build, so the native FIND_PACKAGE(wxWidgets...) and wxWidgets_USE_FILE is not useful.
IF(NOT QT_ANDROID)
IF(NOT DEFINED wxWidgets_USE_FILE)
  SET(wxWidgets_USE_LIBS base core net xml html adv)
  SET(BUILD_SHARED_LIBS TRUE)
  set (WXWIDGETS_FORCE_VERSION CACHE VERSION "Force usage of a specific wxWidgets version.")
  if(WXWIDGETS_FORCE_VERSION)
    set (wxWidgets_CONFIG_OPTIONS --version=${WXWIDGETS_FORCE_VERSION})
  endif()
  FIND_PACKAGE(wxWidgets REQUIRED)
ENDIF(NOT DEFINED wxWidgets_USE_FILE)

  INCLUDE(${wxWidgets_USE_FILE})
ENDIF(NOT QT_ANDROID)


IF(MSYS)
# this is just a hack. I think the bug is in FindwxWidgets.cmake
STRING( REGEX REPLACE "/usr/local" "\\\\;C:/MinGW/msys/1.0/usr/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} )
ENDIF(MSYS)

#  QT_ANDROID is a cross-build, so the native FIND_PACKAGE(OpenGL) is not useful.
#
IF (NOT QT_ANDROID )
FIND_PACKAGE(OpenGL)
IF(OPENGL_GLU_FOUND)

    SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

    MESSAGE (STATUS "Found OpenGL..." )
    MESSAGE (STATUS "    Lib: " ${OPENGL_LIBRARIES})
    MESSAGE (STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
    ADD_DEFINITIONS(-DocpnUSE_GL)
ELSE(OPENGL_GLU_FOUND)
    MESSAGE (STATUS "OpenGL not found..." )
ENDIF(OPENGL_GLU_FOUND)
ENDIF(NOT QT_ANDROID)

#  Building for QT_ANDROID involves a cross-building environment,
#  So the OpenGL include directories, flags, etc must be stated explicitly
#  without trying to locate them on the host build system.
IF(QT_ANDROID)
    MESSAGE (STATUS "Using GLESv1 for Android")
    ADD_DEFINITIONS(-DocpnUSE_GLES)
    ADD_DEFINITIONS(-DocpnUSE_GL)
    ADD_DEFINITIONS(-DUSE_GLU_TESS)
    ADD_DEFINITIONS(-DARMHF)
   
    SET(OPENGLES_FOUND "YES")
    SET(OPENGL_FOUND "YES")

 #   IF(NOT DEFINED wxWidgets_USE_FILE)
#  SET(wxWidgets_USE_LIBS base core net xml html adv)
#  SET(BUILD_SHARED_LIBS TRUE)
#  FIND_PACKAGE(wxWidgets REQUIRED)
#ENDIF(NOT DEFINED wxWidgets_USE_FILE)

 # INCLUDE(${wxWidgets_USE_FILE})
  #  MESSAGE (STATUS "wxwidgets: " ${wxWidgets_USE_FILE})
    
#    SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl )
#    add_subdirectory(src/glshim)

#    add_subdirectory(src/glu)

ELSE(QT_ANDROID)
    FIND_PACKAGE(OpenGL)
    IF(OPENGL_GLU_FOUND)

        SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl)
        INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

        MESSAGE (STATUS "Found OpenGL..." )
        MESSAGE (STATUS "    Lib: " ${OPENGL_LIBRARIES})
        MESSAGE (STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
        ADD_DEFINITIONS(-DocpnUSE_GL)
    ELSE(OPENGL_GLU_FOUND)
        MESSAGE (STATUS "OpenGL not found..." )
    ENDIF(OPENGL_GLU_FOUND)

ENDIF(QT_ANDROID)

# On Android, PlugIns need a specific linkage set....
IF (QT_ANDROID )
  # These libraries are needed to create PlugIns on Android.

  SET(OCPN_Core_LIBRARIES
        # Presently, Android Plugins are built in the core tree, so the variables {wxQT_BASE}, etc.
        # flow to this module from above.  If we want to build Android plugins out-of-core, this will need improvement.

        # TODO This is pretty ugly, but there seems no way to avoid specifying a full path in a cross build....
        /home/sean/build/OpenCPN/build-android/libopencpn.so

        ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_core-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_html-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu_xml-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_qa-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_adv-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_aui-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_baseu_net-3.1-arm-linux-androideabi.a
        ${wxQt_Base}/${wxQt_Build}/lib/libwx_qtu_gl-3.1-arm-linux-androideabi.a
        ${Qt_Base}/lib/libQt5Core.so
        ${Qt_Base}/lib/libQt5OpenGL.so
        ${Qt_Base}/lib/libQt5Widgets.so
        ${Qt_Base}/lib/libQt5Gui.so
        ${Qt_Base}/lib/libQt5AndroidExtras.so

        #${NDK_Base}/sources/cxx-stl/gnu-libstdc++/4.8/libs/armeabi-v7a/libgnustl_shared.so
        )

ENDIF(QT_ANDROID)

SET(BUILD_SHARED_LIBS TRUE)

FIND_PACKAGE(Gettext REQUIRED)

