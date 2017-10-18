////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#ifndef SRS_MATH_LINALG_H
#define SRS_MATH_LINALG_H

#include <mkl.h>
#include <srs/array.h>
#include <srs/types.h>
#include <algorithm>
#include <gsl/gsl>
#include <limits>
#include <numeric>


//
// Provides linear algebra methods.
//
namespace srs {

// Create special vectors and matrices:

// Create identity matrix.
inline dmatrix identity(int n)
{
    Expects(n > 0);
    dmatrix result(n, n, 0.0);
    result.diag() = 1.0;
    return result;
}

// Create Hilbert matrix.
dmatrix hilbert(int n);

// Create a vector with elements set to random integer values in the
// interval [a, b].
ivector randi(int n, int a = 0, int b = std::numeric_limits<int>::max());

// Create a vector with elements set to real random uniform distribution values
// in the interval [0, 1).
dvector randu(int n);

// Create a M x N matrix with elements set to random integer values in the
// interval [a, b].
imatrix randi(int m, int n, int a = 0, int b = std::numeric_limits<int>::max());

// Create a M x N matrix with elements set to real random uniform distribution
// values in the interval [0, 1).
dmatrix randu(int m, int n);

//------------------------------------------------------------------------------

// Find max, min, sum and product of elements:

template <class T>
inline T max(const Array<T, 1>& vec)
{
    return *std::max_element(vec.begin(), vec.end());
}

template <class T>
inline T min(const Array<T, 1>& vec)
{
    return *std::min_element(vec.begin(), vec.end());
}

template <class T>
Array<T, 1> max(const Array<T, 2>& a, int dim = 2)
{
    using size_type = typename Array<T, 2>::size_type;

    Array<T, 1> result;
    if (dim == 1) {  // row
        result.resize(a.rows());
        for (size_type i = 0; i < a.rows(); ++i) {
            auto ri   = a.row(i);
            result(i) = *std::max_element(ri.begin(), ri.end());
        }
    }
    else {  // column
        result.resize(a.cols());
        for (size_type j = 0; j < a.cols(); ++j) {
            auto rj   = a.column(j);
            result(j) = *std::max_element(rj.begin(), rj.end());
        }
    }
    return result;
}

template <class T>
Array<T, 1> min(const Array<T, 2>& a, int dim = 2)
{
    using size_type = typename Array<T, 2>::size_type;

    Array<T, 1> result;
    if (dim == 1) {
        result.resize(a.rows());
        for (size_type i = 0; i < a.rows(); ++i) {
            auto ri   = a.row(i);
            result(i) = *std::min_element(ri.begin(), ri.end());
        }
    }
    else {
        result.resize(a.cols());
        for (size_type j = 0; j < a.cols(); ++j) {
            auto rj   = a.column(j);
            result(j) = *std::min_element(rj.begin(), rj.end());
        }
    }
    return result;
}

template <class T>
inline T sum(const Array<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(0));
}

template <class T>
inline T sum(const Array_ref<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(0));
}

template <class T>
inline T sum(const Array_ref<const T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(0));
}

template <class T>
Array<T, 1> sum(const Array<T, 2>& a, int dim = 2)
{
    using size_type = typename Array<T, 2>::size_type;

    Array<T, 1> result;
    if (dim == 1) {  // sum element along each row
        result.resize(a.rows());
        for (size_type i = 0; i < a.rows(); ++i) {
            result(i) = sum(a.row(i));
        }
    }
    else {  // sum element along each column
        result.resize(a.cols());
        for (size_type j = 0; j < a.cols(); ++j) {
            result(j) = sum(a.column(j));
        }
    }
    return result;
}

template <class T>
inline T prod(const Array<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(1), std::multiplies<T>());
}

template <class T>
inline T prod(const Array_ref<T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(1), std::multiplies<T>());
}

template <class T>
inline T prod(const Array_ref<const T, 1>& vec)
{
    return std::accumulate(vec.begin(), vec.end(), T(1), std::multiplies<T>());
}

template <class T>
Array<T, 1> prod(const Array<T, 2>& a, int dim = 2)
{
    using size_type = typename Array<T, 2>::size_type;

    Array<T, 1> result;
    if (dim == 1) {  // product of elements along each row
        result.resize(a.rows());
        for (size_type i = 0; i < a.rows(); ++i) {
            result(i) = prod(a.row(i));
        }
    }
    else {  // product of elements along each column
        result.resize(a.cols());
        for (size_type j = 0; j < a.cols(); ++j) {
            result(j) = prod(a.column(j));
        }
    }
    return result;
}

//------------------------------------------------------------------------------

// Compute trace of N x N matrix:

template <class T>
inline T trace(const Array<T, 2>& a)
{
    Expects(a.rows() == a.cols());
    const auto d = a.diag();
    return std::accumulate(d.begin(), d.end(), T(0));
}

template <class T>
inline T trace(const Array_ref<T, 2>& a)
{
    Expects(a.rows() == a.cols());
    const auto d = a.diag();
    return std::accumulate(d.begin(), d.end(), T(0));
}

