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

#ifndef SRS_SPARSE_OPR_H
#define SRS_SPARSE_OPR_H

#include <srs/array.h>
#include <srs/sparse_impl/sp_vector.h>
#include <vector>


namespace srs {

// Convert formats:

// Gather a full-storage vector into compressed form.
template <class T>
Sp_vector<T> sp_gather(const Array<T, 1>& y)
{
    std::vector<T> val;
    std::vector<std::size_t> loc;
    for (std::size_t i = 0; i < y.size(); ++i) {
        if (y(i) != T(0)) {
            val.push_back(y(i));
            loc.push_back(i);
        }
    }
    return {val, loc};
}

// Gather a full-storage matrix into CSR3 format.
template <class T>
Sp_matrix<T> sp_gather(const Array<T, 2>& a)
{
    std::vector<T> elems;
    std::vector<std::size_t> col_indx;
    std::vector<std::size_t> row_ptr(a.rows() + 1);
    std::size_t nnz = 0;
    for (std::size_t i = 0; i < a.rows(); ++i) {
        std::size_t inz = 0;
        for (std::size_t j = 0; j < a.cols(); ++j) {
            if (a(i, j) != T(0)) {
                elems.push_back(a(i, j));
                col_indx.push_back(j);
                nnz++;
                inz++;
            }
        }
        row_ptr[i] = nnz - inz;
    }
    row_ptr[row_ptr.size() - 1] = nnz;
    return {a.rows(), a.cols(), elems, col_indx, row_ptr};
}

// Scatter a sparse vector into full-storage form.
template <class T>
Array<T, 1> sp_scatter(const Sp_vector<T>& x)
{
    Array<T, 1> result(x.size());
    for (std::size_t i = 0; i < result.size(); ++i) {
        result(i) = x(i);
    }
    return result;
}

// Scatter a sparse matrix into full-storage form.
template <class T>
Array<T, 2> sp_scatter(const Sp_matrix<T>& a)
{
    Array<T, 2> result(a.rows(), a.cols());
    for (std::size_t i = 0; i < result.rows(); ++i) {
        for (std::size_t j = 0; j < result.cols(); ++j) {
            result(i, j) = a(i, j);
        }
    }
    return result;
}

//------------------------------------------------------------------------------

// Sparse vector math functions:

template <class T>
inline T norm(const Sp_vector<T>& x)
{
    return x.norm();
}

template <class T>
inline T dot(const Sp_vector<T>& x, const Array<T, 1>& y)
{
    return x.dot(y);
}

template <class T>
inline T dot(const Array<T, 1>& y, const Sp_vector<T>& x)
{
    return x.dot(y);
}

template <class T>
inline T dot(const Sp_vector<T>& x, const std::vector<T>& y)
{
    return x.dot(y);
}

template <class T>
inline T dot(const std::vector<T>& y, const Sp_vector<T>& x)
{
    return x.dot(y);
}

//------------------------------------------------------------------------------

// Scalar multiplication:

template <class T>
Sp_vector<T> operator*(const Sp_vector<T>& x, const T& scalar)
{
    Sp_vector<T> result(x);
    return result *= scalar;
}

template <class T>
Sp_vector<T> operator*(const T& scalar, const Sp_vector<T>& x)
{
    Sp_vector<T> result(x);
    return result *= scalar;
}

template <class T>
Sp_matrix<T> operator*(const Sp_matrix<T>& a, const T& scalar)
{
    Sp_matrix<T> result(a);
    return result *= scalar;
}

template <class T>
Sp_matrix<T> operator*(const T& scalar, const Sp_matrix<T>& a)
{
    Sp_matrix<T> result(a);
    return result *= scalar;
}

//------------------------------------------------------------------------------

// Vector addition:

template <class T>
Array<T, 1> operator+(const Sp_vector<T>& x, const Array<T, 1>& y)
{
    Expects(x.size() <= y.size());
    Array<T, 1> result(y);
    std::size_t i = 0;
    for (const auto& v : x) {
        result(x.loc(i)) += v;
        ++i;
    }
    return result;
}

template <class T>
Array<T, 1> operator+(const Array<T, 1>& y, const Sp_vector<T>& x)
{
    Expects(x.size() <= y.size());
    Array<T, 1> result(y);
    std::size_t i = 0;
    for (const auto& v : x) {
        result(x.loc(i)) += v;
        ++i;
    }
    return result;
}

//------------------------------------------------------------------------------

// Vector subtraction:

template <class T>
Array<T, 1> operator-(const Sp_vector<T>& x, const Array<T, 1>& y)
{
    Expects(x.size() <= y.size());
    Array<T, 1> result(y);
    std::size_t i = 0;
    for (const auto& v : x) {
        result(x.loc(i)) -= v;
        ++i;
    }
    return result;
}

template <class T>
Array<T, 1> operator-(const Array<T, 1>& y, const Sp_vector<T>& x)
{
    Expects(x.size() <= y.size());
    Array<T, 1> result(y);
    std::size_t i = 0;
    for (const auto& v : x) {
        result(x.loc(i)) -= v;
        ++i;
    }
    return result;
}

//------------------------------------------------------------------------------

// Matrix-vector product of a sparse matrix.
template <class T>
inline Array<T, 1> operator*(const Sp_matrix<T>& a, const Array<T, 1>& x)
{
    Expects(x.size() == a.cols());
    Array<T, 1> result(a.cols());
    mv_mul(a, x, result);
    return result;
}

// Matrix-vector product of a sparse matrix.
template <class T>
void mv_mul(const Sp_matrix<T>& a, const Array<T, 1>& x, Array<T, 1>& result)
{
    Expects(x.size() == a.cols());
    result.resize(a.cols());

    for (std::size_t i = 0; i < a.rows(); ++i) {
        T sum = T(0);
        for (std::size_t j = a.row_index()[i]; j < a.row_index()[i + 1]; ++j) {
            sum += a.values()[j] * x(a.columns()[j]);
        }
        result(i) = sum;
    }
}

}  // namespace srs

#endif  // SRS_SPARSE_OPR_H
