#ifndef CONVEX_HPP_INCLUDED
#define CONVEX_HPP_INCLUDED

#include <float.h>
#include <limits.h>
#include <span>
#include <stddef.h>


template<typename T>
size_t pointFarthestFromEdge(const T (&a)[2],
                             const T (&b)[2],
                             std::span<const T[2]> points)
{
    // Create edge vector and vector (counterclockwise) perpendicular to it
    T ba[2] = {0};
    sub(b, a, ba);
    T eperp[2] = {-ba[1], ba[0]};
    // Track index, 'distance' and 'rightmostness' of currenlty best point
    int bestIndex = -1;
    float maxVal = -FLT_MAX;
    float rightMostVal = -FLT_MAX;
    // Test all points to find the one farthest from edge ab on the left side
    for (int i = 0; i < points.size(); ++i)
    {
        T pia[2] = {0};
        sub(points[i] - a);

        float d = dot(pia, eperp); // d is proportionalto distance along eperp
        float r = dot(pia, ba);    // r is proportional to distance along ba
        if (d > maxVal || (d == maxVal && r > rightMostVal))
        {
            bestIndex = i;
            maxVal = d;
            rightMostVal = r;
        }
    }
    return bestIndex;
}


#endif