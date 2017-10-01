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

#ifndef SRS_ARRAY2_H
#define SRS_ARRAY2_H

#include <srs/array_impl/array_ref.h>
#include <srs/array_impl/functors.h>
#include <algorithm>
#include <array>
#include <gsl/gsl>
#include <initializer_list>
#include <vector>


namespace srs {

//
// Two-dimensional dense array (matrix) class.
//
template <class T>
class Array<T, 2> {
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Array() : elems(), extents{0, 0}, stride(0) {}

    Array(size_type nrows, size_type ncols)
        : elems(nrows * ncols), extents{nrows, ncols}, stride(nrows)
    {
    }

    Array(size_type nrows, size_type ncols, const T& value)
        : elems(nrows * ncols, value), extents{nrows, ncols}, stride(nrows)
    {
    }

    Array(size_type nrows, size_type ncols, T* ptr);

    template <std::size_t nrows, std::size_t ncols>
    Array(const T (&a)[nrows][ncols]);

    Array(std::initializer_list<std::initializer_list<T>> ilist);

    // Copy elements referenced by array slice.
    Array(const Array_ref<T, 2>& a);

    // T f(const T&) would be a typical type for f.
    template <class F>
    Array(const Array& a, F f);

    // T f(const T&, const Arg&) would be a typical type for f.
    template <class F, class Arg>
    Array(const Array& a, F f, const Arg& value);

    // Assignments:

    Array& operator=(const Array_ref<T, 2>& a);
    Array& operator=(std::initializer_list<std::initializer_list<T>> ilist);

    // Element access:

    T& at(size_type i, size_type j);
    const T& at(size_type i, size_type j) const;

    T& operator()(size_type i, size_type j);
    const T& operator()(size_type i, size_type j) const;

    Array_ref<T, 1> operator[](size_type i) { return row(i); }
    Array<T, 1> operator[](size_type i) const { return row(i); }

    // Iterators:

    iterator begin() { return elems.begin(); }
    iterator end() { return elems.end(); }

    const_iterator begin() const { return elems.begin(); }
    const_iterator end() const { return elems.end(); }

    // Slicing:

    Array_ref<T, 1> row(size_type i);
    Array<T, 1> row(size_type i) const;

    Array_ref<T, 1> column(size_type i);
    Array<T, 1> column(size_type i) const;

    Array_ref<T, 1> diag();
    Array<T, 1> diag() const;

    Array_ref<T, 2> slice(size_type ifirst,
                          size_type ilast,
                          size_type jfirst,
                          size_type jlast);
    Array<T, 2> slice(size_type ifirst,
                      size_type ilast,
                      size_type jfirst,
                      size_type jlast) const;

    // Capacity:

    size_type rank() const { return rank_; }
    size_type order() const { return rank_; }

    bool empty() const { return elems.empty(); }

    size_type size() const { return elems.size(); }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }
    size_type rows() const { return extents[0]; }
    size_type cols() const { return extents[1]; }
    size_type dim1() const { return extents[0]; }
    size_type dim2() const { return extents[1]; }
    size_type extent(size_type dim) const
    {
        Expects(dim >= 0 && dim < rank());
        return extents[dim];
    }

    // Modifiers:

    void clear();

    void swap(Array& a);

    void resize(size_type nrows, size_type ncols);
    void resize(size_type nrows, size_type ncols, const T& value);

    void transpose();

    // Access underlying array:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

    // Element-wise operations:

    template <class F>
    Array& apply(F f);

    template <class F>
    Array& apply(F f, const T& value);

    Array& operator=(const T& value);

    Array& operator*=(const T& value);
    Array& operator/=(const T& value);
    Array& operator%=(const T& value);
    Array& operator+=(const T& value);
    Array& operator-=(const T& value);

    Array& operator&=(const T& value);
    Array& operator|=(const T& value);
    Array& operator^=(const T& value);

    Array& operator!();
    Array& operator-();
    Array& operator~();

    Array& operator+=(const Array& a);
    Array& operator-=(const Array& a);

private:
    std::vector<T> elems;  // storage
    std::array<size_type, 2> extents;
    size_type stride;
    static constexpr size_type rank_ = 2;
};

template <class T>
Array<T, 2>::Array(size_type nrows, size_type ncols, T* ptr)
    : elems(nrows * ncols), extents{nrows, ncols}, stride(nrows)
{
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = ptr[i];
    }
}

template <class T>
template <std::size_t nrows, std::size_t ncols>
Array<T, 2>::Array(const T (&a)[nrows][ncols])
    : elems(nrows * ncols), extents{nrows, ncols}, stride(nrows)
{
    for (size_type i = 0; i < extents[0]; ++i) {
        for (size_type j = 0; j < extents[1]; ++j) {
            (*this)(i, j) = a[i][j];
        }
    }
}

