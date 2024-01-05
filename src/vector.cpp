#include "vector.h"

// vctr

// std

namespace arondina
{
namespace vctr
{

template<typename T>
T dot_product(const Vector<T>& rhs, const Vector<T>& lhs)
{
    if(lhs.dimensions() != rhs.dimensions())
    {
        throw std::runtime_error("cannot dot_product 2 different sizes.");
    }
    const int limit_size_for_parallelization = 1000;

    if(lhs.dimensions() > limit_size_for_parallelization)
    {
        return std::transform_reduce(
            std::execution::par
            , lhs.begin()
            , lhs.end()
            , rhs.begin()
            , T());
    }
    else
    {
        T result = T();
        for(int i = 0; i < lhs.dimensions(); ++i)
        {
            result += (lhs[i] * rhs[i]);
        }
        return result;
    }
}

template<typename T>
bool are_perpendicular(const Vector<T>& rhs, const Vector<T>& lhs)
{
    return dot_product(lhs, rhs) == 0;
}

} // vctr
} // arondina