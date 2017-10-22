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

#ifndef SRS_SPARSE_MATRIX_H
#define SRS_SPARSE_MATRIX_H

#include <mkl.h>
#include <srs/array.h>
#include <srs/array_impl/functors.h>
#include <srs/types.h>
#include <algorithm>
#include <array>
#include <gsl/gsl>
#include <vector>


namespace srs {

//
// Range-checked sparse matrix class (zero-based indexing).
//
// Note:
// - Elements are stored in the three array variation of the compressed
//   sparse row (CSR) format.
// - It is assumed that the sparse vector is initialized with element indices
//   sorted in ascending order.
// - New elements are inserted so that the index order is preserved.
// - Array indexing uses signed integers (int).
// - This class provides a framework for implementing sparse matrix methods
//   that utilize the Intel MKL library.
//
template <class T>
class Sparse_matrix {
public:
    typedef T value_type;
    typedef Int_t size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Sparse_matrix() : elems(), col_indx(), row_ptr(), zero(0) {}

    Sparse_matrix(size_type nrows, size_type ncols, size_type nnz);

    Sparse_matrix(size_type nrows,
                  size_type ncols,
                  const std::vector<T>& elems_,
                  const std::vector<size_type>& colind,
                  const std::vector<size_type>& rowptr);

    template <Int_t n, Int_t nnz>
    Sparse_matrix(size_type nrows,
                  size_type ncols,
                  const T (&val)[n],
                  const Int_t (&colind)[n],
                  const Int_t (&rowptr)[nnz]);

    // Iterators:

    iterator begin() { return elems.begin(); }
    const_iterator begin() const { return elems.begin(); }

    iterator end() { return elems.end(); }
    const_iterator end() const { return elems.end(); }

    // Element access:

    T& at(size_type i, size_type j);
    const T& at(size_type i, size_type j) const;

    T& operator()(size_type i, size_type j);
    const T& operator()(size_type i, size_type j) const;

    // Capacity:

    bool empty() const { return elems.empty(); }
    size_type num_nonzero() const { return elems.size(); }
    size_type rows() const { return extents[0]; }
    size_type cols() const { return extents[1]; }
    size_type dim1() const { return extents[0]; }
    size_type dim2() const { return extents[1]; }
    size_type extent(size_type dim) const;
    size_type size() const { return extents[0] * extents[1]; }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }

    // Modifiers:

    void clear();
    void swap(const Sparse_matrix& m);
    void insert(size_type i, size_type j, const T& value);
    void resize(size_type nrows, size_type ncols, size_type nnz);

    // Access underlying arrays:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

    const auto& values() const { return elems; }
    const auto& columns() const { return col_indx; }
    const auto& row_index() const { return row_ptr; }

    const auto& columns_zero_based() const { return col_indx; }
    const auto& row_index_zero_based() const { return row_ptr; }

    auto columns_one_based() const;
    auto row_index_one_based() const;

    // Element-wise operations:

    template <class F>
    Sparse_matrix& apply(F f);

    template <class F>
    Sparse_matrix& apply(F f, const T& value);

    Sparse_matrix& operator*=(const T& value);
    Sparse_matrix& operator/=(const T& value);
    Sparse_matrix& operator-();

private:
    std::vector<T> elems;
    std::vector<size_type> col_indx;
    std::vector<size_type> row_ptr;
    std::array<size_type, 2> extents;
    T zero;

    T& ref(size_type i, size_type j);
    const T& ref(size_type i, size_type j) const;
};

template <class T>
Sparse_matrix<T>::Sparse_matrix(size_type nrows, size_type ncols, size_type nnz)
    : elems(nnz),
      col_indx(nnz),
      row_ptr(nrows + 1),
      extents{nrows, ncols},
      zero(0)
{
}

template <class T>
Sparse_matrix<T>::Sparse_matrix(size_type nrows,
                                size_type ncols,
                                const std::vector<T>& elems_,
                                const std::vector<size_type>& colind,
                                const std::vector<size_type>& rowptr)
    : elems(elems_),
      col_indx(colind),
      row_ptr(rowptr),
      extents{nrows, ncols},
      zero(0)
{
    Ensures(elems.size() == col_indx.size());
    Ensures(row_ptr.size() == gsl::narrow_cast<std::size_t>(nrows + 1));
}