template <class T>
Array<T, 2>::Array(std::initializer_list<std::initializer_list<T>> ilist)
    : elems(ilist.size() * ilist.begin()->size()),
      extents{ilist.size(), ilist.begin()->size()},
      stride(ilist.size())
{
    size_type i = 0;
    size_type j = 0;

    for (const auto& il : ilist) {
        for (const auto& v : il) {
            elems[i + j * stride] = v;
            ++j;
        }
        j = 0;
        ++i;
    }
}

template <class T>
Array<T, 2>::Array(const Array_ref<T, 2>& a)
    : elems(a.rows() * a.cols()), extents{a.rows(), a.cols()}, stride(a.rows())
{
    for (size_type j = 0; j < a.cols(); ++j) {
        for (size_type i = 0; i < a.rows(); ++i) {
            (*this)(i, j) = a(i, j);
        }
    }
}

template <class T>
template <class F>
Array<T, 2>::Array(const Array& a, F f)
{
    Expects(size() == a.size());
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = f(a.data()[i]);
    }
}

template <class T>
template <class F, class Arg>
Array<T, 2>::Array(const Array& a, F f, const Arg& value)
{
    Expects(size() == a.size());
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = f(a.data()[i], value);
    }
}

template <class T>
Array<T, 2>& Array<T, 2>::operator=(const Array_ref<T, 2>& a)
{
    resize(a.rows() * a.cols());
    extents = {a.rows(), a.cols()};
    stride  = a.rows();

    for (size_type j = 0; j < a.cols(); ++j) {
        for (size_type i = 0; i < a.rows(); ++i) {
            (*this)(i, j) = a(i, j);
        }
    }
    return *this;
}

template <class T>
Array<T, 2>& Array<T, 2>::operator=(
    std::initializer_list<std::initializer_list<T>> ilist)
{
    resize(ilist.size() * ilist.begin()->size());
    extents = {ilist.size(), ilist.begin()->size()};
    stride  = ilist.size();

    size_type i = 0;
    size_type j = 0;

    for (const auto& il : ilist) {
        for (const auto& v : il) {
            elems[i + j * stride] = v;
            ++j;
        }
        j = 0;
        ++i;
    }
    return *this;
}

template <class T>
inline T& Array<T, 2>::at(size_type i, size_type j)
{
    Expects(i >= 0 && i < extents[0] && j >= 0 && j < extents[1]);
    return elems[i + j * stride];
}

template <class T>
inline const T& Array<T, 2>::at(size_type i, size_type j) const
{
    Expects(i >= 0 && i < extents[0] && j >= 0 && j < extents[1]);
    return elems[i + j * stride];
}

template <class T>
inline T& Array<T, 2>::operator()(size_type i, size_type j)
{
#ifdef NDEBUG
    return elems[i + j * stride];
#else
    return at(i, j);
#endif
}

template <class T>
inline const T& Array<T, 2>::operator()(size_type i, size_type j) const
{
#ifdef NDEBUG
    return elems[i + j * stride];
#else
    return at(i, j);
#endif
}

template <class T>
inline Array_ref<T, 1> Array<T, 2>::row(size_type i)
{
    Expects(i >= 0 && i < extents[0]);
    return Array_ref<T, 1>(extents[1], stride, data() + i);
}

template <class T>
inline Array<T, 1> Array<T, 2>::row(size_type i) const
{
    Expects(i >= 0 && i < extents[0]);
    Array<T, 1> result(extents[1]);
    for (size_type j = 0; j < result.size(); ++j) {
        result(j) = data()[i + j * stride];
    }
    return result;
}

template <class T>
inline Array_ref<T, 1> Array<T, 2>::column(size_type i)
{
    Expects(i >= 0 && i < extents[1]);
    return Array_ref<T, 1>(extents[0], 1, data() + i * stride);
}

template <class T>
inline Array<T, 1> Array<T, 2>::column(size_type i) const
{
    Expects(i >= 0 && i < extents[1]);

    Array<T, 1> result(extents[0]);

    size_type start = i * stride;
    for (size_type j = 0; j < result.size(); ++j) {
        result(j) = data()[start + j];
    }
    return result;
}

template <class T>
inline Array_ref<T, 1> Array<T, 2>::diag()
{
    Expects(extents[0] == extents[1]);
    return Array_ref<T, 1>(extents[0], stride + 1, data());
}

template <class T>
inline Array<T, 1> Array<T, 2>::diag() const
{
    Expects(extents[0] == extents[1]);
    Array<T, 1> result(extents[0]);
    for (size_type j = 0; j < result.size(); ++j) {
        result(j) = data()[j * (stride + 1)];
    }
    return result;
}

template <class T>
inline Array_ref<T, 2> Array<T, 2>::slice(size_type ifirst,
                                          size_type ilast,
                                          size_type jfirst,
                                          size_type jlast)
{
    Expects(ifirst >= 0 && ifirst < ilast && ilast < extents[0]);
    Expects(jfirst >= 0 && jfirst < jlast && jlast < extents[1]);
    return Array_ref<T, 2>(ilast - ifirst + 1,
                           jlast - jfirst + 1,
                           stride,
                           data() + ifirst + jfirst * stride);
}

