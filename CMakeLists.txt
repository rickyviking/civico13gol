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

# check Cmake version
IF(WIN32)
   CMAKE_MINIMUM_REQUIRED(VERSION 2.4.8 FATAL_ERROR)
ELSE(WIN32)
   IF(APPLE)
      CMAKE_MINIMUM_REQUIRED(VERSION 2.4.8 FATAL_ERROR)
      IF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4 AND ${CMAKE_PATCH_VERSION} LESS 7)
         MESSAGE("Warning: A critical CMake bug exists in 2.4.6 and below. Trying to build Universal Binaries will result in a compile error that seems unrelated. Either avoid building Universal Binaries by changing the CMAKE_OSX_ARCHITECTURES field to list only your architecture, or upgrade to the current CVS version of CMake or a newer stable version if it exists.")
      ENDIF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4 AND ${CMAKE_PATCH_VERSION} LESS 7)
   ELSE(APPLE)
      CMAKE_MINIMUM_REQUIRED(VERSION 2.4.8 FATAL_ERROR)
   ENDIF(APPLE)
ENDIF(WIN32)


#==============================================================================
#  General settings
#==============================================================================

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
IF(WIN32)

    IF(MSVC)
        # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
        OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." OFF)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)

        # turn off various warnings
        # foreach(warning 4244 4251 4267 4275 4290 4786 4305 4996)
        #     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}")
        # endforeach(warning)

        # More MSVC specific compilation flags
        ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ENDIF(MSVC)

    # needed for net plugin
    SET (OSG_SOCKET_LIBS wsock32)
    # Both Cygwin and Msys need -DNOMINMAX ???
    IF(UNIX)
        ADD_DEFINITIONS(-DNOMINMAX)
    ENDIF(UNIX)
	
ENDIF(WIN32)

#==============================================================================
#  Package(s) import
#==============================================================================

#Run FindQt4.cmake
FIND_PACKAGE(Qt4 REQUIRED)


#==============================================================================
#  Project include(s) folder(s)
#==============================================================================
INCLUDE_DIRECTORIES(

	${civico13gol_SOURCE_DIR}/include/   

	#defined in FindQt4.cmake
	${QT_INCLUDE_DIR}		#QT headers
)

#==============================================================================
#  Project librarie(s) folder(s)
#==============================================================================
LINK_DIRECTORIES(

	#defined in FindQt4.cmake
	${QT_LIBRARY_DIR}		#QT libraries
)


#==============================================================================
#  Add sub-projects now
#==============================================================================

# whole source tree
ADD_SUBDIRECTORY (src)

# installer project
#ADD_SUBDIRECTORY (installer)