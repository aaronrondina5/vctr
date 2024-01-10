#ifndef INCLUDED_ARONDINA_VCTR_MATRIX
#define INCLUDED_ARONDINA_VCTR_MATRIX

// vctr
#include "vector.h"

// std
#include <initializer_list>
#include <stdexcept>

namespace arondina
{
namespace vctr
{

/**
 * @brief Matrix implementation. T must support arithmetic operations.
 *        This class does not contain vctr::Vectors in order to keep the
 *        implementations decoupled. You can still initailize a Matrix with a
 *        vctr::Vector.
*/
template<typename T>
class Matrix
{

public:
    /**
     * @brief Initialize with an initializer list of an initializer list.
     *        Will throw before allocating memory if the columns are mismatching.
    */
    Matrix(std::initializer_list<std::initializer_list<T>> initializer_list)
        : m_num_rows(initializer_list.size())
        , m_num_cols(0)
        , m_data(nullptr)
    {
        if(m_num_rows > 0)
        {
            auto it = initializer_list.begin();
            m_num_cols = it->size();

            for(; it != initializer_list.end(); ++it)
            {
                const std::initializer_list<T>& inner_init_list = *it;
                if(inner_init_list.size() != m_num_cols)
                {
                    throw std::runtime_error("Invalid column size.");
                }
            }

            m_data = new T*[m_num_rows];

            it = initializer_list.begin();
            for(size_t row = 0; row < m_num_rows; ++row, ++it)
            {
                m_data[row] = new T[m_num_cols];
                std::copy(it->begin(), it->end(), m_data[row]);
            }
        }
    }

    /**
     * @brief Initialize with an initializer list of vctr::Vectors.
     *        Will throw before allocating memory if the columns are mismatching.
    */
    Matrix(std::initializer_list<const Vector<T>&> initializer_list)
        : m_num_rows(initializer_list.size())
        , m_num_cols(0)
        , m_data(nullptr)
    {
        if(m_num_rows > 0)
        {
            auto it = initializer_list.begin();
            m_num_cols = it->size();

            for(; it != initializer_list.end(); ++it)
            {
                const Vector<T>& vec = *it;
                if(vec.dimensions() != m_num_cols)
                {
                    throw std::runtime_error("Invalid column size.");
                }
            }

            m_data = new T*[m_num_rows];
            
            it = initializer_list.begin();
            for(size_t row = 0; row < m_num_rows; ++row, ++it)
            {
                m_data[row] = new T[m_num_cols];
                for(size_t j = 0; j < m_num_cols; ++j)
                {
                    m_data[i][j] = vec[j];
                }
            }
        }
    }

    /**
     * @brief Initialize with column, rows, and default value.
    */
    Matrix(size_t num_rows, size_t num_cols, T default_value)
        : m_num_rows(num_rows)
        , m_num_cols(num_cols)
        , m_data(new T*[num_rows])
    {
        for (size_t row = 0; row < num_rows; ++row)
        {
            m_data[row] = new T[num_cols];
            std::fill_n(m_data[row], num_cols, default_value);
        }
    }

    /**
     * @brief Copy constructor.
    */
    Matrix(const Matrix<T>& rhs)
        : m_num_rows(rhs.m_num_rows)
        , m_num_cols(rhs.m_num_cols)
        , m_data(new T*[rhs.m_num_rows])
    {
        for(size_t row = 0; row < m_num_rows; ++row)
        {
            m_data[row] = new T[m_num_cols];
            std::copy(rhs.m_data[row], rhs.m_data[row] + m_num_cols, m_data[row]);
        }
    }

    /**
     * @brief Move constructor.
     *        Transfer ownership of the rhs resources and
     *        then reset the rhs source to a valid, undefined state.
    */
    Matrix(const Matrix<T>&& rhs)
        : m_num_rows(rhs.m_num_rows)
        , m_num_cols(rhs.m_num_cols)
        , m_data(new T*[rhs.m_num_rows])
    {
        m_num_rows = rhs.m_num_rows;
        m_num_cols = rhs.m_num_cols;
        m_data = rhs.m_data;

        rhs.m_num_rows = 0;
        rhs.m_num_cols = 0;
        rhs.m_data = nullptr;
    }

    /**
     * @brief Assignment. Checks for self assignment, frees existing resources
     *        re-assigns data.
    */
    Matrix<T>& operator=(const Matrix<T>& rhs)
    {
        if(this != &rhs)
        {
            delete_heap_data();

            m_num_rows = rhs.m_num_rows;
            m_num_cols = rhs.m_num_cols;

            m_data = new T*[m_num_rows];
            for(size_t row = 0; row < m_num_rows; ++row)
            {
                m_data[row] = new T[m_num_cols];
                std::copy(rhs.m_data[row], rhs.m_data[row] + m_num_cols, m_data[row]);
            }
        }
        return *this;
    }

    /**
     * @brief Move assigment. Delete existing resources, then simply move the pointer
     *        from the rhs.m_data to the pointer in this object.
    */
    Matrix<T>& operator=(Matrix<T>&& rhs)
    {
        if(this != &rhs)
        {
            delete_heap_data();

            m_num_rows = rhs.m_num_rows;
            m_num_cols = rhs.m_num_cols;
            m_data = rhs.m_data;

            rhs.m_num_rows = 0;
            rhs.m_num_cols = 0;
            rhs.m_data = nullptr;
        }

        return *this;
    }

    /**
     * @brief Destroy data.
    */
    ~Matrix() {
        delete_heap_data();
    }

    /**
     * @brief access non-const element.
    */
    T& operator()(size_t i, size_t j)
    {
        return data[i][j];
    }

    /**
     * @brief access const element.
    */
    const T& operator()(size_t i, size_t j) const
    {
        return data[i][j];
    }

private:
    size_t m_num_rows;
    size_t m_num_cols;
    T** m_data;

    void delete_heap_data()
    {
        for(size_t row = 0; row < m_num_rows; ++row)
        {
            delete[] m_data[row];
        }
        delete[] m_data;
    }
};

} // vctr
} // arondina

#endif