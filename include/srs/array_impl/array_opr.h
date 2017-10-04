//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017 Stig Rune Sellevag. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SRS_ARRAY_OPR_H
#define SRS_ARRAY_OPR_H

#include <algorithm>
#include <cmath>
#include <gsl/gsl>


namespace srs {

// Comparison operators:

template <class T, std::size_t N>
inline bool operator==(const Array<T, N>& a, const Array<T, N>& b)
{
    return std::equal(a.begin(), a.end(), b.begin());
}

template <typename T, std::size_t N>
inline bool operator!=(const Array<T, N>& a, const Array<T, N>& b)
{
    return !(a == b);
}

template <typename T, std::size_t N>
inline bool operator<(const Array<T, N>& a, const Array<T, N>& b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template <typename T, std::size_t N>
inline bool operator>(const Array<T, N>& a, const Array<T, N>& b)
{
    return (b < a);
}

template <typename T, std::size_t N>
inline bool operator<=(const Array<T, N>& a, const Array<T, N>& b)
{
    return !(a > b);
}

template <typename T, std::size_t N>
inline bool operator>=(const Array<T, N>& a, const Array<T, N>& b)
{
    return !(a < b);
}

//------------------------------------------------------------------------------

// Array addition:

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array<T, N>& a, const Array<T, N>& b)
{
    Array<T, N> result(a);
    return result += b;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array_ref<T, N>& a, const Array_ref<T, N>& b)
{
    Array<T, N> result(a);
    return result += b;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array_ref<const T, N>& a,
                             const Array_ref<const T, N>& b)
{
    Array<T, N> result(a);
    return result += b;
}

//------------------------------------------------------------------------------

// Array subtraction:

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array<T, N>& a, const Array<T, N>& b)
{
    Array<T, N> result(a);
    return result -= b;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array_ref<T, N>& a, const Array_ref<T, N>& b)
{
    Array<T, N> result(a);
    return result -= b;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array_ref<const T, N>& a,
                             const Array_ref<const T, N>& b)
{
    Array<T, N> result(a);
    return result -= b;
}

//------------------------------------------------------------------------------

// Scalar addition:

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result += scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const T& scalar, const Array<T, N>& a)
{
    Array<T, N> result(a);
    return result += scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array_ref<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result += scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const T& scalar, const Array_ref<T, N>& a)
{
    Array<T, N> result(a);
    return result += scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array_ref<const T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result += scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator+(const T& scalar, const Array_ref<const T, N>& a)
{
    Array<T, N> result(a);
    return result += scalar;
}

//------------------------------------------------------------------------------

// Scalar subtraction:

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result -= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const T& scalar, const Array<T, N>& a)
{
    Array<T, N> result(a);
    return result -= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array_ref<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result -= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const T& scalar, const Array_ref<T, N>& a)
{
    Array<T, N> result(a);
    return result -= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array_ref<const T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result -= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const T& scalar, const Array_ref<const T, N>& a)
{
    Array<T, N> result(a);
    return result -= scalar;
}

//------------------------------------------------------------------------------

// Scalar multiplication:

template <class T, std::size_t N>
inline Array<T, N> operator*(const Array<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const T& scalar, const Array<T, N>& a)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const Array_ref<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const T& scalar, const Array_ref<T, N>& a)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const Array_ref<const T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const T& scalar, const Array_ref<const T, N>& a)
{
    Array<T, N> result(a);
    return result *= scalar;
}

//------------------------------------------------------------------------------

// Matrix-matrix multiplication:

// Declaration.
template <class A1, class A2, class A3>
void mm_mul(const A1& a, const A2& b, A3& c);

