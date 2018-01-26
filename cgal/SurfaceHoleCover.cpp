//
// Created by dongmin on 18. 1. 17.
//

#include "SurfaceHoleCover.h"
#include <algorithm>
#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "features/Surface.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel2;
typedef CGAL::Polyhedron_3<Kernel2>         Polyhedron;

typedef Polyhedron::Halfedge_handle    Halfedge_handle;
typedef Polyhedron::Facet_handle       Facet_handle;
typedef Polyhedron::Vertex_handle      Vertex_handle;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;

typedef Polyhedron::Facet_iterator         Facet_iterator;
typedef Polyhedron::Vertex_iterator        Vertex_iterator;

typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;

// A modifier creating a triangle with the incremental builder.
template<class HDS>
class polyhedron_builder : public CGAL::Modifier_base<HDS> {
public:
    vector<Vertex*> &coords;
    vector<Surface*> &surfaces;
    polyhedron_builder(vector<Vertex*> & _vertices, vector<Surface*> & _surfaces ) : coords(_vertices), surfaces(_surfaces) {}
    void operator()( HDS& hds) {
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;

        // create a cgal incremental builder
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        unsigned int triangleSize = 0;
        for (int i = 0 ; i < surfaces.size() ; i++){
            triangleSize += surfaces[i]->triangulation.size();
        }
        B.begin_surface( coords.size(), triangleSize);
        cout << coords.size() << " , " << triangleSize << endl;
        // add the polyhedron vertices
        for( int i=0; i<(int)coords.size(); i++ ){
            B.add_vertex( Point( coords[i]->coords[0], coords[i]->coords[1], coords[i]->coords[2] ) );
        }

        // add the polyhedron triangles
        for( int i=0; i<(int)surfaces.size(); i++ ){
            cout << "FACET :  " << i <<  " , " << surfaces[i]->triangulation.size() << endl;
            for ( int tri = 0 ; tri < (int)surfaces[i]->triangulation.size() ; tri++){
                B.begin_facet();
                CGAL_assertion(surfaces[i]->triangulation[tri].size() == 3);
                for (int vt = 0 ; vt < 3; vt++){
                    int index = surfaces[i]->triangulation[tri][vt];
                    B.add_vertex_to_facet(index);
                }
                B.end_facet();
            }
        }

        // finish up the surface
        B.end_surface();
    }
};

vector<Vertex *> SurfaceHoleCover::fillHole (vector<Vertex*>& vertices, vector<Surface *>& surfaces) {
    Polyhedron poly;
    polyhedron_builder<HalfedgeDS> polybuilder (vertices, surfaces);
   poly.delegate(polybuilder);
    // Incrementally fill the holes
    unsigned int nb_holes = 0;
    BOOST_FOREACH(Halfedge_handle h, halfedges(poly))
                {
                    if(h->is_border())
                    {
                        std::vector<Facet_handle>  patch_facets;
                        std::vector<Vertex_handle> patch_vertices;
                        bool success = CGAL::cpp11::get<0>(
                                CGAL::Polygon_mesh_processing::triangulate_refine_and_fair_hole(
                                        poly,
                                        h,
                                        std::back_inserter(patch_facets),
                                        std::back_inserter(patch_vertices),
                                        CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, poly)).
                                                geom_traits(Kernel2())) );
                        std::cout << " Number of facets in constructed patch: " << patch_facets.size() << std::endl;
                        std::cout << " Number of vertices in constructed patch: " << patch_vertices.size() << std::endl;
                        std::cout << " Fairing : " << (success ? "succeeded" : "failed") << std::endl;
                        ++nb_holes;
                    }
                }
    std::cout << std::endl;
    std::cout << nb_holes << " holes have been filled" << std::endl;

    vector<Surface*> newSurface;
    vector<Vertex*> newVertices;
    for(Vertex_iterator i = poly.vertices_begin(); i!= poly.vertices_end() ; ++i){
        Vertex* vt = new Vertex(i->point().x(),i->point().y(),i->point().z());
        newVertices.push_back(vt);
    }

    for (Facet_iterator i = poly.facets_begin(); i != poly.facets_end(); i++){
        Halfedge_facet_circulator j = i->facet_begin();

        vector<Vertex*> oneSurfaceCoords;
        CGAL_assertion( CGAL::circulator_size(j) >= 3);
        do {
            ull vtIndex = std::distance(poly.vertices_begin(), j->vertex());
            oneSurfaceCoords.push_back(newVertices[vtIndex]);
        } while ( ++j != i->facet_begin());
        Surface* sf = new Surface(oneSurfaceCoords);
        newSurface.push_back(sf);
    }

    surfaces.clear();
    surfaces = newSurface;
    return newVertices;
}
