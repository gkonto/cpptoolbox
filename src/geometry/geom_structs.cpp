#include "geom_structs.hpp"
#include <cmath>

bool Matrix33::isdiagonal()
{
    if (std::abs(m[1]) > 1e-4)
        return false;
    if (std::abs(m[2]) > 1e-4)
        return false;
    if (std::abs(m[3]) > 1e-4)
        return false;
    if (std::abs(m[5]) > 1e-4)
        return false;
    if (std::abs(m[6]) > 1e-4)
        return false;
    if (std::abs(m[7]) > 1e-4)
        return false;
    return true;
}

bool Matrix33::isorthonormal() {}