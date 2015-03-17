#// ========================================================================
#// @author Riccardo Corsi
#//
#// ========================================================================



# application name
SET(civico13golApp_NAME civico13golApp)

# header files
SET (civico13golApp_HDRS 
   ../include/grid.h
   ../include/gridScene.h
   ../include/imageviewer.h
)

# source files
SET( civico13golApp_SRCS
   grid.cpp
   GridScene.cpp
   imageviewer.cpp
   main.cpp
)    
     



#==============================================================================
# Setup Qt5 stuff

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5PrintSupport)

# path of the resources for the interface 
#SET(RCC_SOURCES_PATH ${Gilgamesh_SOURCE_DIR}/applications)

# qt process stuffs
#QT5_ADD_RESOURCES(RCC_SOURCES ${RCC_SOURCES_PATH}/GmResources.qrc)
QT5_WRAP_UI(UI_SOURCES GOL.ui)

#==============================================================================



# Project files to build
ADD_EXECUTABLE(${civico13golApp_NAME}
	${civico13golApp_SRCS}
	${civico13golApp_HDRS}
	${MOC_SOURCES}
  #  ${RCC_SOURCES}
    ${UI_SOURCES}
)

SET(TARGET_LABEL "${APPLICATION_LABEL_PREFIX} ${civico13golApp_NAME}")
SET_TARGET_PROPERTIES(${civico13golApp_NAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
SET_TARGET_PROPERTIES(${civico13golApp_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")


SET( civico13golApp_EXT_LIBS
   # Qt libs
   Qt5::Widgets
   Qt5::PrintSupport
)


TARGET_LINK_LIBRARIES(${civico13golApp_NAME}
    ${civico13golApp_EXT_LIBS}
)

#==============================================================================
# Installer
#==============================================================================
# Add stuff that needs to be packaged in the installer
INSTALL (TARGETS ${civico13golApp_NAME} RUNTIME DESTINATION ./bin COMPONENT Client)