template <class T>
inline Array<T, 2> operator*(const Array<T, 2>& a, const Array<T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

template <class T>
inline Array<T, 2> operator*(const Array_ref<T, 2>& a, const Array_ref<T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

template <class T>
inline Array<T, 2> operator*(const Array_ref<const T, 2>& a,
                             const Array_ref<const T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

template <class T>
inline Array<T, 2> operator*(const Array<T, 2>& a, const Array_ref<T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

template <class T>
inline Array<T, 2> operator*(const Array<T, 2>& a,
                             const Array_ref<const T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

template <class T>
inline Array<T, 2> operator*(const Array_ref<T, 2>& a, const Array<T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

template <class T>
inline Array<T, 2> operator*(const Array_ref<const T, 2>& a,
                             const Array<T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

//------------------------------------------------------------------------------

// Matrix-vector multiplication:

// Declaration.
template <class A1, class A2, class A3>
void mv_mul(const A1& a, const A2& v, A3& w);

template <class T>
inline Array<T, 1> operator*(const Array<T, 2>& a, const Array<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

template <class T>
inline Array<T, 1> operator*(const Array_ref<T, 2>& a, const Array_ref<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

template <class T>
inline Array<T, 1> operator*(const Array_ref<const T, 2>& a,
                             const Array_ref<const T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

template <class T>
inline Array<T, 1> operator*(const Array<T, 2>& a, const Array_ref<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

template <class T>
inline Array<T, 1> operator*(const Array<T, 2>& a,
                             const Array_ref<const T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

template <class T>
inline Array<T, 1> operator*(const Array_ref<T, 2>& a, const Array<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

template <class T>
inline Array<T, 1> operator*(const Array_ref<const T, 2>& a,
                             const Array<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

//------------------------------------------------------------------------------

// Mathematical functions:

// Find maximum element.
template <class T>
inline T max(const Array<T, 1>& vec)
{
    return *std::max_element(vec.begin(), vec.end());
}

// Find minimum element.
template <class T>
inline T min(const Array<T, 1>& vec)
{
    return *std::min_element(vec.begin(), vec.end());
}

// Find maximum element.
template <class T>
Array<T, 1> max(const Array<T, 2>& a, int dim = 2)
{
    Array<T, 1> result;
    if (dim == 1) {  // row
        result.resize(a.rows());
        for (std::size_t i = 0; i < a.rows(); ++i) {
            auto ri   = a.row(i);
            result(i) = *std::max_element(ri.begin(), ri.end());
        }
    }
    else {  // column
        result.resize(a.cols());
        for (std::size_t j = 0; j < a.cols(); ++j) {
            auto rj   = a.column(j);
            result(j) = *std::max_element(rj.begin(), rj.end());
        }
    }
    return result;
}

// Find maximum element.
template <class T>
Array<T, 1> min(const Array<T, 2>& a, int dim = 2)
{
    Array<T, 1> result;
    if (dim == 1) {
        result.resize(a.rows());
        for (std::size_t i = 0; i < a.rows(); ++i) {
            auto ri   = a.row(i);
            result(i) = *std::min_element(ri.begin(), ri.end());
        }
    }
    else {
        result.resize(a.cols());
        for (std::size_t j = 0; j < a.cols(); ++j) {
            auto rj   = a.column(j);
            result(j) = *std::min_element(rj.begin(), rj.end());
        }
    }
    return result;
}

// Compute sum of elements.
template <class T>
inline T sum(const Array<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(0));
}

// Compute product of elements.
template <class T>
inline T prod(const Array<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(1), std::multiplies<T>());
}

// Compute the norm of a vector.
template <class T>
inline T norm(const Array<T, 1>& vec, int p = 2)
{
    T pnorm = T(0);
    if (!vec.empty()) {
        if (p == 1) {  // L1 norm
            for (const auto& v : vec) {
                pnorm += std::abs(v);
            }
        }
        else if (p == 2) {  // L2 norm (Euclidean)
            for (const auto& v : vec) {
                pnorm += v * v;
            }
            pnorm = std::sqrt(pnorm);
        }
        else if ((p > 2) && (p <= 10)) {
            T pp = static_cast<T>(p);
            for (const auto& v : vec) {
                pnorm += std::pow(std::abs(v), pp);
            }
            pnorm = std::pow(pnorm, T(1) / pp);
        }
        else {  // L-infinity norm
            pnorm = std::abs(vec(0));
            for (const auto& v : vec) {
                pnorm = std::max(pnorm, std::abs(v));
            }
        }
    }
    return pnorm;
}

// Compute normalized vector.
inline srs::dvector normalize(const srs::dvector& vec)
{
    srs::dvector result(vec);
    return result /= norm(vec);
}

// Compute dot product.
template <class T>
inline T dot(const Array<T, 1>& a, const Array<T, 1>& b)
{
    // Intel MKL will be faster for large N.
    Expects(a.size() == b.size());
    return std::inner_product(a.begin(), a.end(), b.begin(), T(0));
}

// Compute cross product.
template <class T>
inline Array<T, 1> cross(const Array<T, 1>& a, const Array<T, 1>& b)
{
    Expects(a.size() == b.size() && a.size() == 3);
    Array<T, 1> result(3);
    result(0) = a(1) * b(2) - a(2) * b(1);
    result(1) = a(2) * b(0) - a(0) * b(2);
    result(2) = a(0) * b(1) - a(1) * b(0);
    return result;
}

// Vector convolution.
template <class T>
Array<T, 1> conv(const Array<T, 1>& a, const Array<T, 1>& b)
{
    const auto na = a.size();
    const auto nb = b.size();
    const auto nc = na + nb - 1;

    Array<T, 1> result(nc);

    for (std::size_t i = 0; i < nc; ++i) {
        result(i) = T(0);
        auto jmin = (i >= (nb - 1)) ? (i - (nb - 1)) : 0;
        auto jmax = (i < (na - 1)) ? i : (na - 1);
        for (std::size_t j = jmin; j <= jmax; ++j) {
            result(i) += a(j) * b(i - j);
        }
    }
    return result;
}

// Compute vector-scalar product and add the result to a vector.
template <class T>
void axpy(const T& a, const Array<T, 1>& x, Array<T, 1>& y)
{
    Expects(x.size() == y.size());
    for (std::size_t i = 0; i < x.size(); ++i) {
        y(i) = a * x(i) + y(i);
    }
}

// Matrix-matrix multiplication.
template <class A1, class A2, class A3>
void mm_mul(const A1& a, const A2& b, A3& c)
{
    using value_type = typename A1::value_type;

    Expects(A1::rank == 2);
    Expects(A2::rank == 2);
    Expects(A3::rank == 2);
    Expects(a.cols() == b.rows());

    c.resize(a.rows(), b.cols());

    for (std::size_t j = 0; j < b.cols(); ++j) {
        for (std::size_t i = 0; i < a.rows(); ++i) {
            c(i, j) = value_type(0);
            for (std::size_t k = 0; k < a.cols(); ++k) {
                c(i, j) += a(i, k) * b(k, j);
            }
        }
    }
}

// Matrix-vector multiplication.
template <class A1, class A2, class A3>
void mv_mul(const A1& a, const A2& v, A3& w)
{
    using value_type = typename A1::value_type;

    Expects(A1::rank == 2);
    Expects(A2::rank == 1);
    Expects(A3::rank == 1);
    Expects(v.size() == a.cols());

    w.resize(a.rows());
    w = value_type(0);

    for (std::size_t j = 0; j < a.cols(); ++j) {
        for (std::size_t i = 0; i < a.rows(); ++i) {
            w(i) += a(i, j) * v(j);
        }
    }
}

//------------------------------------------------------------------------------

// Algorithms:

// Swap arrays.
template <class T, std::size_t N>
inline void swap(Array<T, N>& a, Array<T, N>& b)
{
    if (a != b) {
        a.swap(b);
    }
}

// Sort vector.
template <class T>
inline void sort(Array<T, 1>& vec, bool ascending = true)
{
    if (ascending) {
        std::sort(vec.begin(), vec.end(), std::less<T>());
    }
    else {  // descending
        std::sort(vec.begin(), vec.end(), std::greater<T>());
    }
}

// Sort matrix.
template <typename T>
void sort(Array<T, 2>& a, int dim = 2, bool ascending = true)
{
    if (dim == 1) {  // sort elements along each row
        if (ascending) {
            for (std::size_t i = 0; i < a.rows(); ++i) {
                auto ri = a.row(i);
                std::sort(ri.begin(), ri.end(), std::less<T>());
            }
        }
        else {
            for (std::size_t i = 0; i < a.rows(); ++i) {
                auto ri = a.row(i);
                std::sort(ri.begin(), ri.end(), std::greater<T>());
            }
        }
    }
    else {  // sort elements along each column
        if (ascending) {
            for (std::size_t j = 0; j < a.cols(); ++j) {
                auto cj = a.column(j);
                std::sort(cj.begin(), cj.end(), std::less<T>());
            }
        }
        else {
            for (std::size_t j = 0; j < a.cols(); ++j) {
                auto cj = a.column(j);
                std::sort(cj.begin(), cj.end(), std::greater<T>());
            }
        }
    }
}

}  // namespace srs

#endif  // SRS_ARRAY_OPR_H
