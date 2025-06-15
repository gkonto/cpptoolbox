#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "geom_structs.hpp"
#include "math_utils.hpp"
#include <limits.h>
#include <span>


float triaArea(float x1, float y1, float x2, float y2, float x3, float y3);
Point3d operator-(const Point3d &a, const Point3d &b);
Point2d operator-(const Point2d &a, const Point2d &b);
bool intersection(const AABB3d a, const AABB3d &b);
bool intersection(const Sphere &a, const Sphere &b);

// Transform AABB 'a' by the matrix 'm' and translation t,
// find maximum extends, and store result into AABB b.
void UpdateAABB(AABB3d a, float m[3][3], float t[3], AABB3d &b);

// Compute indices to the two most separated points of the (up to) six points
// defining the AABB encompassing the point set. Return these as min and max.
void mostSeparatePointsOnAABB(int &min, int &max, std::span<Point3d> pt);
Point3d normal(const Point3d &a, const Point3d &b, const Point3d &c);

size_t pointFarthestFromEdge(const Point2d &a,
                             const Point2d &b,
                             std::span<Point2d> points);

/*
Pros:
    Exact for points in the triangle's plane
    Efficient, using only dot products and one division
    Works in arbitrary 3D orientatios without projections
Cons:
    Assumes P is in the plane of triange ABC
    Can be unstable if triangle is degenerate (eg area close to zero)
*/
void barycentric1(const Point3d &a,
                  const Point3d &b,
                  const Point3d &c,
                  const Point3d &p,
                  float &u,
                  float &v,
                  float &w);

/*
Pros:
    Works even if triangle isn't in a plane parallel to axes
    More geometrically intuitive (based on projected areas)
    Ofter numerically stable in degenerate or near-degenerate cases
Cons:
    Slightly more expensive (uses cross product, abs, area computations)
    Involves projection logic - a bit more code
*/
template<typename T>
void barycentric2(const T (&a)[3],
                  const T (&b)[3],
                  const T (&c)[3],
                  const T (&p)[3],
                  float &u,
                  float &v,
                  float &w)
{
    T v0[3], v1[3], m[3];
    sub(b, a, v0);
    sub(c, a, v1);
    // Unnormalized triangle normal
    crossProd(v0, v1, m);
    // Nominators and one-over-denominator for u and v ratios
    float nu, nv, ood;
    // Absolute components for determining projection plane
    float x = std::abs(m.x);
    float y = std::abs(m.y);
    float z = std::abs(m.z);
    // Compute areas in plane of largest projection
    if (x >= y && x >= z)
    {
        // x is largest, project to the yz plane
        nu = TriArea2D(p.y, p.z, b.y, b.z, c.y, c.z); // Area of PBC in yz plane
        nv = TriArea2D(p.y, p.z, c.y, c.z, a.y, a.z); // Area of PCA in yz plane
        ood = 1.0f / m.x; // 1 / (2 * area of ABC in yz plane)
    }
    else if (y >= x && y >= z)
    {
        // y is largest, project to the xz plane
        nu = TriArea2D(p.x, p.z, b.x, b.z, c.x, c.z);
        nv = TriArea2D(p.x, p.z, c.x, c.z, a.x, a.z);
        ood = 1.0f / -m.y;
    }
    else
    {
        // z is largest, project to the xy plane
        nu = TriArea2D(p.x, p.y, b.x, b.y, c.x, c.y);
        nv = TriArea2D(p.x, p.y, c.x, c.y, a.x, a.y);
        ood = 1.0f / m.z;
    }
    u = nu * ood;
    v = nv * ood;
    w = 1.0f - u - v;
}


#endif