#include "geom_structs.hpp"
#include <cmath>

bool Matrix33::isdiagonal() const
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

bool Matrix33::isorthogonal() const
{
    auto identity = transpose() * (*this);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (std::fabs(identity[i][j] - (i == j ? 1.0f : 0.0f)) > 1e-4)
                return false;
    return true;
}