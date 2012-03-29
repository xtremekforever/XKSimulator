 
project(QextSerialPort)
cmake_minimum_required( VERSION 2.6 )
find_package( Qt4 REQUIRED )

include( ${QT_USE_FILE} )

set ( LIB_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}
    CACHE string "Paths to include QextSerialPort library headers."
)

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set( LIB_TARGET "qextserialportd" CACHE string "Name of the output QextSerialPort library." )
else()
    set( LIB_TARGET "qextserialport" CACHE string "Name of the output QextSerialPort library." )
endif()

set( QT_DONT_USE_QTGUI 1 )
set( QT_DEFINITIONS
    -DUNICODE
    -DQT_NO_DEBUG
)
add_definitions( ${QT_DEFINITIONS} -DQEXTSERIALPORT_LIB )

# Library source files.
set( QEXTSERIALPORT_SRCS
    qextserialport.cpp
    qextserialport.h
    qextserialenumerator.h
    qextserialport_global.h
)

if(WIN32)
    set( QEXTSERIALPORT_SRCS
        ${QEXTSERIALPORT_SRCS}
        win_qextserialport.cpp
        qextserialenumerator_win.cpp
    )
    set( ADDITIONAL_LIBRARY setupapi uuid advapi32 )
    add_definitions(-DWINVER=0x0501 )
elseif(UNIX)
    set( QEXTSERIALPORT_SRCS
        ${QEXTSERIALPORT_SRCS}
		posix_qextserialport.cpp
    )
    if(APPLE)
        set( QEXTSERIALPORT_SRCS
            ${QEXTSERIALPORT_SRCS}
			qextserialenumerator_osx.cpp
        )
    else(APPLE)
        set( QEXTSERIALPORT_SRCS
            ${QEXTSERIALPORT_SRCS}
            qextserialenumerator_unix.cpp
        )
        set( ADDITIONAL_LIBRARY udev )
    endif(APPLE)
else()
    message( FATAL_ERROR "This operating system is not supported by the QextSerialPort library" )
endif()

set ( QEXTSERIALPORT_MOC_HEADERS
        qextserialport.h
        qextserialenumerator.h
)

qt4_wrap_cpp( QEXTSERIALPORT_SRCS ${QEXTSERIALPORT_MOC_HEADERS} )

if(BUILD_SHARED)
    add_library( ${LIB_TARGET} SHARED ${QEXTSERIALPORT_SRCS} )
else()
    add_library( ${LIB_TARGET} STATIC ${QEXTSERIALPORT_SRCS} )
endif()

target_link_libraries( ${LIB_TARGET} ${QT_LIBRARIES} ${ADDITIONAL_LIBRARY} )
