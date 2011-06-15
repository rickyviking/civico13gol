#// ========================================================================
#// @author Riccardo Corsi
#//
#// ========================================================================



# application name
SET(civico13golApp_NAME civico13golApp)

# application header path
#SET(HEADER_PATH ${ITscape_SOURCE_DIR}/applications/${civico13golApp_NAME})

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
     
     
# additional include directories
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR} # for ui*.h files
)

               
# qt process stuffs
qt4_wrap_cpp(MOC_SOURCES ${civico13golApp_HDRS})
#QT4_ADD_RESOURCES(RCC_SOURCES resources/itCLientQt.qrc)
QT4_WRAP_UI(UI_SOURCES GOL.ui)



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
# Qt libs - note: following variables are defined by the cMake routine. They contain both debug and release versions
   ${QT_QTCORE_LIBRARY}
   ${QT_QTGUI_LIBRARY}	
)


TARGET_LINK_LIBRARIES(${civico13golApp_NAME}
    ${civico13golApp_EXT_LIBS}
)

#==============================================================================
# Installer
#==============================================================================
# Add stuff that needs to be packaged in the installer
INSTALL (TARGETS ${civico13golApp_NAME} RUNTIME DESTINATION ./bin COMPONENT Client)
