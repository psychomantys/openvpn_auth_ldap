cmake_minimum_required(VERSION 2.8)
PROJECT(openvpn_auth_ldap)

#The version number.
set(OPAL_VERSION_MAJOR 0)
set(OPAL_VERSION_MINOR 0)
set(OPAL_VERSION_MINOR_FIX 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/src/config.h.in"
	"${PROJECT_BINARY_DIR}/src/config.h"
)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

INCLUDE_DIRECTORIES(
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/src
)

FILE(GLOB OPAL_SOURCES 
	"src/*.cpp"
	"src/*.c"
)

set(Boost_USE_STATIC_LIBS	ON)
set(Boost_USE_MULTITHREADED	ON)
# search for Boost version 1.49.0
find_package( Boost "1.49" COMPONENTS propertytree )
find_package( Ldap ) 

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

add_library(opal STATIC
	${OPAL_SOURCES}
)
target_link_libraries(opal ${Boost_LIBRARIES})

if(UNIX)
	add_custom_target(distclean @echo cleaning for source distribution)
	add_custom_command(
		COMMENT	"distribution clean"
		DEPENDS	clean
		COMMAND	sh
		ARGS		-c '(for x in cmake.depends cmake_install.cmake cmake.check_depends CMakeCache.txt cmake.check_cache Makefile gmon.out '*~' CMakeTmp .backups CMakeFiles config.h \; do find . -iname \"$$x\" \; done ) | xargs -n1 rm -rf'
		TARGET  distclean
	)
ENDIF(UNIX)

