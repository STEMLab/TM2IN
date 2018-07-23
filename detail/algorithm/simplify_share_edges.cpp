//
// Created by dongmin on 18. 7. 20.
//

#include <detail/feature/polygon.h>
#include <detail/feature/plane.h>
#include "simplify_share_edges.h"
#include "surface_neighbor.h"


namespace TM2IN {
    namespace detail {
        namespace algorithm {
            /**
             * Simplification for share edges after merging
             *
             * @todo re-factoring
             * @return 0 : succeed for simplification, 1 : failure
             */
            int simplify_share_edges(Surface *origin, Surface *piece) {
                ll piece_middle = -1, origin_middle = -1;
                ll piece_size = piece->getVerticesSize();
                ll origin_size = origin->getVerticesSize();

                vector < Vertex * > piece_vertex_list = piece->getVerticesList();
                vector < Vertex * > origin_vertex_list = origin->getVerticesList();

                for (ll i = 0; i < piece_size; i++) {
                    for (ll j = origin_size - 1; j >= 0; j--) {
                        if (piece_vertex_list[i] == origin_vertex_list[j]) {
                            ll next_i = i + 1 == piece_size ? 0 : i + 1;
                            ll next_j = j - 1 == -1 ? origin_size - 1 : j - 1;

                            ll pre_i = i - 1 == -1 ? piece_size - 1 : i - 1;
                            ll pre_j = j + 1 == origin_size ? 0 : j + 1;

                            if (piece_vertex_list[next_i] == origin_vertex_list[next_j]
                                && piece_vertex_list[pre_i] == origin_vertex_list[pre_j]) {
                                piece_middle = i;
                                origin_middle = j;
                                break;
                            }
                        }
                    }
                    if (piece_middle != -1) break;
                }

                if (piece_middle == -1) return 1;

                ll lastVertex_piece = -1, lastVertex_origin = -1;
                ll firstVertex_piece = -1, firstVertex_origin = -1;
                if (findStartAndEnd(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle,
                                    firstVertex_piece, lastVertex_piece, firstVertex_origin, lastVertex_origin))
                    return 1;

                assert(origin->getSegmentsNumber(firstVertex_origin, lastVertex_origin) > 1);
                assert(piece->getSegmentsNumber(firstVertex_piece, lastVertex_piece) > 1);
                assert(piece->vertex(firstVertex_piece) == origin->vertex(lastVertex_origin));
                assert(piece->vertex(lastVertex_piece) == origin->vertex(firstVertex_origin));

                HalfEdge *newEdge_piece = new HalfEdge(piece->vertex(firstVertex_piece),
                                                       piece->vertex(lastVertex_piece), piece);
                HalfEdge *newEdge_origin = new HalfEdge(origin->vertex(firstVertex_origin),
                                                        origin->vertex(lastVertex_origin), origin);
                newEdge_origin->setOppositeEdge(newEdge_piece);
                newEdge_piece->setOppositeEdge(newEdge_origin);

                vector < HalfEdge * > newHalfEdgeList_piece;
                vector < HalfEdge * > newHalfEdgeList_origin;

                for (ll j = lastVertex_origin;;) {
                    newHalfEdgeList_origin.push_back(origin->boundary_edges(j));
                    j++;
                    if (j == origin_size) j = 0;
                    if (j == firstVertex_origin) break;
                }
                newHalfEdgeList_origin.push_back(newEdge_origin);

                for (ll j = lastVertex_piece;;) {
                    newHalfEdgeList_piece.push_back(piece->boundary_edges(j));
                    j++;
                    if (j == piece_size) j = 0;
                    if (j == firstVertex_piece) break;
                }
                newHalfEdgeList_piece.push_back(newEdge_piece);

                // check polygon after simplification
                Surface* pSurface = new Surface();
                pSurface->setExteriorBoundary(newHalfEdgeList_piece);
                Plane_3 planeRef = TM2IN::detail::feature::make_simple_plane(piece->normal);
                vector<Point_2> point2dList = TM2IN::detail::feature::project_to_plane(pSurface->getVerticesList(), planeRef);
                Polygon_2 polygon = TM2IN::detail::feature::make_CGAL_polygon(point2dList);
                if (!polygon.is_simple() || polygon.orientation() == -1) {
                    cerr << "cannot be simplified" << endl;
                    return 1;
                }

                origin->exteriorBoundary.clear();
                piece->exteriorBoundary.clear();

                origin->setExteriorBoundary(newHalfEdgeList_origin);
                piece->setExteriorBoundary(newHalfEdgeList_piece);

                return 0;
            }
        }
    }
}