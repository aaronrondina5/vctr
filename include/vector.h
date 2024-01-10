#ifndef INCLUDED_ARONDINA_VCTR_VECTOR
#define INCLUDED_ARONDINA_VCTR_VECTOR

// vctr

// std
#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <initializer_list>
#include <memory>
#include <stdexcept>

namespace arondina
{
namespace vctr
{

struct VectorConstants
{
    static const size_t maxDimensionsForSequentialArithmeticOps;
    static const size_t maxDimensionsForSequentialDotProduct; 
};

/**
 * @brief A class representing a mathematical vector of elements
 *        T must support +, -, *, and /.
*/
template <typename T>
class Vector
{
public:
    /**
     * @brief Iterator class that takes a pointer to a type T
    */
    class Iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        /**
         * @brief Constructor for creating an Iterator instance.
         */
        Iterator(T* ptr): m_ptr(ptr)
        {
        }

        /**
         * @brief Dereferences the iterator to access the value it points to.
         */
        reference operator*() const
        {
            return *m_ptr;
        }

        /**
         * @brief Prefix increment operator. Advances the iterator to the next element.
         */
        Iterator& operator++()
        {
            m_ptr++;
            return *this;
        }

        /**
         * @brief Postfix increment operator. Advances the iterator to the next element.
         */
        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Prefix decrement operator. Moves the iterator to the previous element.
         */
        Iterator& operator--()
        {
            m_ptr--;
            return *this;
        }

        /**
         * @brief Relational operators for comparing iterator positions.
         */
        bool operator<(const Iterator& rhs) const { return m_ptr < rhs.m_ptr; }
        bool operator<=(const Iterator& rhs) const { return m_ptr <= rhs.m_ptr; }
        bool operator>(const Iterator& rhs) const { return m_ptr > rhs.m_ptr; }
        bool operator>=(const Iterator& rhs) const { return m_ptr >= rhs.m_ptr; }
        bool operator==(const Iterator& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator!=(const Iterator& rhs) const { return m_ptr != rhs.m_ptr; }

        /**
         * @brief Random access operators to support jumping forward or backward by n positions.
         */
        Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
        Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }
        Iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        Iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }

        /**
         * @brief Subtraction operator to calculate the distance between two iterators.
         */
        difference_type operator-(const Iterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

    private:
        T* m_ptr;
    };

    /**
     * @brief Constructor that initializes the Vector with an initializer list.
     *        This allows for direct list initialization of the Vector, like Vector v{1, 2, 3}.
     */
    Vector(std::initializer_list<T> list)
        : m_dimensions(list.size())
        , m_data(new T[list.size()])
    {
        std::copy(list.begin(), list.end(), m_data);
    }

    /**
     * @brief Constructor that initializes the Vector with a specific size and a default value for all elements.
     *        For example, Vector<int> v(7, 0) creates a Vector of size 7 with all elements initialized to 0.
     */
    Vector(size_t dimensions, T default_value)
        : m_dimensions(dimensions)
        , m_data(new T[dimensions])
    {
        for (size_t i = 0; i < m_dimensions; ++i)
        {
            m_data[i] = default_value;
        }
    }

    /**
     * @brief Constructor that initializes the Vector with a specific size. 
     *        Elements are default-constructed and may contain garbage values.
     */
    Vector(size_t dimensions)
        : m_dimensions(dimensions)
        , m_data(new T[dimensions])
    {
    }

    /**
     * @brief Copy constructor that creates a new Vector as a copy of an existing Vector.
     *        This constructor is used when a new Vector is directly initialized with another Vector.
     */
    Vector(const Vector<T>& rhs)
        : m_dimensions(rhs.dimensions())
        , m_data(new T[rhs.dimensions()])
    {
        std::copy(rhs.m_data, rhs.m_data + m_dimensions, m_data);
    }

