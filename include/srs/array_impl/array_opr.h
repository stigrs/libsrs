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

// Aritmetic operators:

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array<T, N>& a, const Array<T, N>& b)
{
    Array<T, N> result(a);
    return result += b;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array<T, N>& a, const Array<T, N>& b)
{
    Array<T, N> result(a);
    return result -= b;
}

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

template <class T>
inline Array<T, 2> operator*(const Array<T, 2>& a, const Array<T, 2>& b)
{
    return mm_mul(a, b);
}

template <class T>
inline Array<T, 1> operator*(const Array<T, 2>& a, const Array<T, 1>& v)
{
    return mv_mul(a, v);
}

//------------------------------------------------------------------------------

// Mathematical functions:

// Sort vector.
template <class T>
inline void sort(srs::Array<T, 1>& vec, bool ascending = true)
{
    if (ascending) {
        std::sort(vec.begin(), vec.end(), std::less<T>());
    }
    else {  // descending
        std::sort(vec.begin(), vec.end(), std::greater<T>());
    }
}

// Find maximum element.
template <class T>
inline T max(const srs::Array<T, 1>& vec)
{
    return *std::max_element(vec.begin(), vec.end());
}

// Find minimum element.
template <class T>
inline T min(const srs::Array<T, 1>& vec)
{
    return *std::min_element(vec.begin(), vec.end());
}

// Compute sum of elements.
template <class T>
inline T sum(const srs::Array<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(0));
}

// Compute product of elements.
template <class T>
inline T prod(const srs::Array<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(1), std::multiplies<T>());
}

// Compute the norm of a vector.
template <class T>
inline T norm(const srs::Array<T, 1>& vec, int p = 2)
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
inline T dot(const srs::Array<T, 1>& a, const srs::Array<T, 1>& b)
{
    // Intel MKL will be faster for large N.
    Expects(a.size() == b.size());
    return std::inner_product(a.begin(), a.end(), b.begin(), T(0));
}

// Compute cross product.
template <class T>
inline srs::Array<T, 1> cross(const srs::Array<T, 1>& a,
                              const srs::Array<T, 1>& b)
{
    Expects(a.size() == b.size() && a.size() == 3);
    srs::Array<T, 1> result(3);
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
template <class T>
inline Array<T, 2> mm_mul(const Array<T, 2>& a, const Array<T, 2>& b)
{
    Array<T, 2> result;
    mm_mul(a, b, result);
    return result;
}

// Matrix-matrix multiplication.
template <class T>
void mm_mul(const Array<T, 2>& a, const Array<T, 2>& b, Array<T, 2>& c)
{
    typedef typename Array<T, 2>::size_type size_type;

    Expects(a.cols() == b.rows());

    c.resize(a.rows(), b.cols());
    c = T(0);

    size_type astr = a.rows();
    size_type bstr = b.rows();
    size_type cstr = c.rows();

    for (size_type j = 0; j < b.cols(); ++j) {
        for (size_type k = 0; k < a.cols(); ++k) {
            T b_kj = b.data()[k + j * bstr];  // help the compiler
            if (b_kj != T(0)) {
                for (size_type i = 0; i < a.rows(); ++i) {
                    // c(i,j) += a(i,k) * b(k,j);
                    c.data()[i + j * cstr] += a.data()[i + k * astr] * b_kj;
                }
            }
        }
    }
}

// Matrix-vector multiplication.
template <class T>
inline Array<T, 1> mv_mul(const Array<T, 2>& a, const Array<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}


// Matrix-vector multiplication.
template <class T>
void mv_mul(const Array<T, 2>& a, const Array<T, 1>& v, Array<T, 1>& w)
{
    typedef typename Array<T, 2>::size_type size_type;

    Expects(v.size() == a.cols());

    w.resize(a.rows());
    w = T(0);

    size_type astr = a.rows();

    for (size_type j = 0; j < a.cols(); ++j) {
        T vj = v(j);
        if (vj != T(0)) {
            for (size_type i = 0; i < a.rows(); ++i) {
                // w(i) += a(i,j) * v[j];
                w(i) += a.data()[i + j * astr] * vj;  // help the compiler
            }
        }
    }
}

//------------------------------------------------------------------------------

// Algorithms:

template <class T, std::size_t N>
inline void swap(Array<T, N>& a, Array<T, N>& b)
{
    if (a != b) {
        a.swap(b);
    }
}

}  // namespace srs

#endif  // SRS_ARRAY_OPR_H
