#ifndef INCLUDED_ARONDINA_VCTR_VECTOR
#define INCLUDED_ARONDINA_VCTR_VECTOR

// vctr

// std
#include <algorithm>
#include <execution>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <vector>

namespace arondina
{
namespace vctr
{

/**
 * T must support +, -, *, and /.
*/
template <typename T>
class Vector
{

public:
    /**
     * @brief Initialize with initializer list.
     * ie Vector v{1, 2, 3};
    */
    Vector(std::initializer_list<T> list)
        : m_dimensions(list.size())
        , m_data(std::make_unique<T[]>(m_dimensions))
    {
        std::copy(list.begin(), list.end(), m_data.get());
    }

    /**
     * @brief Initialize with size.
     * ie Vector v(7);
    */
    Vector(size_t dimensions)
        : m_dimensions(dimensions)
        , m_data(std::make_unique<T[]>(m_dimensions))
    {
    }

    /**
     * @brief Copy constructor.
    */
    Vector(const Vector& rhs)
        : m_dimensions(rhs.size())
        , m_data(std::make_unique<T[]>(m_dimensions))
    {
        std::copy(rhs.m_data.get(), rhs.m_data.get() + m_dimensions, m_data.get());
    }

    /**
     * @brief Move constructor.
     */
    Vector(Vector&& rhs) noexcept
        : m_dimensions(rhs.m_dimensions)
        , m_data(std::move(rhs.m_data))
    {
        rhs.m_dimensions = 0; // Leave the rhs in a valid state
    }

    /**
     * @brief Assigment
    */
    Vector& operator=(const Vector& rhs)
    {
        if(this != &rhs)
        {
            m_data = rhs.m_data;
            m_dimensions = rhs.m_dimensions;
        }
        return *this;
    }

    /**
     * @brief Move assignment.
     */
    Vector& operator=(Vector&& rhs) noexcept
    {
        if(this != &rhs)
        {
            m_data = std::move(rhs.m_data);
            m_dimensions = rhs.m_dimensions;
            rhs.m_dimensions = 0;
        }
        return *this;
    }

    /**
     * @brief delete the data from the heap
    */
    ~Vector() = default;

    /**
     * return the dimensions
    */
    size_t dimensions() const
    {
        return m_dimensions;
    }

    // Iterator methods
    T* begin() {
        return m_data.get(); // Pointer to the first element
    }
    T* end() {
        return m_data.get() + m_dimensions; // Pointer to one past the last element
    }

    /**
     * @brief Const versions of iterator methods
    */
    const T* cbegin() const {
        return m_data.get();
    }
    const T* cend() const {
        return m_data.get() + m_dimensions;
    }

    /**
     * Access element
    */
    T& operator[](size_t index)
    {
        return m_data.get()[index];
    }

    /**
     * Compare to other vector.
     * Probably okay not to parallelize because of simple
     * boolean check and potential early exits.
    */
    bool operator==(const Vector& rhs)
    {
        if(rhs.dimensions() != m_dimensions)
        {
            return false;
        }
        for(int i = 0; i < m_dimensions; ++i)
        {
            if((*this)[i] != rhs[i])
            {
                return false;
            }
        }
        return true;
    }

    /**
     * Does not equal another vector
    */
    bool operator!=(const Vector& rhs)
    {
        return !(*this == rhs);
    }

    /**
     * Add another vector.
     * Uses parallelization if large enough.
     * 
     * see std::transform 
     * https://en.cppreference.com/w/cpp/algorithm/transform
    */
    Vector<T> operator+(const Vector<T>& rhs)
    {
        if(rhs.m_dimensions() != m_dimensions)
        {
            throw std::runtime_error("cannot add 2 different sizes.");
        }
        Vector<T> result(m_dimensions);
        std::transform(
            m_dimensions > 1000 ? std::execution::seq : std::execution::par
            , this->begin()
            , this->end()
            , rhs.begin()
            , result.begin(),
            [](const T& a, const T& b) { return a + b; });

        return result;
    }

    /**
     * Subtract another vector.
     * Uses parallelization if large enough.
     * 
     * see std::transform 
     * https://en.cppreference.com/w/cpp/algorithm/transform
    */
    Vector<T> operator-(const Vector<T>& rhs)
    {
        if(rhs.m_dimensions() != m_dimensions)
        {
            throw std::runtime_error("cannot add 2 different sizes.");
        }
        Vector<T> result(m_dimensions);
        std::transform(
            m_dimensions > 1000 ? std::execution::seq : std::execution::par
            , this->begin()
            , this->end()
            , rhs.begin()
            , result.begin(),
            [](const T& a, const T& b) { return a - b; });

        return result;
    }

private:
    std::unique_ptr<T[]> m_data;
    size_t m_dimensions;
};

/**
 * @brief Computes the dot product of 2 vectors by utilizing
 * Uses parallelization if large enough data.
 * 
 * see transform_reduce
 * https://en.cppreference.com/w/cpp/algorithm/transform_reduce
*/
template<typename T>
T dot_product(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if(lhs.m_dimensions() != rhs.dimensions())
    {
        throw std::runtime_error("cannot dot_product 2 different sizes.");
    }
    return std::transform_reduce(
        m_dimensions > 1000 ? std::execution::seq : std::execution::par
        , lhs.begin()
        , lhs.end()
        , rhs.begin()
        , T());
}

/**
 * Determines if the vectors are perpendicular by computing 
 * the dot product in a parallel manner and checking if equals 0.
 * 
*/
template<typename T>
bool are_perpendicular(const Vector<T>& rhs, const Vector<T>& lhs)
{
    return dot_product(lhs, rhs) == 0;
}

} // vctr
} // arondina

#endif