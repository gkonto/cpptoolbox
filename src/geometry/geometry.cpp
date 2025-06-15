#include "geometry.hpp"
#include "math_utils.hpp"

float triaArea(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
}

Point3d operator-(const Point3d &a, const Point3d &b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Point2d operator-(const Point2d &a, const Point2d &b)
{
    return {a.x - b.x, a.y - b.y};
}

Matrix33 operator*(const Matrix33 &a, const Matrix33 &b)
{
    Matrix33 trg;
    trg.m[0] = 0.f;
    trg.m[1] = 0.f;
    trg.m[2] = 0.f;
    trg.m[3] = 0.f;
    trg.m[4] = 0.f;
    trg.m[5] = 0.f;
    trg.m[6] = 0.f;
    trg.m[7] = 0.f;
    trg.m[8] = 0.f;
    return trg;
}

bool intersection(const AABB3d a, const AABB3d &b)
{
    if (std::abs(a.c.x - b.c.x) > a.r[0] + b.r[0])
        return false;
    if (std::abs(a.c.y - b.c.y) > a.r[1] + b.r[1])
        return false;
    if (std::abs(a.c.z - b.c.z) > a.r[2] + b.r[2])
        return false;
    return 1;
}

bool intersection(const Sphere &a, const Sphere &b)
{
    Point3d d = b.c - a.c;
    auto dist2 = dotProd(d, d);
    //Spheres intersect if squared distance is less than squared sum of
    auto radiusSum = a.r + b.r;
    return dist2 <= radiusSum * radiusSum;
}

// Transform AABB 'a' by the matrix 'm' and translation t,
// find maximum extends, and store result into AABB b.
void UpdateAABB(AABB3d a, float m[3][3], float t[3], AABB3d &b)
{
    auto *bc = &b.c.x;
    auto *ac = &a.c.x;
    for (int i = 0; i < 3; ++i)
    {
        bc[i] = t[i];
        b.r[i] = 0.0f;
        for (int j = 0; j < 3; ++j)
        {
            bc[i] += m[i][j] * ac[j];
            b.r[i] += std::abs(m[i][j]) * a.r[j];
        }
    }
}

// Compute indices to the two most separated points of the (up to) six points
// defining the AABB encompassing the point set. Return these as min and max.
void mostSeparatePointsOnAABB(int &min, int &max, std::span<Point3d> pt)
{
    // First find most extreme points along principal axes
    int minx = 0, maxx = 0, miny = 0, maxy = 0, minz = 0, maxz = 0;
    for (int i = 1; i < pt.size(); i++)
    {
        if (pt[i].x < pt[minx].x)
            minx = i;
        if (pt[i].x < pt[maxx].x)
            maxx = i;
        if (pt[i].y < pt[miny].y)
            miny = i;
        if (pt[i].y < pt[maxy].y)
            maxy = i;
        if (pt[i].z < pt[minz].z)
            minz = i;
        if (pt[i].z < pt[maxz].z)
            maxz = i;
    }
    // Compute the squared distances for the three pairs of points
    float dist2x = dotProd(pt[maxx] - pt[minx], pt[maxx] - pt[minx]);
    float dist2y = dotProd(pt[maxy] - pt[miny], pt[maxy] - pt[miny]);
    float dist2z = dotProd(pt[maxz] - pt[minz], pt[maxz] - pt[minz]);
    // Pick the pair (min, max) of points most distant
    min = minx;
    max = maxx;
    if (dist2y > dist2x && dist2y > dist2z)
    {
        max = maxy;
        min = miny;
    }
    if (dist2z > dist2x && dist2z > dist2y)
    {
        max = maxz;
        min = minz;
    }
}

Point3d normal(const Point3d &a, const Point3d &b, const Point3d &c)
{
    auto v0 = b - a;
    auto v1 = c - a;
    auto n = crossProd(v0, v1);
    normalize(n, n);
    return n;
}

void barycentric1(const Point3d &a,
                  const Point3d &b,
                  const Point3d &c,
                  const Point3d &p,
                  float &u,
                  float &v,
                  float &w)
{
    auto v0 = b - a;
    auto v1 = c - a;
    auto v2 = p - a;

    auto d00 = dotProd(v0, v0);
    auto d01 = dotProd(v0, v1);
    auto d11 = dotProd(v1, v1);
    auto d20 = dotProd(v2, v0);
    auto d21 = dotProd(v2, v1);
    auto denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}

size_t pointFarthestFromEdge(const Point2d &a,
                             const Point2d &b,
                             std::span<Point2d> points)
{
    // Create edge vector and vector (counterclockwise) perpendicular to it
    auto ba = b - a;
    Point2d eperp{-ba.y, ba.x};
    // Track index, 'distance' and 'rightmostness' of currenlty best point
    int bestIndex = -1;
    float maxVal = -std::numeric_limits<float>::max();
    float rightMostVal = -std::numeric_limits<float>::max();
    // Test all points to find the one farthest from edge ab on the left side
    for (int i = 0; i < points.size(); ++i)
    {
        auto pia = points[i] - a;

        float d =
            dotProd(pia, eperp);    // d is proportionalto distance along eperp
        float r = dotProd(pia, ba); // r is proportional to distance along ba
        if (d > maxVal || (d == maxVal && r > rightMostVal))
        {
            bestIndex = i;
            maxVal = d;
            rightMostVal = r;
        }
    }
    return bestIndex;
}

// 2-by-2 Symmetric Schur decomposition. Given an n-by-n symmetric matrix
// and indices p, q such that 1 <= p < q <= n, computes a sine-cosine pair
// (s, c) that will serve to form a Jacobi rotation matrix.
//
// See Golub, Van Load, Matrix Computations, 3rd ed, p428
static void SymSchur2(const Matrix33 &a, int p, int q, float &c, float &s)
{
    if (std::abs(a[p][q]) > 0.0001f)
    {
        float r = (a[q][q] - a[p][p]) / (2.0f * a[p][q]);
        float t;
        if (r >= 0.0f)
            t = 1.0f / (r + std::sqrt(1.0f + r * r));
        else
            t = -1.0f / (-r + std::sqrt(1.0f + r * r));
        c = 1.0f / std::sqrt(1.0f + t * t);
        s = t * c;
    }
    else
    {
        c = 1.0f;
        s = 0.0f;
    }
}

void Jacobi(const Matrix33 &a, Matrix33 &v)
{
    auto a_copied = a;
    Matrix33 J, b, t;
    float prevoff, c, s;
    // Initialize v to identify matrix
    for (int i = 0; i < 3; ++i)
    {
        v[i][i] = 1.0f;
    }

    // Repeat for some maximum number of iterations
    constexpr int MAX_ITERATIONS = 50;
    for (int n = 0; n < MAX_ITERATIONS; ++n)
    {
        // Find largest off-diagonal absolute element a[p][q]
        int p = 0, q = 1;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i == j)
                    continue;
                if (std::abs(a_copied[i][j]) > std::abs(a_copied[p][q]))
                {
                    p = i;
                    q = j;
                }
            }
        }

        // Compute the Jacobi rotationmatrix J(p, q, theta)
        // (This code can be optimized for the three different cases of rotation)
        SymSchur2(a_copied, p, q, c, s);
        for (int i = 0; i < 3; ++i)
        {
            J[i][0] = J[i][1] = J[i][2] = 0.0f;
            J[i][i] = 1.0f;
        }
        J[p][p] = c;
        J[p][q] = s;
        J[q][p] = -s;
        J[q][q] = c;

        // Cumulate rotations into what will contain the eigenvectors
        v = v * J;

        // Make 'a' more diagonal, until just eigenvalues remain on diagonal
        a_copied = (J.transpose() * a_copied) * J;

        // Compute 'norm' of off-diagonal elements
        float off = 0.0f;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i == j)
                    continue;
                off += a_copied[i][j] * a_copied[i][j];
            }
        }

        /* off = sqrt(off); not needed for norm comparison */
        // Stop when norm no longer decreasing
        if (n > 2 && off >= prevoff)
            return;

        prevoff = off;
    }
}