template <class T>
inline T trace(const Array_ref<const T, 2>& a)
{
    Expects(a.rows() == a.cols());
    const auto d = a.diag();
    return std::accumulate(d.begin(), d.end(), T(0));
}

//------------------------------------------------------------------------------

// Compute vector and matrix norm:

// Compute the norm of a vector.
template <class T>
inline T norm(const Array<T, 1>& vec, int p = srs::L2)
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

// Compute matrix norm.
template <class T>
T norm(const Array<T, 2>& a, int p = srs::Fro)
{
    using size_type = typename Array<T, 2>::size_type;

    T pnorm = T(0);
    T tsum  = T(0);

    if (!a.empty()) {
        if (p == srs::Fro) {  // Frobenius norm
            for (size_type j = 0; j < a.cols(); ++j) {
                for (size_type i = 0; i < a.rows(); ++i) {
                    tsum += a(i, j) * a(i, j);
                }
            }
            pnorm = std::sqrt(tsum);
        }
        else if (p == srs::L1) {  // maximum absolute column sum norm (L1 norm)
            for (size_type j = 0; j < a.cols(); ++j) {
                tsum    = T(0);
                auto cj = a.column(j);
                for (const auto& v : cj) {
                    tsum += std::abs(v);
                }
                if (tsum > pnorm) {
                    pnorm = tsum;
                }
            }
        }
        else {  // maximum absolute row sum norm (L-infinity norm)
            for (size_type i = 0; i < a.rows(); ++i) {
                tsum    = T(0);
                auto ri = a.row(i);
                for (const auto& v : ri) {
                    tsum += std::abs(v);
                }
                if (tsum > pnorm) {
                    pnorm = tsum;
                }
            }
        }
    }
    return pnorm;
}

// Compute normalized vector.
inline dvector normalize(const dvector& vec)
{
    dvector result(vec);
    return result /= norm(vec);
}

//------------------------------------------------------------------------------

// Vector dot and cross products:

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

//------------------------------------------------------------------------------

// Compute vector-scalar product and add the result to a vector.
template <class T>
void axpy(const T& a, const Array<T, 1>& x, Array<T, 1>& y)
{
    Expects(x.size() == y.size());

    using size_type = typename Array<T, 1>::size_type;

    for (size_type i = 0; i < x.size(); ++i) {
        y(i) = a * x(i) + y(i);
    }
}

//------------------------------------------------------------------------------

// Determinant of a matrix.
double det(const dmatrix& a);

// Matrix inversion.
void inv(dmatrix& a);

//------------------------------------------------------------------------------

// Matrix decomposition:

// Compute LU factorization.
void lu(dmatrix& a, ivector& ipiv);

//------------------------------------------------------------------------------

// Eigensolvers:

// Compute eigenvalues and eigenvectors of a real symmetric matrix.
void eigs(dmatrix& a, dvector& wr);

// Compute eigenvalues and eigenvectors of a real non-symmetric matrix.
void eig(dmatrix& a, zmatrix& v, zvector& w);

// Compute eigenvalues and eigenvectors of a real symmetric matrix.
void jacobi(dmatrix& a, dvector& wr);

//------------------------------------------------------------------------------

// Solve linear system of equations.
void linsolve(dmatrix& a, dmatrix& b);

//------------------------------------------------------------------------------

// Wrappers to Intel MKL for fast numerical performance for large arrays:

// Compute vector-scalar product and add the result to a vector.
inline void mkl_daxpy(double a, const dvector& x, dvector& y)
{
    Expects(x.size() == y.size());
    MKL_INT n    = x.size();
    MKL_INT incx = 1;
    MKL_INT incy = 1;
    cblas_daxpy(n, a, x.data(), incx, y.data(), incy);
}

// Compute dot product.
inline double mkl_ddot(const dvector& x, const dvector& y)
{
    Expects(x.size() == y.size());
    MKL_INT n    = x.size();
    MKL_INT incx = 1;
    MKL_INT incy = 1;
    return cblas_ddot(n, x.data(), incx, y.data(), incy);
}

// Matrix-matrix multiplication.
void mkl_dgemm(const std::string& transa,
               const std::string& transb,
               const double alpha,
               const dmatrix& a,
               const dmatrix& b,
               const double beta,
               dmatrix& c);

// Matrix-vector multiplication.
void mkl_dgemv(const std::string& transa,
               const double alpha,
               const dmatrix& a,
               const dvector& x,
               const double beta,
               dvector& y);

// Matrix transpose.
inline void mkl_transpose(const dmatrix& a, dmatrix& b)
{
    MKL_INT nrows = a.cols();
    MKL_INT ncols = a.rows();
    MKL_INT lda   = a.rows();
    MKL_INT ldb   = ncols;

    b.resize(nrows, ncols);
    mkl_domatcopy('C', 'T', nrows, ncols, 1.0, a.data(), lda, b.data(), ldb);
}

}  // namespace srs

#endif  // SRS_MATH_LINALG_H
