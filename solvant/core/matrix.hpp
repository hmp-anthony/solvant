#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include "core/vector.hpp"

namespace solvant {
template <typename T, std::size_t R, std::size_t C>
class matrix {
public:
    constexpr matrix() : m_data({}){};

    //! matrix a = {1,2,3,...}
    //! this will generate in a matrix in ROW major form!
    template <typename... Ts>
    constexpr matrix(Ts... ts) noexcept : m_data{ts...} {}

    ~matrix(){};

    std::size_t rows() const { return R; }
    std::size_t cols() const { return C; }

    T operator()(const std::size_t i, const std::size_t j) const {
        return m_data[i * C + j];
    }

    T& operator()(const std::size_t i, const std::size_t j) {
        return m_data[i * C + j];
    }

    //! obtain raw row data
    T* operator[](const std::size_t row) { return &m_data[row * C]; }
    //! obtain raw row data
    const T* operator[](const std::size_t row) const {
        return &m_data[row * C];
    }

    const bool operator==(const matrix<T, R, C>& m) {
        return m_data == m.m_data;
    }

    vector<T, R> operator*(const vector<T, C>& v) {
        vector<T, R> r;
        for (std::size_t i = 0; i < R; ++i) {
            auto mi = (*this)[i];
            double ri = 0;
            for (std::size_t j = 0; j < C; ++j) {
                ri += mi[j] * v[j];
            }
            r[i] = ri;
        }
        return r;
    }

    void interchange_rows(const std::size_t i, const std::size_t j) {
        T temp[C];
        for (std::size_t k = 0; k < C; ++k) {
            temp[k] = m_data[i * C + k];
            m_data[i * C + k] = m_data[j * C + k];
            m_data[j * C + k] = temp[k];
        }
    }

private:
    // Initialises all elements to zero
    std::array<T, R * C> m_data;
};

template <typename T, std::size_t R, std::size_t C>
inline void print(const matrix<T, R, C>& a) {
    for (std::size_t i = 0; i < R; ++i) {
        std::cout << '\n';
        for (std::size_t j = 0; j < C; ++j) {
            std::cout << a(i, j) << " ";
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

/** Calculates the product between two matrices.
 */
template <typename T, std::size_t R, std::size_t K, std::size_t C>
inline void matrix_prod(const matrix<T, R, K>& a, const matrix<T, K, C>& b,
                        matrix<T, R, C>& c) {
    for (std::size_t i = 0; i < R; ++i) {
        const auto ai = a[i];  // obtain row i of a
        auto ci = c[i];        // obtain row i of c
        for (std::size_t k = 0; k < K; ++k) {
            const auto bk = b[k];    // obtain row k of b
            const auto aik = ai[k];  // obtain raw data - cir. mul. indices
            for (std::size_t j = 0; j < C; ++j) {
                ci[j] += aik * bk[j];
            }
        }
    }
}
}  // namespace solvant
