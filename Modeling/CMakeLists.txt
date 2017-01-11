cmake_minimum_required(VERSION 3.5)

add_executable(Hanoi_models
main.cpp
)

set(tarver "x86")
set(CMAKE_CXX_FLAGS "-std=c++11")

if(UNIX)
	set(libdir "/usr/lib/")
	find_library(OSG_LIB NAME osg HINTS ${libdir})
	find_library(OSGviewer_LIB NAME osgViewer HINTS ${libdir})
	find_library(OSGdb_LIB NAME osgDB HINTS ${libdir})
endif()
if(WIN32)
	set(libdir "../OSGPack/lib/${tarver}")
	include_directories(OSGPack/include)
	find_library(OSG_LIB NAME osg.lib HINTS ${libdir})
	find_library(OSGviewer_LIB NAME osgViewer.lib HINTS ${libdir})
	find_library(OSGdb_LIB NAME osgDB.lib HINTS ${libdir})
endif()

target_link_libraries(Hanoi_models PUBLIC ${OSG_LIB})
target_link_libraries(Hanoi_models PUBLIC ${OSGviewer_LIB})
target_link_libraries(Hanoi_models PUBLIC ${OSGdb_LIB})
