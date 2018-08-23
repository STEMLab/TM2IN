/**
 *
 * @mainpage TM2IN
 *
 * @section intro_sec Introduction
 *
 * TM2IN generates appropriate Solid geometry for Indoor Map from Triangle Mesh data such as TVR, COLLADA, 3DS.
 *
 *
 * @section dependencies_sec Dependencies
 *
- CGAL (http://www.cgal.org/)
- SFCGAL (http://www.sfcgal.org)
- Eigen3 (http://eigen.tuxfamily.org/index.php?title=Main_Page)
- Boost (http://www.boost.org/)
- rapidxml (http://rapidxml.sourceforge.net)
 *
 * @section geometric_features_sec Geometric Indoor Features
 *
 * @link geo_features Geometric Indoor Features @endlink
 *
 * All geometric features in TM2IN are TM2IN::IndoorComponent.
 *
 * - TM2IN::IndoorComponent
 *
 * Each TM2IN::IndoorComponent objects have TM2IN::MinimumBoundingBox.
 *
 * - TM2IN::MinimumBoundingBox
 *
 * Basic geometric features are TM2IN::Vertex and TM2IN::HalfEdge.
 *
 *  - TM2IN::Vertex
 *  - TM2IN::HalfEdge
 *
 * A list of HalfEdge constructs TM2IN::Wall.
 *
 *  - TM2IN::Wall::Surface
 *  - TM2IN::Wall::Triangle
 *  - TM2IN::Wall::TriangulatedSurface
 *  - TM2IN::Wall::Polygon
 *
 * A set of TM2IN::Wall constructs TM2IN::RoomBoundary.
 *
 *  - TM2IN::RoomBoundary::PolygonMesh
 *  - TM2IN::RoomBoundary::TriangleMesh
 *  - TM2IN::RoomBoundary::TriangulatedSurfaceMesh
 *
 * A TM2IN::RoomBoundary constructs TM2IN::Room.
 *
 *  - TM2IN::Room
 *
 * @section api_sec API
 *
 * There are several sets of functions separated by user level.
 *
 * Just for users who wants to use converter, see @link Converter Converter. @endlink
 *
 * Functions for light users are posted in @link public_api Public API @endlink
 *
 * Implementation details are posted in @link imp_details Details @endlink
 *
 * @section released Released
 *
 * Now released version of TM2IN.cpp and Converter.cpp is 1.0.0
 *
 * @defgroup public_api Public API
 *
 * @defgroup imp_details Details
 *
 * @defgroup geo_features Geometric Indoor Features
 *
 * @defgroup unused Unused
 *
 */