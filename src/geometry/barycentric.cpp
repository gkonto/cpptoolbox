#include "barycentric.hpp"

inline float TriArea2D(float x1,
                       float y1,
                       float x2,
                       float y2,
                       float x3,
                       float y3)
{
    return (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
}

void Barycentric(Point a,
                 Point b,
                 Point c,
                 Point p,
                 float &u,
                 float &v,
                 float &w)
{
    // Unnormalized triangle normal
    Vector m = Cross(b - a, c - a);

    // Nominators and one-over-denominator for u and v ratios
    float nu, nv, ood;

    // Absolute components for determining projection plane
    float x = Abs(m.x);
    float y = Abs(m.y);
    float z = Abs(m.z);

    // Compute areas in plane of larget projection
    if (x >= y && x >= z)
    {
        // x is largest project to the yz plane
        nu = TriArea2D(p.y, p.z, b.y, b.z, c.y, c.z); // Area of PBC in yz plane
        nv = TriArea2D(p.y, p.z, c.y, c.z, a.y, a.z); // Area of PCA in yz plane
        ood = 1.0f / m.x; // 1/(2 * area of ABC in yz plane
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