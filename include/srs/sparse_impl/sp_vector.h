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

#include <srs/array_impl/functors.h>
#include <algorithm>
#include <cstddef>
#include <gsl/gsl>
#include <iterator>
#include <utility>
#include <vector>


namespace srs {

//
// Sparse vector class.
//
// Note:
// - It is assumed that the sparse vector is initialized with element indices
//   sorted in ascending order.
// - New elements are inserted so that the index order is preserved.
//
template <class T>
class Sp_vector {
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    Sp_vector() : elems(), indx() {}

    explicit Sp_vector(size_type n) : elems(n), indx(n) {}

    Sp_vector(const std::vector<T>& val, const std::vector<size_type>& loc)
        : elems(val), indx(loc)
    {
    }

    Sp_vector(std::initializer_list<std::pair<T, size_type>> list);

    // Assignment:
    Sp_vector& operator=(std::initializer_list<std::pair<T, size_type>> list);

    // Iterators:
    iterator begin() { return elems.begin(); }
    const_iterator begin() const { return elems.begin(); }

    iterator end() { return elems.end(); }
    const_iterator end() const { return elems.end(); }

    // Element access:
    T& at(size_type i);
    const T& at(size_type i) const;

    T& operator()(size_type i) { return at(i); }
    const T& operator()(size_type i) const { return at(i); }

    // Capacity:
    bool empty() const { return elems.empty(); }
    size_type size() const { return elems.size(); }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }

    // Modifiers:
    void clear();
    void insert(const T& value, size_type i);
    void swap(const Sp_vector& vec);
    void resize(size_type n);

    // Access underlying arrays:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

    size_type* index() { return indx.data(); }
    const size_type* index() const { return indx.data(); }

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
    static const T zero = T(0);
};

template <class T>
Sp_vector<T>::Sp_vector(std::initializer_list<std::pair<T, size_type>> list)
    : elems(list.size()), indx(list.size())
{
    size_type i = 0;
    for (const auto& il : list) {
        elems[i] = std::get<0>(il);
        indx[i]  = std::get<1>(il);
        ++i;
    }
}

template <class T>
Sp_vector<T>& Sp_vector<T>::operator=(
    std::initializer_list<std::pair<T, size_type>> list)
{
    elems.resize(list.size());
    indx.resize(list.size());

    size_type i = 0;
    for (const auto& il : list) {
        elems[i] = std::get<0>(il);
        indx[i]  = std::get<1>(il);
        ++i;
    }
}

template <class T>
inline T& Sp_vector<T>::at(size_type i)
{
    auto pos   = std::find(indx.begin(), indx.end(), i);
    auto index = std::distance(indx.begin(), pos);
    Expects(index >= 0 && index < gsl::narrow_cast<std::ptrdiff_t>(size()));
    return elems[index];
}

template <class T>
inline const T& Sp_vector<T>::at(size_type i) const
{
    auto pos   = std::find(indx.begin(), indx.end(), i);
    auto index = std::distance(indx.begin(), pos);
    Expects(index >= 0 && index < gsl::narrow_cast<std::ptrdiff_t>(size()));
    return elems[index];
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
    auto pos = std::lower_bound(indx.begin(), indx.end(), value);
    elems.insert(pos, value);
    indx.insert(pos, i);
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

}  // namespace srs

#endif  // SRS_SP_VECTOR_H
