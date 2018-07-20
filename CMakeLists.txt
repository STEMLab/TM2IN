cmake_minimum_required(VERSION 3.9)
project(TM2IN)

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules;${CMAKE_MODULE_PATH}" )

find_package(CGAL REQUIRED )

if ( CGAL_FOUND )
    include( ${CGAL_USE_FILE} )
    include( CGAL_CreateSingleSourceCGALProgram )
endif()
if ( NOT CGAL_FOUND )
    message(STATUS "This project requires the CGAL library, and will not be compiled.")
    return()
endif()

FIND_PACKAGE( Boost 1.40 REQUIRED COMPONENTS filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(SFCGAL REQUIRED )

file(GLOB LIB_FILES
        "lib/rapidxml/*.hpp"
        )

file(GLOB DETAIL_FILES
        "detail/io/*.h"
        "detail/io/*.cpp"
        "detail/feature/*.h"
        "detail/feature/*.cpp"
        "detail/cgal_config.h"
        "detail/algorithm/*.h"
        "detail/algorithm/*.cpp"
        )

file(GLOB SOURCE_FILES
        "features/*.h"
        "features/*.cpp"
        "io/*.h"
        "io/*.cpp"
        "cgal/*.h"
        "cgal/*.cpp"
        "logic/*.h"
        "logic/*.cpp"
        "compute/*.h"
        "compute/*.cpp"
        "converter/*.h"
        "converter/*.cpp"
        "algorithm/*.h"
        "algorithm/*.cpp"
        predefine.h
        util.cpp
        util.h
        )

add_executable(TM2IN
        ${SOURCE_FILES}
        ${DETAIL_FILES}
        ${LIB_FILES}
        Options.h
        Options.cpp
        TM2IN.cpp)

TARGET_LINK_LIBRARIES(TM2IN LINK_PUBLIC ${Boost_LIBRARIES} Eigen3::Eigen ${SFCGAL_LIBRARIES})

include_directories(
        ../TriangleMeshToClearSolid
        lib)

