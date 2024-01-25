/**
 * @file Math.cpp
 * @author Tomáš Ludrovan
 * @brief Mathematical functions
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "math/Math.hpp"

#include <unordered_map>
#include <boost/property_map/property_map.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/mark_domain_in_triangulation.h>
#include <CGAL/Polygon_2.h>

void triangulatePolygon(std::vector<double[2]> corners,
	std::vector<double[2][3]> triangles)
{
	// Inspired from https://doc.cgal.org/latest/Triangulation_2/Triangulation_2_2polygon_triangulation_8cpp-example.html

	// Could also use SimpleCartesian<double>, but that's rather needless
	// sacrifice of precision for the performance. Especially, considering
	// that this function won't be called too often -- only during reading
	// and writing stage files
	typedef CGAL::Exact_predicates_inexact_constructions_kernel            Kernel;
	typedef CGAL::Triangulation_vertex_base_2<Kernel>                      Vb;
	typedef CGAL::Constrained_triangulation_face_base_2<Kernel>            Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                    TDS;
	typedef CGAL::Exact_predicates_tag                                     Itag;
	typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>  CDT;
	typedef CGAL::Point_2<Kernel>                                          Point_2;
	typedef CGAL::Polygon_2<Kernel>                                        Polygon_2;
	typedef std::unordered_map<CDT::Face_handle, bool>                     InDomainMap;
	typedef boost::associative_property_map<InDomainMap>                   InDomain;

	Polygon_2 pog;
	// Construct the polygon object
	for (auto& corner : corners) {
		pog.push_back(CDT::Point(corner[0], corner[1]));
	}

	CDT cdt;
	// Insert the polygon into constrained triangulation
	cdt.insert_constraint(pog.vertices_begin(), pog.vertices_end(), true);

	InDomainMap in_domain_map;
	InDomain in_domain(in_domain_map);
	// Find facets (triangles) which are within the bounds of the polygon
	CGAL::mark_domain_in_triangulation(cdt, in_domain);

	decltype(triangles)::value_type triangle;
	// Move the triangles to the output vector
	for (CDT::Face_handle fh : cdt.finite_face_handles()) {
		// If the facet is within the bounds of the polygon...
		if (get(in_domain, fh)) {
			// For each triangle corner...
			for (int i = 0; i < 3; i++) {
				Point_2& pt = fh->vertex(i)->point();
				triangle[i][0] = pt.x();
				triangle[i][1] = pt.y();
			}
			triangles.push_back(std::move(triangle));
		}
	}
}
