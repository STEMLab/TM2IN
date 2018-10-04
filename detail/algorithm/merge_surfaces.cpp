//
// Created by dongmin on 18. 7. 19.
//

#include "merge_surfaces.h"

#include "features/Wall/TriangulatedSurface.h"
#include <detail/cgal/plane.h>
#include <cgal/vector_angle.h>
#include <detail/features/halfedge_string.h>
#include <Options.h>
#include <io/GenerationWriter.h>

#include "detail/algorithm/surface_neighbor.h"
#include "detail/cgal/polygon.h"
#include "detail/cgal/geometry.h"

namespace TM2IN {
    namespace detail {
        namespace algorithm {

            SurfaceMerger::SurfaceMerger(double t1, double t2) : thres1(t1), thres2(t2){}

            bool SurfaceMerger::mergeSurfaces(vector<Wall::TriangulatedSurface *> surfaceList, vector<Wall::TriangulatedSurface *> &result) {
                result.clear();

                //deep copy
                for (ull i = 0 ; i < surfaceList.size() ; i++){
                    result.push_back(new TriangulatedSurface(surfaceList[i]));
                }

                bool hasMerged = false;
                bool isMerged = true;
                ull combined_count = 0;
                while (isMerged){
                    if (result.size() == 1) break;
                    sort(result.begin(), result.end(), Surface::compareLength);

                    map<string, bool> removed_map;
                    for (ull i = 0 ; i < result.size() ; i++){
                        removed_map.emplace(result[i]->geom_id, false);
                    }

                    for (ull i = 0 ; i < result.size() - 1 ; i++){
                        if (removed_map[result[i]->geom_id]) continue;
                        isMerged = false;

                        vector<TriangulatedSurface*> neighbors = result[i]->getNeighborList();

                        for (int j = 0 ; j < neighbors.size() ; j++){
                            if (removed_map[neighbors[j]->geom_id]) continue;
                            if (merge(result[i], neighbors[j]) == 0){
                                combined_count++;
                                isMerged = true;
                                hasMerged = true;
                                removed_map[neighbors[j]->geom_id] = true;
                                if (Options::getInstance()->generator && combined_count != 0 && combined_count % 500 == 0){
                                    Options::getInstance()->generation++;
                                    vector<TriangulatedSurface*> temp;
                                    for (ull ti = result.size() - 1 ; ti >= 0 ; ti--) {
                                        if (!removed_map[result[ti]->geom_id]){
                                            temp.push_back(result[ti]);
                                        }
                                    }
                                    TM2IN::io::GenerationWriter::getInstance()->write(temp);
                                    temp.clear();
                                }
                            }
                        }

                        if (isMerged) i -= 1;
                        printProcess(combined_count, surfaceList.size(), "mergeSurface");
                    }

                    ull i = 0;
                    while (i < result.size()){
                        if (removed_map[result[i]->geom_id]){
                            result.erase(result.begin() + i);
                        } else
                            i++;
                    }
                    assert(result.size() >= 1);
                    removed_map.clear();
                }

                /*
                 *
                while (isMerged){
                    sort(result.begin(), result.end(), Surface::compareLength);
                    if (result.size() == 1) break;
                    for (ull i = 0 ; i < result.size() - 1 ; i++){
                        ull j = i + 1;
                        isMerged = false;
                        while (j < result.size()){
                            if (merge(result[i], result[j]) == 0)
                            {
                                cout << ".";
                                combined_count++;
                                isMerged = true;
                                hasMerged = true;
                                result.erase(result.begin() + j);
                                if (Options::getInstance()->generator && combined_count != 0 && combined_count % 500 == 0){
                                    Options::getInstance()->generation++;
                                    TM2IN::io::GenerationWriter::getInstance()->write(result);
                                }
                            } else
                                j++;
                        }
                        if (isMerged) i -= 1;
                        printProcess(combined_count, surfaceList.size(), "mergeSurface");
                    }
                }
                 */
                return hasMerged;
            }


