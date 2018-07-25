//
// Created by dongmin on 18. 1. 18.
//

#include <detail/cgal/plane.h>
#include <algorithm/collinear.h>
#include "unused.h"
#include "features/TriangleMeshGraph.h"

#define GAP_FOR_SNAP 3


using namespace std;

namespace TM2IN{
    namespace unused{
        void removeStraight(Surface*& pSurface){
            if (pSurface->getVerticesSize() < 3) return;
            int removed_count = 0;

            vector<Vertex*> vertexList = pSurface->getVerticesList();
            for (ll i = 0 ; i < vertexList.size(); ){
                if (vertexList.size() < 3) return;
                ll firstIndex = i == 0? vertexList.size() - 1: i-1;
                Vertex* start_p = vertexList[firstIndex];
                ll secondIndex = i;
                Vertex* check_p = vertexList[secondIndex];
                ll thirdIndex = i + 1 == vertexList.size()? 0 : i+1;
                Vertex* end_p = vertexList[thirdIndex];
                if (TM2IN::algorithm::isCollinear(start_p, check_p, end_p)){
                    vertexList.erase(vertexList.begin() + i);
                    removed_count++;
                }
                else{
                    i++;
                }
            }

            pSurface->setVertexList(vertexList);
            pSurface->updateMBB();
            if (removed_count) cout << removed_count << " are removed in straight" << endl;
        }


        std::vector<Segment_2> makeSegment2List(Surface *&pSurface, Plane_3 plane3) {
            vector<Point_2> pointsList = TM2IN::detail::cgal::project_to_plane(pSurface->getVerticesList(), plane3);
            vector<Segment_2> segmentsList;
            for (int i = 0 ; i < pointsList.size() - 1 ; i++){
                Segment_2 seg(pointsList[i], pointsList[i+1]);
                segmentsList.push_back(seg);
            }
            Segment_2 last_seg(pointsList[pointsList.size() - 1], pointsList[0]);
            segmentsList.push_back(last_seg);
            return segmentsList;
        }

    }
}