template <class T>
inline Array<T, 2> Array<T, 2>::slice(size_type ifirst,
                                      size_type ilast,
                                      size_type jfirst,
                                      size_type jlast) const
{
    Expects(ifirst >= 0 && ifirst < ilast && ilast < extents[0]);
    Expects(jfirst >= 0 && jfirst < jlast && jlast < extents[1]);

    Array<T, 2> result(ilast - ifirst + 1, jlast - jfirst + 1);

    size_type start = ifirst + jfirst * stride;
    for (size_type k = 0; k < result.size(); ++k) {
        result.data()[k] = data()[start + k * stride];
    }
    return result;
}

template <class T>
inline void Array<T, 2>::clear()
{
    elems.clear();
    extents = {0, 0};
    stride  = 0;
}

template <class T>
inline void Array<T, 2>::swap(Array<T, 2>& a)
{
    elems.swap(a.elems);
    std::swap(extents, a.extents);
    std::swap(stride, a.stride);
}

template <class T>
inline void Array<T, 2>::resize(size_type nrows, size_type ncols)
{
    elems.resize(nrows * ncols);
    extents = {nrows, ncols};
    stride  = nrows;
}

template <class T>
inline void Array<T, 2>::resize(size_type nrows,
                                size_type ncols,
                                const T& value)
{
    elems.resize(nrows * ncols, value);
    extents = {nrows, ncols};
    stride  = nrows;
}

template <class T>
void Array<T, 2>::transpose()
{
    // If better numerical performance is required, use srs::transpose() or
    // e.g. the mkl_?imatcopy() routine provided by Intel MKL.
    //
    if (extents[0] == extents[1]) {  // array is square
        for (size_type j = 0; j < extents[1] - 1; ++j) {
            for (size_type i = j + 1; i < extents[0]; ++i) {
                std::swap((*this)(i, j), (*this)(j, i));
            }
        }
    }
    else {
        // Algorithm obtained from:
        // http://www.geeksforgeeks.org/inplace-m-x-n-size-matrix-transpose/
        // (accessed 25 Sept 2017)
        size_type sz = size() - 1;

        std::vector<bool> visited(sz);
        visited[0]  = true;
        visited[sz] = true;

        size_type iter = 1;
        while (iter < sz) {
            size_type start = iter;
            T tmp           = elems[iter];
            do {
                size_type next = (iter * extents[1]) % sz;
                std::swap(elems[next], tmp);
                visited[iter] = true;
                iter          = next;
            } while (iter != start);
            for (iter = 1; iter < sz && visited[iter]; ++iter) {
            }
        }
    }
    std::swap(extents[0], extents[1]);
    stride = extents[0];
}

template <class T>
template <class F>
inline Array<T, 2>& Array<T, 2>::apply(F f)
{
    for (auto& v : elems) {
        f(v);
    }
    return *this;
}

template <class T>
template <class F>
inline Array<T, 2>& Array<T, 2>::apply(F f, const T& value)
{
    for (auto& v : elems) {
        f(v, value);
    }
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator=(const T& value)
{
    apply(Assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator*=(const T& value)
{
    apply(Mul_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator/=(const T& value)
{
    apply(Div_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator%=(const T& value)
{
    apply(Mod_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator+=(const T& value)
{
    apply(Add_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator-=(const T& value)
{
    apply(Minus_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator&=(const T& value)
{
    apply(And_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator|=(const T& value)
{
    apply(Or_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator^=(const T& value)
{
    apply(Xor_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator!()
{
    apply(Not<T>());
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator-()
{
    apply(Unary_minus<T>());
    return *this;
}

template <class T>
inline Array<T, 2>& Array<T, 2>::operator~()
{
    apply(Complement<T>());
    return *this;
}

template <class T>
Array<T, 2>& Array<T, 2>::operator+=(const Array<T, 2>& a)
{
    Expects(extents == a.extents);
    for (size_type i = 0; i < size(); ++i) {
        elems[i] += a.data()[i];
    }
    return *this;
}

template <class T>
Array<T, 2>& Array<T, 2>::operator-=(const Array<T, 2>& a)
{
    Expects(extents == a.extents);
    for (size_type i = 0; i < size(); ++i) {
        elems[i] -= a.data()[i];
    }
    return *this;
}

//------------------------------------------------------------------------------

// Non-member function:

// Transpose array.
template <class T>
inline Array<T, 2> transpose(const Array<T, 2>& a)
{
    Array<T, 2> result(a.cols(), a.rows());
    for (std::size_t j = 0; j < result.cols(); ++j) {
        for (std::size_t i = 0; i < result.rows(); ++i) {
            result(i, j) = a.data()[i * a.rows() + j];
        }
    }
    return result;
}

}  // namespace srs

#endif  // SRS_ARRAY2_H