    /**
     * @brief Move constructor that transfers the ownership of the internal data from another Vector to this Vector.
     *        This constructor is used to optimize performance when a temporary Vector is moved into a new Vector.
     *        After the move, the original Vector is left in a valid but unspecified state.
     */
    Vector(Vector<T>&& rhs) noexcept
        : m_dimensions(rhs.m_dimensions)
        , m_data(std::move(rhs.m_data))
    {
        rhs.m_dimensions = 0;
        rhs.m_data = nullptr; // Avoid double deletion
    }

    /**
     * @brief Assignment operator to copy the contents from another Vector.
     *        If the current Vector already contains data, it is first deleted before copying.
     */
    Vector<T>& operator=(const Vector<T>& rhs)
    {
        if(this != &rhs)
        {
            delete[] m_data;

            m_dimensions = rhs.m_dimensions;

            m_data = new T[m_dimensions];
            for (size_t dimension = 0; dimension < m_dimensions; ++dimension)
            {
                m_data[dimension] = rhs.m_data[dimension];
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator to transfer the contents from another Vector.
     *        The other Vector is left in a valid but unspecified state.
     */
    Vector<T>& operator=(Vector<T>&& rhs) noexcept
    {
        if(this != &rhs)
        {
            delete[] m_data;

            m_dimensions = rhs.m_dimensions;
            m_data = rhs.m_data;

            rhs.m_dimensions = 0;
            rhs.m_data = nullptr;
        }
        return *this;
    }

    /**
     * @brief Destructor to clean up allocated resources.
     */
    ~Vector()
    {
        delete[] m_data;
    }

   /**
     * @brief Returns the number of elements in the Vector.
     */
    size_t dimensions() const
    {
        return m_dimensions;
    }

    /**
     * @brief Calculates the geometric length (magnitude) of the Vector.
     */
    double magnitude() const
    {
        T sum_squares = 0;

        if(m_dimensions > VectorConstants::maxDimensionsForSequentialArithmeticOps)
        {
            sum_squares = std::transform_reduce(
                std::execution::par
                , m_data
                , m_data + m_dimensions
                , 0.0
                , std::plus<>()
                , [](const T& value) { return value * value; });
        }
        else
        {
            sum_squares = std::transform_reduce(
                m_data
                , m_data + m_dimensions
                , 0.0
                , std::plus<>()
                , [](const T& value) { return value * value; });
        }

        return std::sqrt(sum_squares);
    }

    /**
     * @brief scale this vector.
    */
    void scale(double scalar)
    {
        if (m_dimensions > VectorConstants::maxDimensionsForSequentialArithmeticOps)
        {
            std::transform(
                std::execution::par,
                m_data,
                m_data + m_dimensions,
                m_data,
                [scalar](const T& a) { return scalar * a; });
        }
        else
        {
            for (int i = 0; i < m_dimensions; ++i)
            {
                m_data[i] *= scalar;
            }
        }
    }

    /**
     * @brief Provides an iterator to the beginning of the Vector.
     */
    Iterator begin()
    {
        return Iterator(&m_data[0]);
    }

    /**
     * @brief Provides an iterator to the end of the Vector.
     */
    Iterator end()
    {
        return Iterator(&m_data[m_dimensions]);
    }

    /**
     * @brief Non-const access. If outside dimensions, throws a std::runtime_error.
    */
    T& operator[](size_t index)
    {
        if(index > m_dimensions - 1)
        {
            throw std::runtime_error("index out of bounds.");
        }
        return m_data[index];
    }

    /**
     * @brief Const access. If outside dimensions, throws a std::runtime_error.
    */
    const T& operator[](size_t index) const
    {
        if(index > m_dimensions - 1)
        {
            throw std::runtime_error("index out of bounds.");
        }
        return m_data[index];
    }

    /**
     * @brief equality check to another Vector references.
    */
    bool operator==(const Vector<T>& rhs)
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
     * @brief inequality check. Relies on operator==
    */
    bool operator!=(const Vector<T>& rhs)
    {
        return !(*this == rhs);
    }

    /**
     * @brief Adds another vector.
     *        Uses parallelization if large enough.
     * 
     *        see std::transform 
     *        https://en.cppreference.com/w/cpp/algorithm/transform
    */
    Vector<T> operator+(const Vector<T>& rhs)
    {
        if(rhs.dimensions() != m_dimensions)
        {
            throw std::runtime_error("unequal vector sizes.");
        }
        if(m_dimensions == 0)
        {
            return Vector<T>(m_dimensions);
        }

        Vector<T> result(m_dimensions);
        if (m_dimensions > VectorConstants::maxDimensionsForSequentialArithmeticOps) {
            std::transform(
                std::execution::par
                , m_data
                , m_data + m_dimensions
                , rhs.m_data
                , result.m_data
                , [](const T& a, const T& b) { return a + b; });
        } else {
            std::transform(
                m_data
                , m_data + m_dimensions
                , rhs.m_data
                , result.m_data
                , [](const T& a, const T& b) { return a + b; });
        }

        return result;
    }

    /**
     * @brief Subtracts another vector.
              Uses parallelization if large enough.
     * 
     *        see std::transform 
     *        https://en.cppreference.com/w/cpp/algorithm/transform
    */
    Vector<T> operator-(const Vector<T>& rhs)
    {
        if(rhs.dimensions() != m_dimensions)
        {
            throw std::runtime_error("unequal vector sizes.");
        }
        if(m_dimensions == 0)
        {
            return Vector<T>(m_dimensions);
        }

        Vector<T> result(m_dimensions);
        if (m_dimensions > VectorConstants::maxDimensionsForSequentialArithmeticOps) {
            std::transform(
                std::execution::par
                , m_data
                , m_data + m_dimensions
                , rhs.m_data
                , result.m_data
                , [](const T& a, const T& b) { return a - b; });
        } else {
            std::transform(
                m_data
                , m_data + m_dimensions
                , rhs.m_data
                , result.m_data
                , [](const T& a, const T& b) { return a - b; });
        }

        return result;
    }

private:
    T* m_data;
    size_t m_dimensions;
};

/**
 * @brief Computes the dot product of 2 vectors by utilizing
 *        Uses parallelization if large enough data.
 * 
 *        see transform_reduce
 *        https://en.cppreference.com/w/cpp/algorithm/transform_reduce
*/
template<typename T>
T dot_product(Vector<T>& rhs, Vector<T>& lhs)
{
    if(lhs.dimensions() != rhs.dimensions())
    {
        throw std::runtime_error("unequal vector sizes.");
    }
    if(lhs.dimensions() == 0)
    {
        throw std::runtime_error("cannot dot product null vectors.");
    }

    if(lhs.dimensions() > VectorConstants::maxDimensionsForSequentialDotProduct)
    {
        return std::transform_reduce(
            std::execution::par
            , lhs.begin()
            , lhs.end()
            , rhs.begin()
            , T()
            , std::plus<>() // reduction
            , std::multiplies<>()); // transformation
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

/**
 * @brief Determines if the vectors are perpendicular by computing 
 *        the dot product in a parallel manner and checking if equals 0.
 * 
*/
template<typename T>
bool are_perpendicular(const Vector<T>& rhs, const Vector<T>& lhs)
{
    return dot_product(lhs, rhs) == 0;
}

/**
 * @brief Generate the unit vector for this vector.
 *        The unit vector is determined by dividing each vector
 *        component by the magnitude of the vector. This is because
 *        if you scale individual components, your length will scale
 *        in the same proportion.
 * 
 *        magnitude = sqrt( x^2 + y^2 )
 *        apply a constant to each dimension
 *        sqrt( (Cx)^2 + (Cy)^2 ) = sqrt(C^2 * ( x^2 + y^2 )) = C * sqrt( x^2 + y^2 ) = C * magnitude.
*/
template<typename T>
Vector<T> unit_vector(const Vector<int>& vec)
{
    Vector<T> copy(vec);
    double scale = 1.0 / vec.magnitude();
    copy.scale(scale);
    return copy;
}

} // vctr
} // arondina

#endif