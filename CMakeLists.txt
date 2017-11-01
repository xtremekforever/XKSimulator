
project( XKSimulator )
cmake_minimum_required(VERSION 2.6)

add_subdirectory( qextserialport )

set( VERSION_MAJOR     0 )
set( VERSION_MINOR     1 )
set( VERSION_STABILITY "Beta" )

set( VERSION_STR "v${VERSION_MAJOR}.${VERSION_MINOR}" )

if( CMAKE_BUILD_TYPE STREQUAL "debug" )
  # If we are building in debug mode, we will set the version string
  # to the version + a string that identies that this is a debug build.
  set( VERSION "${VERSION_STR} *DEBUG BUILD*")
  set( simulator_target xksimulator-${VERSION_STR}-dbg )

  # Add a definition to indicate that we are in debug mode.
  add_definitions( -DXKSIMULATOR_DEBUG )
else()
  set( VERSION "${VERSION_STR} ${VERSION_STABILITY}" )
  set( simulator_target xksimulator-${VERSION_STR} )
endif()

# This command find Qt4 and all required variables
find_package( Qt4 REQUIRED )

IF (APPLE)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation )
  find_library(IOKIT_LIBRARY IOKit )

  set( simulator_target XKSimulator )
  #set( MACOSX_BUNDLE_ICON_FILE xkmanager.icns )
  set( MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION} )
  set( MACOSX_BUNDLE_VERSION ${VERSION} )
  set( MACOSX_BUNDLE_LONG_VERSION_STRING Version ${VERSION} )
ENDIF (APPLE)

#The QT_USE_FILE macro is defined by FIND_PACKAGE that contains a path to the CMake script
include( ${QT_USE_FILE} )

include_directories( ${CMAKE_BINARY_DIR}
                     ${LIB_INCLUDE_DIRS}
                     .
                     nmealib )

set ( nmea_headers
  nmealib/nmea/config.h
  nmealib/nmea/context.h
  nmealib/nmea/generate.h
  nmealib/nmea/generator.h
  nmealib/nmea/gmath.h
  nmealib/nmea/info.h
  nmealib/nmea/parse.h
  nmealib/nmea/parser.h
  nmealib/nmea/sentence.h
  nmealib/nmea/time.h
  nmealib/nmea/tok.h
  nmealib/nmea/units.h
)

set ( nmea_sources
  nmealib/context.c
  nmealib/generate.c
  nmealib/generator.c
  nmealib/gmath.c
  nmealib/info.c
  nmealib/parse.c
  nmealib/parser.c
  nmealib/sentence.c
  nmealib/time.c
  nmealib/tok.c
)

# Now these are the sources for the actual XKManager project
set ( simulator_sources
  main.cpp
  mainwindow.cpp
  serial.cpp
  travelline.cpp
  ${nmea_sources}
)

set ( simulator_headers
  defines.h
  ${nmea_headers}
)

set ( simulator_moc_headers
  serial.h
  mainwindow.h
)

set ( simulator_ui_files
  mainwindow.ui
)

set ( simulator_files
  README
)

if( MINGW )
  # resource compilation for MinGW)
  #add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/xkmanagerico.o
  #                    COMMAND windres.exe -I. -o ${CMAKE_CURRENT_BINARY_DIR}/xkmanagerico.o
  #                         -i xkmanagerico.rc WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
  #set(simulator_sources ${simulator_sources} ${CMAKE_CURRENT_BINARY_DIR}/xkmanagerico.o)
  set(simulator_sources ${simulator_sources} )
else( MINGW )
  #set(simulator_sources ${simulator_sources} xkmanagerico.rc)
  set(simulator_sources ${simulator_sources})
endif( MINGW )

link_directories( ${LIBRARY_OUTPUT_PATH} )

configure_file( version.h.in version.h )

qt4_add_resources( simulator_sources xksimulator.qrc)
qt4_wrap_cpp( simulator_sources ${simulator_moc_headers} )
qt4_wrap_ui( simulator_sources ${simulator_ui_files} )

if( APPLE )
  add_executable( ${simulator_target} MACOSX_BUNDLE ${simulator_sources} ${simulator_headers} ${simulator_files} )

  add_custom_command( TARGET ${simulator_target} POST_BUILD
    COMMAND mkdir -p ARGS ${CMAKE_CURRENT_BINARY_DIR}/${simulator_target}.app/Contents/Resources
    COMMAND cp -r ARGS ${CMAKE_CURRENT_SOURCE_DIR}/icons/ ${CMAKE_CURRENT_BINARY_DIR}/${simulator_target}.app/Contents/Resources
  )

  target_link_libraries( ${simulator_target} ${QT_LIBRARIES} ${LIB_TARGET} ${COREFOUNDATION_LIBRARY} ${IOKIT_LIBRARY} )
else( APPLE )
  if( CMAKE_BUILD_TYPE STREQUAL "debug" )
    add_executable( ${simulator_target} ${simulator_sources} ${simulator_headers} ${simulator_files} )
  else()
    add_executable( ${simulator_target} WIN32 ${simulator_sources} ${simulator_headers} ${simulator_files} )
  endif()

  target_link_libraries( ${simulator_target} ${QT_LIBRARIES} ${LIB_TARGET} )

  add_custom_target( run DEPENDS ${simulator_target} ${CMAKE_BINARY_DIR}/${simulator_target} )
endif( APPLE )