template <class T>
template <Int_t n, Int_t nnz>
Sparse_matrix<T>::Sparse_matrix(size_type nrows,
                                size_type ncols,
                                const T (&val)[n],
                                const Int_t (&colind)[n],
                                const Int_t (&rowptr)[nnz])
    : elems(n), col_indx(n), row_ptr(nnz), extents{nrows, ncols}, zero(0)
{
    for (size_type i = 0; i < n; ++i) {
        elems[i]    = val[i];
        col_indx[i] = colind[i];
    }
    for (size_type i = 0; i < nnz; ++i) {
        row_ptr[i] = rowptr[i];
    }
    Ensures(row_ptr.size() == gsl::narrow_cast<std::size_t>(nrows + 1));
}

template <class T>
inline T& Sparse_matrix<T>::at(size_type i, size_type j)
{
    Expects(i >= 0 && i < extents[0]);
    Expects(j >= 0 && j < extents[1]);
    return ref(i, j);
}

template <class T>
inline const T& Sparse_matrix<T>::at(size_type i, size_type j) const
{
    Expects(i >= 0 && i < extents[0]);
    Expects(j >= 0 && j < extents[1]);
    return ref(i, j);
}

template <class T>
inline T& Sparse_matrix<T>::operator()(size_type i, size_type j)
{
#ifndef NDEBUG
    return at(i, j);
#else
    return ref(i, j);
#endif
}

template <class T>
inline const T& Sparse_matrix<T>::operator()(size_type i, size_type j) const
{
#ifndef NDEBUG
    return at(i, j);
#else
    return ref(i, j);
#endif
}

template <class T>
inline int Sparse_matrix<T>::extent(size_type dim) const
{
    Expects(dim >= 0 && dim < 2);
    return extents[dim];
}

template <class T>
inline void Sparse_matrix<T>::clear()
{
    elems.clear();
    col_indx.clear();
    row_ptr.clear();
    extents = {0, 0};
}

template <class T>
inline void Sparse_matrix<T>::swap(const Sparse_matrix& m)
{
    elems.swap(m.elems);
    col_indx.swap(m.cols);
    row_ptr.swap(m.row_indx);
    std::swap(extents, m.extents);
}

template <class T>
void Sparse_matrix<T>::insert(size_type i, size_type j, const T& value)
{
    if (ref(i, j) == zero) {
        auto pos = std::upper_bound(col_indx.begin() + row_ptr[i],
                                    col_indx.begin() + row_ptr[i + 1],
                                    j);

        size_type index = std::distance(col_indx.begin(), pos);
        elems.insert(elems.begin() + index, value);
        col_indx.insert(pos, j);
        for (std::size_t k = i + 1; k < row_ptr.size(); ++k) {
            row_ptr[k]++;
        }
    }
}

template <class T>
void Sparse_matrix<T>::resize(size_type nrows, size_type ncols, size_type nnz)
{
    elems.resize(nnz);
    col_indx.resize(nnz);
    row_ptr.resize(nrows + 1);
    extents = {nrows, ncols};
}

template <class T>
auto Sparse_matrix<T>::columns_one_based() const
{
    auto result = col_indx;
    for (auto& i : result) {
        i += 1;
    }
    return result;
}

template <class T>
auto Sparse_matrix<T>::row_index_one_based() const
{
    auto result = row_ptr;
    for (auto& i : result) {
        i += 1;
    }
    return result;
}

template <class T>
template <class F>
Sparse_matrix<T>& Sparse_matrix<T>::apply(F f)
{
    for (auto& v : elems) {
        f(v);
    }
    return *this;
}

template <class T>
template <class F>
Sparse_matrix<T>& Sparse_matrix<T>::apply(F f, const T& value)
{
    for (auto& v : elems) {
        f(v, value);
    }
    return *this;
}

template <class T>
inline Sparse_matrix<T>& Sparse_matrix<T>::operator*=(const T& value)
{
    apply(Mul_assign<T>(), value);
    return *this;
}

template <class T>
inline Sparse_matrix<T>& Sparse_matrix<T>::operator/=(const T& value)
{
    apply(Div_assign<T>(), value);
    return *this;
}

template <class T>
inline Sparse_matrix<T>& Sparse_matrix<T>::operator-()
{
    apply(Unary_minus<T>());
    return *this;
}

template <class T>
inline T& Sparse_matrix<T>::ref(size_type i, size_type j)
{
    for (size_type k = row_ptr[i]; k < row_ptr[i + 1]; ++k) {
        if (col_indx[k] == j) {
            return elems[k];
        }
    }
    return zero;
}

template <class T>
inline const T& Sparse_matrix<T>::ref(size_type i, size_type j) const
{
    for (size_type k = row_ptr[i]; k < row_ptr[i + 1]; ++k) {
        if (col_indx[k] == j) {
            return elems[k];
        }
    }
    return zero;
}

}  // namespace srs

#endif  // SRS_SPARSE_MATRIX_H
