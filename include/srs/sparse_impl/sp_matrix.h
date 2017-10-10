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

#ifndef SRS_SP_MATRIX_H
#define SRS_SP_MATRIX_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <gsl/gsl>
#include <vector>


namespace srs {

template <class T>
class Sp_matrix {
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Sp_matrix() : elems(), cols(), row_indx(), zero(0) {}

    Sp_matrix(size_type nrows,
              size_type ncols,
              const std::vector<T>& elems_,
              const std::vector<size_type>& cols_,
              const std::vector<size_type>& row_indx_);

    // Iterators:

    iterator begin() { return elems.begin(); }
    const_iterator begin() const { return elems.begin(); }

    iterator end() { return elems.end(); }
    const_iterator end() const { return elems.end(); }

    // Element access:

    T& at(size_type i, size_type j);
    const T& at(size_type i, size_type j) const;

    T& operator()(size_type i, size_type j) { return at(i, j); }
    const T& operator()(size_type i, size_type j) const { return at(i, j); }

    // Capacity:

    bool empty() const { return elems.empty(); }
    size_type num_nonzero() const { return elems.size(); }
    size_type rows() const { return extents[0]; }
    size_type cols() const { return extents[1]; }
    size_type dim1() const { return extents[0]; }
    size_type dim2() const { return extents[1]; }
    size_type extent() const;
    size_type size() const { return extents[0] * extents[1]; }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }

    // Modifiers:

    void clear();
    void swap(const Sp_matrix& m);

    // Access underlying arrays:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

    size_type* columns() { return cols.data(); }
    const size_type* columns() const { return cols.data(); }

    size_type* row_index() { return row_indx.data(); }
    const size_type* row_index() const { return row_indx.data(); }

private:
    std::vector<T> elems;
    std::vector<size_type> cols;
    std::vector<size_type> row_indx;
    std::array<size_type, 2> extents;
    T zero;

    T& ref(size_type i, size_type j);
    const T& ref(size_type i, size_type j) const;
};

template <class T>
Sp_matrix<T>::Sp_matrix(size_type nrows,
                        size_type ncols,
                        const std::vector<T>& elems_,
                        const std::vector<size_type>& cols_,
                        const std::vector<size_type>& row_indx_)
    : elems(elems_),
      cols(cols_),
      row_indx(row_indx_),
      extents{nrows, ncols},
      zero(0)
{
    Ensures(elems.size() == cols.size());
    Ensures(row_indx.size() == nrows + 1);
}

template <class T>
inline T& Sp_matrix<T>::at(size_type i, size_type j)
{
    Expects(i >= 0 && i < extents[0]);
    Expects(j >= 0 && j < extents[1]);
    return ref(i, j);
}

template <class T>
inline const T& Sp_matrix<T>::at(size_type i, size_type j) const
{
    Expects(i >= 0 && i < extents[0]);
    Expects(j >= 0 && j < extents[1]);
    return ref(i, j);
}

template <class T>
inline T& Sp_matrix<T>::operator()(size_type i, size_type j)
{
#ifndef NDEBUG
    return at(i, j);
#else
    return ref(i, j);
#endif
}

template <class T>
inline const T& Sp_matrix<T>::operator()(size_type i, size_type j) const
{
#ifndef NDEBUG
    return at(i, j);
#else
    return ref(i, j);
#endif
}

template <class T>
inline std::size_t Sp_matrix<T>::extent(size_type dim) const
{
    Expects(dim >= 0 && dim < 2);
    return extents[dim];
}

template <class T>
inline void Sp_matrix<T>::clear()
{
    elems.clear();
    cols.clear();
    row_indx.clear();
    extents = {0, 0};
}

template <class T>
inline void Sp_matrix<T>::swap(const Sp_matrix& m)
{
    elems.swap(m.elems);
    cols.swap(m.cols);
    row_indx.swap(m.row_indx);
    std::swap(extents, m.extents);
}

template <class T>
inline T& Sp_matrix<T>::ref(size_type i, size_type j)
{
    T result = zero;
    for (size_type k = row_indx[i]; k < row_indx[i + 1]; ++k) {
        if (cols[k] == j) {
            result = elems[k];
            break;
        }
    }
    return result;
}

template <class T>
inline const T& Sp_matrix<T>::ref(size_type i, size_type j) const
{
    T result = zero;
    for (size_type k = row_indx[i]; k < row_indx[i + 1]; ++k) {
        if (cols[k] == j) {
            result = elems[k];
            break;
        }
    }
    return result;
}

}  // namespace srs

#endif  // SRS_SP_MATRIX_H
