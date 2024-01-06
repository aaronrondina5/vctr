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

/**
 * @brief A class representing a mathematical vector of elements
 * T must support +, -, *, and /.
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
        using difference_type = std::ptrdiff_t;

        /**
         * @brief Constructor for creating an Iterator instance.
         * @param ptr A pointer to an element of type T, indicating the starting point of the iterator.
         */
        Iterator(T* ptr): m_ptr(ptr)
        {
        }

        /**
         * @brief Dereferences the iterator to access the value it points to.
         * @return A reference to the value of type T pointed to by the iterator.
         */
        T& operator*() const
        {
            return *m_ptr;
        }

        /**
         * @brief Prefix increment operator. Advances the iterator to the next element.
         * @return A reference to the iterator after it has been advanced.
         */
        Iterator& operator++()
        {
            m_ptr++;
            return *this;
        }

        /**
         * @brief Postfix increment operator. Advances the iterator to the next element.
         * @return A copy of the iterator before it was advanced.
         */
        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * @brief Prefix decrement operator. Moves the iterator to the previous element.
         * @return A reference to the iterator after it has been moved back.
         */
        Iterator& operator--()
        {
            m_ptr--;
            return *this;
        }

        /**
         * @brief Relational operators for comparing iterator positions.
         * @param rhs The right-hand side iterator for comparison.
         * @return Boolean result of the comparison operation.
         */
        bool operator<(const Iterator& rhs) const { return m_ptr < rhs.m_ptr; }
        bool operator<=(const Iterator& rhs) const { return m_ptr <= rhs.m_ptr; }
        bool operator>(const Iterator& rhs) const { return m_ptr > rhs.m_ptr; }
        bool operator>=(const Iterator& rhs) const { return m_ptr >= rhs.m_ptr; }
        bool operator==(const Iterator& rhs) const { return m_ptr == rhs.m_ptr; }
        bool operator!=(const Iterator& rhs) const { return m_ptr != rhs.m_ptr; }

        /**
         * @brief Random access operators to support jumping forward or backward by n positions.
         * @param n The number of positions to move the iterator.
         * @return An iterator positioned n elements away from the current one.
         */
        Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
        Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }
        Iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        Iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }

        /**
         * @brief Subtraction operator to calculate the distance between two iterators.
         * @param other Another iterator of the same type.
         * @return The number of elements between the current iterator and the 'other' iterator.
         */
        difference_type operator-(const Iterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

    private:
        T* m_ptr; ///< Pointer to the current element in the iteration.
    };

    /**
     * @brief Constructor that initializes the Vector with an initializer list.
     *        This allows for direct list initialization of the Vector, like Vector v{1, 2, 3}.
     * @param list An initializer list containing elements of type T to initialize the Vector.
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
     * @param dimensions The size of the Vector.
     * @param default_value The default value to initialize each element of the Vector.
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
     * @param dimensions The size of the Vector.
     */
    Vector(size_t dimensions)
        : m_dimensions(dimensions)
        , m_data(new T[dimensions])
    {
    }

    /**
     * @brief Copy constructor that creates a new Vector as a copy of an existing Vector.
     *        This constructor is used when a new Vector is directly initialized with another Vector.
     * @param rhs The Vector to be copied.
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
     * @param rhs The Vector from which the data is moved.
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
     * @param rhs The Vector from which to copy.
     * @return A reference to the current Vector after copying.
     */
    Vector<T>& operator=(const Vector<T>& rhs)
    {
        if(this != &rhs)
        {
            // Delete the existing resources
            delete[] m_data;

            // copy dimensions
            m_dimensions = rhs.m_dimensions;

            // re-allocate memory & copy data
            m_data = new T[m_dimensions];
            for (int i = 0; i < m_dimensions; ++i)
            {
                m_data[i] = rhs.m_data[i];
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator to transfer the contents from another Vector.
     *        The other Vector is left in a valid but unspecified state.
     * @param rhs The Vector from which to move data.
     * @return A reference to the current Vector after the move.
     */
    Vector<T>& operator=(Vector<T>&& rhs) noexcept
    {
        if(this != &rhs)
        {
            // delete current data
            delete[] m_data;

            // move the data from the other vector
            m_dimensions = rhs.m_dimensions;
            m_data = rhs.m_data; // this will make a pointer to the old data

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
     * @return The size of the Vector.
     */
    size_t dimensions() const
    {
        return m_dimensions;
    }

    /**
     * @brief Calculates the geometric length (magnitude) of the Vector.
     * @return The geometric length of the Vector.
     */
    double magnitude() const
    {
        T sum_squares = 0;

        const int limit_size_for_parallelization = 1000;
        if(m_dimensions > limit_size_for_parallelization)
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
     * @brief Provides an iterator to the beginning of the Vector.
     * @return An iterator pointing to the first element in the Vector.
     */
    Iterator begin()
    {
        return Iterator(&m_data[0]);
    }

    /**
     * @brief Provides an iterator to the end of the Vector.
     * @return An iterator pointing to one past the last element in the Vector.
     */
    Iterator end()
    {
        return Iterator(&m_data[m_dimensions]);
    }

    /**
     * @brief Non-const access. If outside dimensions, throws a std::runtime_error.
     * @return A non-const reference to the element T at index.
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
     * @return A const reference to the value T at the index.
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
     * @brief equality check.
     * @param A const reference to another vector
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
     * Uses parallelization if large enough.
     * 
     * see std::transform 
     * https://en.cppreference.com/w/cpp/algorithm/transform
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

        const int limit_size_for_parallelization = 1000;
        Vector<T> result(m_dimensions);

        if (m_dimensions > limit_size_for_parallelization) {
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
     * @param rhs The Vector to subtract.
     * @return a copy of a new vector
     * 
     * see std::transform 
     * https://en.cppreference.com/w/cpp/algorithm/transform
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

        const int limit_size_for_parallelization = 1000;
        Vector<T> result(m_dimensions);

        if (m_dimensions > limit_size_for_parallelization) {
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
    T* m_data; ///<Pointer to the array data
    size_t m_dimensions; ///<size of the array of data
};

/**
 * @brief Computes the dot product of 2 vectors by utilizing
 * Uses parallelization if large enough data.
 * 
 * see transform_reduce
 * https://en.cppreference.com/w/cpp/algorithm/transform_reduce
*/
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