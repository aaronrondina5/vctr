#ifndef INCLUDED_ARONDINA_VCTR_VCTR
#define INCLUDED_ARONDINA_VCTR_VCTR

// vctr

// std
#include <vector>

namespace arondina
{
namespace vctr
{

enum class LinearCombinationSolveAlgorithm
{
    LU_DECOMPOSITION
};

/**
 * @brief Solves linear combinations given the provided matrices.
 * 
 * @param a Matrix to be combined.
 * @param b Resulting column vector.
 * @param x The column vector result that contains the linear combinations of
 *          matrix a that result in matrix x.
*/
void solve_combinations(
    const std::vector<std::vector<int>>& a
    , const std::vector<int>& b
    , std::vector<int>& x
    , LinearCombinationSolveAlgorithm algorithm = LinearCombinationSolveAlgorithm::LU_DECOMPOSITION);

} // vctr
} // arondina

#endif