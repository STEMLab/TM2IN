cmake_minimum_required(VERSION 3.9)
project(TriangleMeshToClearSolid)

set(CMAKE_CXX_STANDARD 11)

include_directories(../TriangleMeshToClearSolid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(CGAL REQUIRED )

if ( CGAL_FOUND )
    include( ${CGAL_USE_FILE} )
    include( CGAL_CreateSingleSourceCGALProgram )
endif()

FIND_PACKAGE( Boost 1.40 COMPONENTS filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
INCLUDE_DIRECTORIES ( /usr/local/include/eigen3 )

add_executable(TriangleMeshToClearSolid
        features/Space.cpp
        features/Space.h
        features/Surface.cpp
        features/Surface.hpp
        features/SurfaceGraph.cpp
        features/SurfaceGraph.h
        features/Triangle.cpp
        features/Triangle.h
        features/TriangleMesh.h
        features/TriangleMeshGraph.h
        features/Vertex.cpp
        features/Vertex.h
        features/HalfEdge.cpp
        features/HalfEdge.h
        fileio/3DSImporter.cpp
        fileio/3DSImporter.h
        fileio/GenerationWriter.cpp
        fileio/GenerationWriter.h
        fileio/Importer.h
        fileio/JSONMaker.cpp
        fileio/JSONMaker.h
        fileio/TVRImporter.cpp
        fileio/TVRImporter.h
        cgal/CGALCalculation.cpp
        cgal/CGALCalculation.h
        logic/check.cpp
        logic/check.hpp
        compute/SurfacePairComputation.cpp
        compute/SurfacePairComputation.h
        compute/SurfacesListComputation.cpp
        compute/SurfacesListComputation.h
        manage/ClusteringRoomMaker.cpp
        manage/ClusteringRoomMaker.h
        manage/MergingRoomMaker.cpp
        manage/MergingRoomMaker.h
        manage/RoomMaker.cpp
        manage/RoomMaker.h
        space_maker/OnlyWallSpaceMaker.cpp
        space_maker/OnlyWallSpaceMaker.h
        predefine.h
        TVRConverter.cpp
        util.cpp
        util.h cgal/SurfaceHoleCover.cpp cgal/SurfaceHoleCover.h cgal/Types.h compute/VertexListComputation.cpp compute/VertexListComputation.h compute/SurfaceComputation.cpp compute/SurfaceComputation.h compute/VertexComputation.cpp compute/VertexComputation.h)


TARGET_LINK_LIBRARIES( TriangleMeshToClearSolid LINK_PUBLIC ${Boost_LIBRARIES} )

