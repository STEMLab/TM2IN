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
        "polygonizer/Polygonizer.h"
        "polygonizer/PCAPolygonizer.cpp"
        "polygonizer/PCAPolygonizer.h"
        "polygonizer/TrianglePolygonizer.h"
        "polygonizer/TrianglePolygonizer.cpp"
        "polygonizer/DividedPolygonizer.cpp"
        "polygonizer/DividedPolygonizer.h"
        predefine.h
        util.cpp
        util.h
        )

add_executable(develop1
        ${SOURCE_FILES}
        ${DETAIL_FILES}
        ${LIB_FILES}
        main.cpp)

TARGET_LINK_LIBRARIES(develop1 LINK_PUBLIC ${Boost_LIBRARIES} Eigen3::Eigen ${SFCGAL_LIBRARIES})

include_directories(
        ../TriangleMeshToClearSolid
        lib)

