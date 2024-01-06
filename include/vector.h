#ifndef INCLUDED_ARONDINA_VCTR_VECTOR
#define INCLUDED_ARONDINA_VCTR_VECTOR

// vctr

// std
#include <algorithm>
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
 * T must support +, -, *, and /.
*/
template <typename T>
class Vector
{
public:
    /**
     * Iterator
    */
    class Iterator
    {
    public:
        using difference_type = std::ptrdiff_t;

        Iterator(T* ptr): m_ptr(ptr)
        {
        }

        T& operator*() const
        {
            return *m_ptr;
        }

        Iterator& operator++()
        {
            m_ptr++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--()
        {
            m_ptr--;
            return *this;
        }

        /**
         * Relational operators
        */
        bool operator<(const Iterator& rhs) const
        {
            return m_ptr < rhs.m_ptr;
        }

        bool operator<=(const Iterator& rhs) const
        {
            return m_ptr <= rhs.m_ptr;
        }

        bool operator>(const Iterator& rhs) const
        {
            return m_ptr > rhs.m_ptr;
        }

        bool operator>=(const Iterator& rhs) const
        {
            return m_ptr >= rhs.m_ptr;
        }

        bool operator==(const Iterator& rhs) const
        {
            return m_ptr == rhs.m_ptr;
        }

        bool operator!=(const Iterator& rhs) const
        {
            return m_ptr != rhs.m_ptr;
        }

        /**
         * Random access iterators
        */
        Iterator operator+(difference_type n) const
        {
            return Iterator(m_ptr + n);
        }

        Iterator operator-(difference_type n) const
        {
            return Iterator(m_ptr - n);
        }

        Iterator& operator+=(difference_type n)
        {
            m_ptr += n;
            return *this;
        }

        Iterator& operator-=(difference_type n)
        {
            m_ptr -= n;
            return *this;
        }

        difference_type operator-(const Iterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

    private:
        T* m_ptr;
    };

    /**
     * @brief Initialize with initializer list.
     * ie Vector v{1, 2, 3};
    */
    Vector(std::initializer_list<T> list)
        : m_dimensions(list.size())
        , m_data(new T[list.size()])
    {
        std::copy(list.begin(), list.end(), m_data);
    }

    /**
     * @brief Initialize with size & default value
     * ie Vector<int> v(7, 0);
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
     * @brief Copy constructor.
    */
    Vector(const Vector& rhs)
        : m_dimensions(rhs.dimensions())
        , m_data(new T[rhs.dimensions()])
    {
        std::copy(rhs.m_data, rhs.m_data + m_dimensions, m_data);
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
    ~Vector()
    {
        delete[] m_data;
    }

    /**
     * return the dimensions
    */
    size_t dimensions() const
    {
        return m_dimensions;
    }

    // Iterator methods
    Iterator begin()
    {
        return Iterator(&m_data[0]);
    }

    Iterator end()
    {
        return Iterator(&m_data[m_dimensions]);
    }

    /**
     * Access element
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
        const int limit_size_for_parallelization = 1000;

        Vector<T> result(m_dimensions);

        if(m_dimensions > limit_size_for_parallelization)
        {
            std::transform(
                std::execution::par
                , this->begin()
                , this->end()
                , rhs.begin()
                , result.begin(),
                [](const T& a, const T& b) { return a + b; });
        }
        else
        {
            // could potentially have just changed the execution policy to std::execution::seq
            // but I almost think its nicer to fall back to visible for loop
            for(int i = 0; i < m_dimensions; ++i)
            {
                result[i] = (*this)[i] + rhs[i];
            }
        }

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
        const int limit_size_for_parallelization = 1000;

        Vector<T> result(m_dimensions);
        
        if(m_dimensions > limit_size_for_parallelization)
        {
            std::transform(
                std::execution::par
                , this->begin()
                , this->end()
                , rhs.begin()
                , result.begin(),
                [](const T& a, const T& b) { return a - b; });
        }
        else
        {
            // see comment inside operator+
            for(int i = 0; i < m_dimensions; ++i)
            {
                result[i] = (*this)[i] - rhs[i];
            }
        }

        return result;
    }

private:
    T* m_data;

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