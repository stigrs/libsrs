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

#ifndef SRS_SP_VECTOR_H
#define SRS_SP_VECTOR_H

#include <srs/array.h>
#include <srs/array_impl/functors.h>
#include <srs/types.h>
#include <algorithm>
#include <cmath>
#include <gsl/gsl>
#include <initializer_list>
#include <utility>
#include <vector>


namespace srs {

//
// Range-checked sparse vector class (zero-based indexing).
//
// Note:
// - It is assumed that the sparse vector is initialized with element indices
//   sorted in ascending order.
// - New elements are inserted so that the index order is preserved.
// - Array indexing uses signed integers (int).
//
template <class T>
class Sp_vector {
public:
    typedef T value_type;
    typedef Int_t size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Sp_vector() : elems(), indx(), zero(0) {}

    explicit Sp_vector(size_type n) : elems(n), indx(n), zero(0) {}

    Sp_vector(const std::vector<T>& val, const std::vector<size_type>& loc)
        : elems(val), indx(loc), zero(0)
    {
        Ensures(val.size() == indx.size());
    }

    template <Int_t n>
    Sp_vector(const T (&val)[n], const Int_t (&loc)[n]);

    Sp_vector(std::initializer_list<std::pair<size_type, T>> list);

    // Assignment:
    Sp_vector& operator=(std::initializer_list<std::pair<size_type, T>> list);

    // Iterators:

    iterator begin() { return elems.begin(); }
    const_iterator begin() const { return elems.begin(); }

    iterator end() { return elems.end(); }
    const_iterator end() const { return elems.end(); }

    // Element index.
    auto loc(size_type i) const;

    // Element access:

    T& at(size_type i);
    const T& at(size_type i) const;

    T& operator()(size_type i);
    const T& operator()(size_type i) const;

    // Capacity:
    bool empty() const { return elems.empty(); }
    size_type num_nonzero() const { return elems.size(); }
    size_type size() const;
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }

    // Modifiers:

    void clear();
    void insert(const T& value, size_type i);
    void swap(const Sp_vector& x);
    void resize(size_type n);

    // Euclidean norm.
    T norm() const;

    // Dot product.
    T dot(const srs::Array<T, 1>& y);
    T dot(const std::vector<T>& y);

    // Access underlying arrays:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

    const auto& values() const { return elems; }
    const auto& index() const { return indx; }

    const auto& index_zero_based() const { return indx; }
    auto index_one_based() const;

    // Element-wise operations:

    template <class F>
    Sp_vector& apply(F f);

    template <class F>
    Sp_vector& apply(F f, const T& value);

    Sp_vector& operator*=(const T& value);
    Sp_vector& operator/=(const T& value);
    Sp_vector& operator-();

private:
    std::vector<T> elems;
    std::vector<size_type> indx;
    T zero;

    T& ref(size_type i);
    const T& ref(size_type i) const;
};

template <class T>
template <Int_t n>
Sp_vector<T>::Sp_vector(const T (&val)[n], const Int_t (&loc)[n])
    : elems(n), indx(n), zero(0)
{
    for (size_type i = 0; i < n; ++i) {
        elems[i] = val[i];
        indx[i]  = loc[i];
    }
}

template <class T>
Sp_vector<T>::Sp_vector(std::initializer_list<std::pair<size_type, T>> list)
    : elems(list.size()), indx(list.size()), zero(0)
{
    size_type i = 0;
    for (const auto& il : list) {
        elems[i] = std::get<1>(il);
        indx[i]  = std::get<0>(il);
        ++i;
    }
}

template <class T>
Sp_vector<T>& Sp_vector<T>::operator=(
    std::initializer_list<std::pair<size_type, T>> list)
{
    elems.resize(list.size());
    indx.resize(list.size());
    zero = T(0);

    size_type i = 0;
    for (const auto& il : list) {
        elems[i] = std::get<1>(il);
        indx[i]  = std::get<0>(il);
        ++i;
    }
}

template <class T>
inline auto Sp_vector<T>::loc(size_type i) const
{
#ifndef NDEBUG
    Expects(i >= 0 && i < num_nonzero());
#endif
    return indx[i];
}

