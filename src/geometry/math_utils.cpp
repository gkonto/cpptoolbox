#include "math_utils.hpp"
#include "geom_structs.hpp"

float dotProd(const Point3d &u, const Point3d &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

float dotProd(const Point2d &u, const Point2d &v)
{
    return u.x * v.x + u.y * v.y;
}

Point3d crossProd(const Point3d &u, const Point3d &v)
{
    Point3d exo;
    float t1 = u.x - u.y;
    float t2 = v.y + v.z;
    float t3 = u.x * v.z;
    float t4 = t1 * t2 - t3;
    exo.x = v.y * (t1 - u.z) - t4;
    exo.y = u.z * v.x - t3;
    exo.z = t4 - u.y * (v.x - t2);
    return exo;
}

float magnitute(const Point3d &v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void normalize(const Point3d &v, Point3d &n)
{
    float magn = magnitute(v);

    if (magn != 0)
    {
        n.x = v.x / magn;
        n.y = v.y / magn;
        n.z = v.z / magn;
    }
}