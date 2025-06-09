#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include "vector.hpp"

// FIXME the points must be co-planar!
template<typename T>
bool isConvexQuad(const T (&a)[3],
                  const T (&b)[3],
                  const T (&c)[3],
                  const T (&d)[3])
{
    // Quad is non convex if dot(cross(db, ba), Cross(db, bc)) >= 0
    T db[3], ab[3], cb[3];
    sub(d, b, db);
    sub(a, b, ab);
    sub(c, b, cb);

    T bda[3], bdc[3];
    crossProd(db, ab, bda);
    crossProd(db, cb, bdc);

    if (dotProd(bda, bdc) >= 0.0f)
        return 0;
    // Quad is now convex iff dot
    T ca[3], da[3], ba[3];
    sub(c, a, ca);
    sub(d, a, da);
    sub(b, a, ba);

    T acd[3], acb[3];
    crossProd(ca, da, acd);
    crossProd(ca, ba, acb);
    return dotProd(acd, acb) < 0.0f;
}

#endif