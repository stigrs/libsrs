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

#ifndef SRS_BAND_MATRIX_H
#define SRS_BAND_MATRIX_H

#include <srs/array.h>
#include <srs/types.h>
#include <algorithm>
#include <array>
#include <gsl/gsl>
#include <vector>



namespace srs {

template <class T>
class Band_matrix {
public:
    typedef T value_type;
    typedef Int_t size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Band_matrix() : elems(0), extents{0, 0}, bwidth{0, 0}, stride{0}, zero{T(0)}
    {
    }

    Band_matrix(size_type m, size_type n, size_type kl, size_type ku)
        : elems((kl + ku + 1) * n),
          extents{m, n},
          bwidth{kl, ku},
          stride{kl + ku + 1},
          zero{T(0)}
    {
    }

    Band_matrix(
        size_type m, size_type n, size_type kl, size_type ku, const T& value);

    template <Int_t nb>
    Band_matrix(size_type m,
                size_type n,
                size_type kl,
                size_type ku,
                const T (&ab)[nb]);

    template <Int_t n1, Int_t n2>
    Band_matrix(size_type m,
                size_type n,
                size_type kl,
                size_type ku,
                const T (&a)[n1][n2]);

    Band_matrix(size_type kl, size_type ku, const Array<T, 2>& a);

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
    size_type rows() const { return extents[0]; }
    size_type cols() const { return extents[1]; }
    size_type dim1() const { return extents[0]; }
    size_type dim2() const { return extents[1]; }
    size_type size() const { return elems.size(); }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }
    size_type lower() const { return bwidth[0]; }
    size_type upper() const { return bwidth[1]; }

    // Access underlying array:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

private:
    std::vector<T> elems;
    std::array<size_type, 2> extents;
    std::array<size_type, 2> bwidth;
    size_type stride;
    T zero;

    T& ref(size_type i, size_type j);
    const T& ref(size_type i, size_type j) const;

    size_type index(size_type i, size_type j) const;
};

template <class T>
Band_matrix<T>::Band_matrix(
    size_type m, size_type n, size_type kl, size_type ku, const T& value)
    : elems((kl + ku + 1) * n, value),
      extents{m, n},
      bwidth{kl, ku},
      stride{kl + ku + 1},
      zero{T(0)}
{
}

template <class T>
template <Int_t nb>
Band_matrix<T>::Band_matrix(
    size_type m, size_type n, size_type kl, size_type ku, const T (&ab)[nb])
    : elems(nb), extents{m, n}, bwidth{kl, ku}, stride{kl + ku + 1}, zero{T(0)}
{
    Expects(nb >= (kl + ku + 1) * n);
    for (size_type i = 0; i < nb; ++i) {
        elems[i] = ab[i];
    }
}

template <class T>
template <Int_t n1, Int_t n2>
Band_matrix<T>::Band_matrix(
    size_type m, size_type n, size_type kl, size_type ku, const T (&a)[n1][n2])
    : elems((kl + ku + 1) * n),
      extents{m, n},
      bwidth{kl, ku},
      stride{kl + ku + 1},
      zero{T(0)}
{
    for (size_type j = 0; j < n2; ++j) {
        for (size_type i = 0; i < n1; ++i) {
            (*this)(i, j) = a[i][j];
        }
    }
}

template <class T>
Band_matrix<T>::Band_matrix(size_type kl, size_type ku, const Array<T, 2>& a)
    : elems((kl + ku + 1) * a.cols()),
      extents{a.rows(), a.cols()},
      bwidth{kl, ku},
      stride{kl + ku + 1},
      zero{T(0)}
{
    for (size_type j = 0; j < a.cols(); ++j) {
        for (size_type i = 0; i < a.rows(); ++i) {
            (*this)(i, j) = a(i, j);
        }
    }
}

template <class T>
inline T& Band_matrix<T>::at(size_type i, size_type j)
{
    Expects(i >= 0 && i < extents[0]);
    Expects(j >= 0 && j < extents[1]);
    return ref(i, j);
}

template <class T>
inline const T& Band_matrix<T>::at(size_type i, size_type j) const
{
    Expects(i >= 0 && i < extents[0]);
    Expects(j >= 0 && j < extents[1]);
    return ref(i, j);
}

template <class T>
inline T& Band_matrix<T>::operator()(size_type i, size_type j)
{
#ifdef NDEBUG
    return ref(i, j);
#else
    return at(i, j);
#endif
}

template <class T>
inline const T& Band_matrix<T>::operator()(size_type i, size_type j) const
{
#ifdef NDEBUG
    return ref(i, j);
#else
    return at(i, j);
#endif
}

template <class T>
inline T& Band_matrix<T>::ref(size_type i, size_type j)
{
    if (std::max(0, j - bwidth[1]) <= i
        && i < std::min(extents[0], j + bwidth[0] + 1)) {
        return elems[index(i, j)];
    }
    else {
        return zero;
    }
}

template <class T>
inline const T& Band_matrix<T>::ref(size_type i, size_type j) const
{
    if (std::max(0, j - bwidth[1]) <= i
        && i < std::min(extents[0], j + bwidth[0] + 1)) {
        return elems[index(i, j)];
    }
    else {
        return zero;
    }
}

template <class T>
inline Int_t Band_matrix<T>::index(size_type i, size_type j) const
{
    return bwidth[1] + i - j + j * stride;
}

}  // namespace srs

#endif  // SRS_BAND_MATRIX_H
