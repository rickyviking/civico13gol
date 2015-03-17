#// ========================================================================
#//
#// @author Riccardo Corsi
#//
#// ========================================================================

#==============================================================================
#  civico13gol
#==============================================================================

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

PROJECT (civico13gol)


#==============================================================================
#  General settings
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.1 FATAL_ERROR)

# see cmake --help-policy CMP0003 for more information
IF(COMMAND cmake_policy)
   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# tell Cmake where to look for modules
SET(CMAKE_MODULE_PATH "${civico13gol_SOURCE_DIR}/CMakeModules/${CMAKE_MODULE_PATH}")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${civico13gol_SOURCE_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${civico13gol_SOURCE_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${civico13gol_SOURCE_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${civico13gol_SOURCE_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${civico13gol_SOURCE_DIR}/lib)

SET(CMAKE_DEBUG_POSTFIX "d")

################################################################################
# Create bin and lib directories if required

IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin ${CMAKE_BINARY_DIR}/lib )
ENDIF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
 
#==============================================================================
# Platform specific definitions 
#==============================================================================


#==============================================================================
#  Package(s) import
#==============================================================================

# Look for Qt5
SET (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} CACHE PATH "")
FIND_PACKAGE(Qt5Core REQUIRED)
IF (Qt5Core_FOUND)
   # add the path to glu32 lib for Qt
   SET (Gilgamesh_GLU32_LIB_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x86" CACHE PATH "Path to the glu32.lib file on Windows")
   SET (Gilgamesh_USER32_LIB_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib" CACHE PATH "Path to the user32.lib file on Windows")
   SET (CMAKE_LIBRARY_PATH 
         ${CMAKE_LIBRARY_PATH} 
         ${Gilgamesh_GLU32_LIB_PATH} 
         ${Gilgamesh_USER32_LIB_PATH} 
   ) 
   # make sure we also find the UIC executable, as we have some macros that need it
   set (tmpDir ${Qt5Core_DIR}/../../../bin)
   find_program(QT5_UIC_EXECUTABLE uic
         DOC "The Qt5 uic executable to wrap ui files"
         HINTS "tmpDir"
   )
   # needed to install Qt5 DLLs
   set(Qt5_BIN_DIR ${Qt5Core_DIR}/../../../bin CACHE PATH "")
   set(Qt5_PLUGINS_DIR ${Qt5Core_DIR}/../../../plugins CACHE PATH "")
ENDIF (Qt5Core_FOUND)
message ("Using CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH})



#==============================================================================
#  Project include(s) folder(s)
#==============================================================================
INCLUDE_DIRECTORIES(
   ${civico13gol_SOURCE_DIR}/include
)



#==============================================================================
#  Add sub-projects now
#==============================================================================

# whole source tree
ADD_SUBDIRECTORY (src)

# installer project
#ADD_SUBDIRECTORY (installer)