template <class T>
inline T& Sp_vector<T>::at(size_type i)
{
    Expects(i >= 0);
    return ref(i);
}

template <class T>
inline const T& Sp_vector<T>::at(size_type i) const
{
    Expects(i >= 0);
    return ref(i);
}

template <class T>
inline T& Sp_vector<T>::operator()(size_type i)
{
#ifndef NDEBUG
    return at(i);
#else
    return ref(i);
#endif
}

template <class T>
inline const T& Sp_vector<T>::operator()(size_type i) const
{
#ifndef NDEBUG
    return at(i);
#else
    return ref(i);
#endif
}

template <class T>
inline Int_t Sp_vector<T>::size() const
{
    return *std::max_element(indx.begin(), indx.end()) + 1;
}

template <class T>
inline void Sp_vector<T>::clear()
{
    elems.clear();
    indx.clear();
}

template <class T>
inline void Sp_vector<T>::insert(const T& value, size_type i)
{
    Expects(value != T(0));  // zero values should not be stored

    // Insert should not replace any existing elements.
    if (std::find(indx.begin(), indx.end(), i) == indx.end()) {
        auto pos        = std::upper_bound(indx.begin(), indx.end(), i);
        size_type index = std::distance(indx.begin(), pos);
        elems.insert(elems.begin() + index, value);
        indx.insert(pos, i);
    }
}

template <class T>
inline void Sp_vector<T>::swap(const Sp_vector<T>& x)
{
    elems.swap(x.elems);
    indx.swap(x.indx);
}

template <class T>
inline void Sp_vector<T>::resize(size_type n)
{
    elems.resize(n);
    indx.resize(n);
}

template <class T>
T Sp_vector<T>::norm() const
{
    T result(0);
    for (const auto& v : elems) {
        result += v * v;
    }
    return std::sqrt(result);
}

template <class T>
T Sp_vector<T>::dot(const srs::Array<T, 1>& y)
{
    T result(0);
    size_type i = 0;
    for (const auto& v : elems) {
        result += v * y(indx[i]);
        ++i;
    }
    return result;
}

template <class T>
T Sp_vector<T>::dot(const std::vector<T>& y)
{
    T result(0);
    size_type i = 0;
    for (const auto& v : elems) {
        result += v * y[indx[i]];
        ++i;
    }
    return result;
}

template <class T>
auto Sp_vector<T>::index_one_based() const
{
    auto result = indx;
    for (auto& i : result) {
        i += 1;
    }
    return result;
}

template <class T>
template <class F>
inline Sp_vector<T>& Sp_vector<T>::apply(F f)
{
    for (auto& v : elems) {
        f(v);
    }
    return *this;
}

template <class T>
template <class F>
inline Sp_vector<T>& Sp_vector<T>::apply(F f, const T& value)
{
    for (auto& v : elems) {
        f(v, value);
    }
    return *this;
}

template <class T>
inline Sp_vector<T>& Sp_vector<T>::operator*=(const T& value)
{
    apply(Mul_assign<T>(), value);
    return *this;
}

template <class T>
inline Sp_vector<T>& Sp_vector<T>::operator/=(const T& value)
{
    apply(Div_assign<T>(), value);
    return *this;
}

template <class T>
inline Sp_vector<T>& Sp_vector<T>::operator-()
{
    apply(Unary_minus<T>());
    return *this;
}

template <class T>
inline T& Sp_vector<T>::ref(size_type i)
{
    auto pos        = std::find(indx.begin(), indx.end(), i);
    size_type index = std::distance(indx.begin(), pos);
    return index >= 0 && index < num_nonzero() ? elems[index] : zero;
}

template <class T>
inline const T& Sp_vector<T>::ref(size_type i) const
{
    auto pos        = std::find(indx.begin(), indx.end(), i);
    size_type index = std::distance(indx.begin(), pos);
    return index >= 0 && index < num_nonzero() ? elems[index] : zero;
}

}  // namespace srs

#endif  // SRS_SP_VECTOR_H
