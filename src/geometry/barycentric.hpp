#ifndef BARYCENTRIC_HPP_INCLUDED
#define BARYCENTRIC_HPP_INCLUDED

#include <cmath>

inline float TriaArea(float x1,
                      float y1,
                      float x2,
                      float y2,
                      float x3,
                      float y3)
{
    return (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
}

/*
Pros:
    Exact for points in the triangle's plane
    Efficient, using only dot products and one division
    Works in arbitrary 3D orientatios without projections
Cons:
    Assumes P is in the plane of triange ABC
    Can be unstable if triangle is degenerate (eg area close to zero)
*/
template<typename T>
void barycentric1(const T (&a)[3],
                  const T (&b)[3],
                  const T (&c)[3],
                  const T (&p)[3],
                  float &u,
                  float &v,
                  float &w)
{
    T v0[3], v1[3], v2[3];
    sub(b, a, v0);
    sub(c, a, v1);
    sub(p, a, v2);

    auto d00 = dot(v0, v0);
    auto d01 = dot(v0, v1);
    auto d11 = dot(v1, v1);
    auto d20 = dot(v2, v0);
    auto d21 = dot(v2, v1);
    auto denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}

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