/*
 *
std::vector<Surface *> resolveSelfIntersection(Surface * &pSurface) {
    int rec = 0;
    vector<Surface*> newSurfaceList;

    int number = pSurface->getVerticesSize();
    // SurfaceIntersection::resolveEasySelfIntersection(pSurface);
    // SurfaceComputation::removeStraight(pSurface);

    while (true){
        int result = makeNewIntersectionVertex(pSurface);
        if (result == 1) {
            break;
        }
        if (newSurfaceList.size() == 0){
            newSurfaceList.push_back(pSurface);
            break;
        }

        if (result == 2){
            cout << "Special case " << endl;
            break;
        }
        rec ++;
        if (rec % 20 == 0) cout << "rec : " << rec << endl;
        if (rec > number ) {
            cout << "Too much recursive? " << endl;
            break;
        }

        SurfaceIntersection::resolveEasySelfIntersection(pSurface);
        SurfaceComputation::removeConsecutiveDuplicationIndex(pSurface);
    }
    SurfaceIntersection::resolveEasySelfIntersection(pSurface);

    return newSurfaceList;
}


//
// @param pSurface
// @return
// 0 : made New Vertex.
// 1 : no special Intersection
// 2 : special case.
//

int makeNewIntersectionVertex(Surface *&pSurface){
    double threshold = 0.0;
    Plane_3 pl = TM2IN::detail::cgal::make_PCA_plane(pSurface->getVerticesList(), pSurface->normal);
    vector<Segment_2> segmentList = SurfaceComputation::makeSegment2List(pSurface, pl);
    // Intersection Point
    for (int i = 0 ; i < segmentList.size() - 2; i++){
        for (int j = i + 2 ; j < segmentList.size() ; j++){
            CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
                    result = CGAL::intersection(segmentList[i], segmentList[j]);
            if (result){
                if (const Point_2* p = boost::get<Point_2>(&*result)){
                    if (i == 0 && j == segmentList.size() - 1){
                        int pj = 0;
                        for (;pj < 2 ; pj++){
                            double distJ = CGAL::squared_distance(*p, segmentList[j][pj]);
                            if (distJ < threshold){
                                break;
                            }
                        }
                        if (pj == 2){
                            Point_3 point3 = pSurface->getPlaneRef().to_3d(*p);
                            Vertex* vt = new Vertex(point3.x(), point3.y(), point3.z());
                            pSurface->setVertex(0, vt);
                            int insertIndex = j + 1;
                            pSurface->insertVertex(insertIndex, vt);
                            return 0;
                        } else
                            continue;
                    }
                    int pi = 0;
                    Point_3 point3 = pSurface->getPlaneRef().to_3d(*p);
                    Vertex* vt = new Vertex(point3.x(), point3.y(), point3.z());
                    for (;pi < 2 ; pi++){
                        double distI = CGAL::squared_distance(*p, segmentList[i][pi]);
                        if (distI < threshold){
                            break;
                        }
                    }
                    int pj = 0;
                    for (;pj < 2 ; pj++){
                        double distJ = CGAL::squared_distance(*p, segmentList[j][pj]);
                        if (distJ < threshold){
                            break;
                        }
                    }

                    int pointI = pi + i == segmentList.size() ? 0 : pi + i;
                    int pointJ = pj + j == segmentList.size() ? 0 : pj + j;

                    if (pi != 2 && pj != 2){
                        if (pSurface->vertex(pointI) != pSurface->vertex(pointJ)){
                            pSurface->setVertex(pointI, vt);
                            pSurface->setVertex(pointJ, vt);
                        }
                        continue;
                    }

                    // put new Vertex. i < j 이므로 j 부터 넣기.
                    if (pj == 2){
                        pSurface->setVertex(pointI, vt);
                        int insertIndex = j + 1;
                        pSurface->insertVertex(insertIndex, vt);
                    }

                    if (pi == 2){ // put Vertex
                        pSurface->setVertex(pointJ, vt);
                        int insertIndex = i + 1;
                        pSurface->insertVertex(insertIndex, vt);
                    }

                    if (pi == 2 || pj == 2){
                        return 0;
                    }

                }
                else if (const Segment_2* seg = boost::get<Segment_2>(&*result)){
                    cout << "i : " << i << " , j : " << j << endl;
                    cout << *seg << endl;
                    pSurface->removeVertexByIndex(i);
                    return 0;

                    Point_3 source = pSurface->getPlaneRef().to_3d(seg->source());
                    Point_3 target = pSurface->getPlaneRef().to_3d(seg->target());

                    // j
                    int sourceJ = 0, targetJ = 0;
                    for (; sourceJ < 2 ; sourceJ++){
                        double distJ = CGAL::squared_distance(seg->vertex(0), segmentList[j][sourceJ]);
                        if (distJ < threshold)
                            break;
                    }

                    for (; targetJ < 2; targetJ++){
                        double distJ = CGAL::squared_distance(seg->vertex(0), segmentList[j][targetJ]);
                        if (distJ < threshold)
                            break;
                    }

                    // i
                    int sourceI = 0, targetI = 0;
                    for (; sourceI < 2 ; sourceI++){
                        double distJ = CGAL::squared_distance(seg->vertex(0), segmentList[j][sourceI]);
                        if (distJ < threshold)
                            break;
                    }

                    for (; targetI < 2; targetI++){
                        double distJ = CGAL::squared_distance(seg->vertex(0), segmentList[j][targetI]);
                        if (distJ < threshold)
                            break;
                    }

                    int indexSourceI = sourceI + j == segmentList.size() ? 0 : sourceI + j;
                    int indexTargetI = targetI + j == segmentList.size() ? 0 : targetI + j;
                    int indexSourceJ = sourceJ + j == segmentList.size() ? 0 : sourceJ + j;
                    int indexTargetJ = targetJ + j == segmentList.size() ? 0 : targetJ + j;

                    if (sourceI == 1) return 2;
                    if (sourceJ == 0) return 2;
                    if (targetI == 0) return 2;
                    if (targetJ == 1) return 2;

                    CGAL_assertion(sourceI == 2 || sourceI == 0);
                    CGAL_assertion(sourceJ == 2 || sourceJ == 1);
                    CGAL_assertion(targetI == 2 || targetI == 1);
                    CGAL_assertion(targetJ == 2 || targetJ == 0);

                    bool sourceVertexOfIntersectionPointExist = (sourceJ != 2 && sourceI != 2);
                    bool targetVertexOfIntersectionPointExist = (targetJ != 2 && targetI != 2);
                    if (sourceVertexOfIntersectionPointExist){
                        if (pSurface->vertex(indexSourceI) != pSurface->vertex(indexSourceJ)){
                            pSurface->setVertex(indexSourceI, pSurface->vertex(indexSourceJ));
                        }
                    }

                    if (targetVertexOfIntersectionPointExist){
                        if (pSurface->vertex(indexTargetI) != pSurface->vertex(indexTargetJ)){
                            pSurface->setVertex(indexTargetI, pSurface->vertex(indexTargetJ));
                        }
                    }

                    if (sourceVertexOfIntersectionPointExist && targetVertexOfIntersectionPointExist){
                        CGAL_assertion(sourceJ != targetJ);
                        CGAL_assertion(sourceI != targetI);
                        continue;
                    }

                    // j
                    if (targetJ == 2 || sourceJ == 2){
                        if (targetJ == 2 && sourceJ == 2){
                            pSurface->insertVertex(j + 1, pSurface->vertex(indexSourceI));
                            pSurface->insertVertex(j + 1, pSurface->vertex(indexTargetI));
                        }
                        else if (targetJ == 2){
                            pSurface->insertVertex(j + 1, pSurface->vertex(indexTargetI));
                        }
                        else if (sourceJ == 2){
                            pSurface->insertVertex(j + 1, pSurface->vertex(indexSourceI));
                        }
                        return 0;
                    }

                    // i
                    if (targetI == 2 || sourceI == 2){
                        if (targetI == 2 && sourceI == 2){
                            pSurface->insertVertex(i + 1, pSurface->vertex(indexTargetJ));
                            pSurface->insertVertex(i+1, pSurface->vertex(indexSourceJ));
                        }
                        else if (targetI == 2){
                            pSurface->insertVertex(i + 1, pSurface->vertex(indexTargetJ));
                        }
                        else if (sourceI == 2){
                            pSurface->insertVertex(i + 1, pSurface->vertex(indexSourceJ));
                        }
                        return 0;
                    }

                }
            }
        }
    }
    return 1;
}

void resolveEasySelfIntersection(Surface *&pSurface) {
    double threshold = 0.000001;

    vector<Point_2> pointsList = TM2IN::detail::cgal::project_to_plane(pSurface->getVerticesList(), pSurface->getPlaneRef());
    for (int i = 0 ; i < pointsList.size() - 1; i++){
        for (int j = 2 ; j <= GAP_FOR_SNAP ; j++){
            int nextIndex = i + j >= pointsList.size() ? i + j - pointsList.size() : i + j;
            Point_2 next = pointsList[nextIndex];
            if (CGAL::squared_distance(next, pointsList[i]) <= threshold){
                if (nextIndex < i){
                    pointsList.erase(pointsList.begin() + i + 1, pointsList.end());
                    pointsList.erase(pointsList.begin(), pointsList.begin() + nextIndex + 1);

                    pSurface->removeVertexByIndex(i + 1, pSurface->getVerticesSize());
                    pSurface->removeVertexByIndex(0, nextIndex + 1);
                }
                else{
                    pointsList.erase(pointsList.begin() + i + 1, pointsList.begin() + nextIndex + 1);
                    pSurface->removeVertexByIndex(i + 1, nextIndex + 1);
                }
                break;
            }
        }
    }

}

 */