            int SurfaceMerger::merge(Wall::TriangulatedSurface *origin, Wall::TriangulatedSurface  *piece) {
                // check Polygon is in near polygon or not
                if (!TM2IN::detail::cgal::has_bbox_intersect(origin, piece)) return 1;

                // check They are neighbor
                if (!isNeighbor(origin, piece)) return 1;
                if (TM2IN::cgal::getAngle(origin->normal, piece->normal) > 179.999999){
                    return 1;
                }

                ll origin_size = origin->getVerticesSize();
                ll piece_size = piece->getVerticesSize();

                neighbor_info ni;
                if (constructNeighborInfo(piece, origin, ni)){
                    cerr << "\n" << origin->asJsonText() << endl;
                    cerr << "\n" << piece->asJsonText() <<endl;
                    cerr << TM2IN::cgal::getAngle(origin->normal, piece->normal)  << endl;
                    return 1;
                }

                /*
                ll piece_middle = -1, origin_middle = -1;
                if (!findShareVertex(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle)) return 1;
                ll lastVertex_piece = -1, lastVertex_origin = -1;
                ll firstVertex_piece = -1, firstVertex_origin = -1;
                if (findStartAndEnd(piece_vertex_list, origin_vertex_list, piece_middle, origin_middle, firstVertex_piece, lastVertex_piece, firstVertex_origin, lastVertex_origin)){
                    cerr << "\n" << origin->asJsonText() << endl;
                    cerr << "\n" << piece->asJsonText() <<endl;
                    cerr << TM2IN::cgal::getAngle(origin->normal, piece->normal)  << endl;
                    return 1;
                }
                */

                assert (piece->vertex(ni.firstVertex_piece) == origin->vertex(ni.lastVertex_origin));
                assert (piece->vertex(ni.lastVertex_piece) == origin->vertex(ni.firstVertex_origin));

                int seg_num = piece->getSegmentsNumber(ni.lastVertex_piece, ni.firstVertex_piece);

                if (seg_num == -1)
                {
                    cerr << "segment Number is -1" << endl;
                    exit(-1);
                }
                else if (seg_num == 0){
                    return 1;
                }
                else if (seg_num == 1){

                }
                else{
                    if (!check_merge_condition(origin->normal, piece->normal)) {
                        return 1;
                    }
                }

                vector<HalfEdge*> new_edges;

                for (ll j = ni.lastVertex_origin; ; ){
                    new_edges.push_back(origin->exterior_boundary_edge(j));
                    j++;
                    if (j == origin_size) j = 0;
                    if (j == ni.firstVertex_origin) break;
                }

                for (ll i = ni.lastVertex_piece; ;){
                    new_edges.push_back(piece->exterior_boundary_edge(i));
                    i++;
                    if (i == piece_size) i = 0;
                    if (i == ni.firstVertex_piece) break;
                }

                if (merging_invalid_test(new_edges, origin->normal + piece->normal)) return 1;


                TM2IN::detail::HalfEdgeString::setParent(new_edges, origin);

                origin->setExteriorBoundary(new_edges);
                origin->mergeMBB(piece);
                origin->normal = origin->normal + piece->normal;
                origin->setArea(origin->getArea() + piece->getArea());
                origin->triangles.insert(origin->triangles.end(), piece->triangles.begin(), piece->triangles.end());

                return 0;
            }

            bool SurfaceMerger::check_merge_condition(Vector_3 &big, Vector_3 &small) {
                Vector_3 added = big + small;
                if (big.squared_length() == 0 || small.squared_length() == 0){
                    if (Options::getInstance()->do_validation){
                        throw std::runtime_error("Cannot check merge condition");
                    }
                    else{
                        return true;
                    }
                }

                if (is_coplanar(big, small)){
                    double addedAngle = TM2IN::cgal::getAngle(added, big);
                    return addedAngle <= thres2;
                }
                return false;
            }

            /**
             * check two normal vector are co-planar by using threshold 1
             *
             * @return is it co-planar
             */
            bool SurfaceMerger::is_coplanar(Vector_3 &big, Vector_3 &small) {
                double angle = TM2IN::cgal::getAngle(big, small);
                return angle <= thres1;
            }

            bool merging_invalid_test(vector<HalfEdge *> new_edges, Vector_3 newNormal){
                Wall::TriangulatedSurface * pSurface = new Wall::TriangulatedSurface();
                pSurface->setExteriorBoundary(new_edges);
                if (!pSurface->easy_validation()) return 1;
                Plane_3 planeRef = TM2IN::detail::cgal::make_simple_plane(newNormal);
                vector<Point_2> point2dList = TM2IN::detail::cgal::project_to_plane(pSurface->getVerticesList(), planeRef);
                Polygon_2 polygon = TM2IN::detail::cgal::make_CGAL_polygon(point2dList);
                if (!polygon.is_simple() || polygon.orientation() == -1){
                    return 1;
                }
                else{
                    delete pSurface;
                }

                return 0;
            }


        } // algorithm
    } // detail
}