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

#include <srs/types.h>
#include <array>
#include <vector>


namespace srs {

template <class T>
class Band_matrix {
public:
    typedef T value_type;
    typedef Int_t size_type;
    typedef std::vector<T>::iterator iterator;
    typedef std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Band_matrix(), elems(0), extents{0, 0}, bwidth{0, 0}, stride{0}, zero{0} {}

    Band_matrix(size_type m, size_type n, size_type kl, size_type ku)
        : elems((kl + ku + 1) * n),
          extents{m, n},
          bwidth{kl, ku},
          stride{m},
          zero{0}
    {
    }

    Band_matrix(
        size_type m, size_type n, size_type kl, size_type ku, const T& value);

    Band_matrix(size_type m, size_type n, size_type kl, size_type ku, T* ptr);

    template <Int_t nb>
    Band_matrix(size_type m,
                size_type n,
                size_type kl,
                size_type ku,
                const T (&ab)[nb]);

    // Iterators:

    iterator begin() { return elems.begin(); }
    const_iterator begin() const { return elems.begin(); }

    iterator end() { return elems.end(); }
    const_iterator end() const { return elems.end(); }

    // Element access:

    T& at(size_type i, size_type j);
    const T& at(size_type i, size_type j) const;

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
      stride{m},
      zero{0}
{
}

template <class T>
Band_matrix<T>::Band_matrix(
    size_type m, size_type n, size_type kl, size_type ku, T* ptr)
    : elems((kl + ku + 1) * n),
      extents{m, n},
      bwidth{kl, ku},
      stride{m},
      zero{0}
{
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = ptr[i];
    }
}

template <class T>
template <Int_t nb>
Band_matrix(
    size_type m, size_type n, size_type kl, size_type ku, const T (&ab)[nb])
    : elems(nb), extents{m, n}, bwidth{kl, ku}, stride{m}, zero{0}
{
    for (size_type i = 0; i < nb; ++i) {
        elems[i] = ab[i];
    }
}

template <class T>
T& Band_matrix<T>::ref(size_type i, size_type j)
{
    if (std::max(0, j - bwidth[1]) <= i
        && i < std::min(extents[0], j + bwidth[0])) {
        return elems[index(i, j)];
    }
    else {
        return zero;
    }
}

template <class T>
const T& Band_matrix<T>::ref(size_type i, size_type j)
{
    if (std::max(0, j - bwidth[1]) <= i
        && i < std::min(extents[0], j + bwidth[0])) {
        return elems[index(i, j)];
    }
    else {
        return zero;
    }
}

template <class T>
Int_t Band_matrix<T>::index(size_type i, size_type j) const
{
    return i + bwidth[1] - j + (j - 2) * stride;
}
}  // namespace srs

#endif  // SRS_BAND_MATRIX_H
