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

#ifndef SRS_ARRAY1_H
#define SRS_ARRAY1_H

#include <srs/array_impl/array_ref.h>
#include <srs/array_impl/functors.h>
#include <algorithm>
#include <gsl/gsl>
#include <initializer_list>
#include <vector>


namespace srs {

//
// One-dimensional dense array (vector) class.
//
template <class T>
class Array<T, 1> {
public:
    static constexpr std::size_t rank = 1;

    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Array() : elems() {}

    explicit Array(size_type n) : elems(n) {}

    Array(size_type n, const T& value) : elems(n, value) {}

    Array(size_type n, T* ptr);

    template <std::size_t n>
    Array(const T (&a)[n]);

    Array(std::initializer_list<T> ilist) : elems(ilist) {}

    // Copy elements referenced by Array_ref.
    template <class U>
    Array(const Array_ref<U, 1>& a);

    // T f(const T&) would be a typical type for f.
    template <class F>
    Array(const Array& a, F f);

    // T f(const T&, const Arg&) would be a typical type for f.
    template <class F, class Arg>
    Array(const Array& a, F f, const Arg& value);

    // Assignments:

    template <class U>
    Array& operator=(const Array_ref<U, 1>& a);

    Array& operator=(std::initializer_list<T> ilist);

    // Element access:

    T& at(size_type i) { return elems.at(i); }
    const T& at(size_type i) const { elems.at(i); }

    T& operator()(size_type i);
    const T& operator()(size_type i) const;

    T& operator[](size_type i);
    const T& operator[](size_type i) const;

    // Iterators:

    iterator begin() { return elems.begin(); }
    iterator end() { return elems.end(); }

    const_iterator begin() const { return elems.begin(); }
    const_iterator end() const { return elems.end(); }

    // Slicing:

    Array_ref<T, 1> head(size_type n) { return slice(0, n); }
    Array_ref<const T, 1> head(size_type n) const { return slice(0, n); }

    Array_ref<T, 1> tail(size_type n) { return slice(n, size() - 1); }
    Array_ref<const T, 1> tail(size_type n) const
    {
        return slice(n, size() - 1);
    }

    Array_ref<T, 1> slice(size_type ifirst, size_type ilast);
    Array_ref<const T, 1> slice(size_type ifirst, size_type ilast) const;

    // Capacity:

    bool empty() const { return elems.empty(); }

    size_type size() const { return elems.size(); }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }

    // Modifiers:

    void clear() { elems.clear(); }

    void swap(Array& a) { elems.swap(a.elems); }

    void resize(size_type n) { elems.resize(n); }
    void resize(size_type n, const T& value) { elems.resize(n, value); }

    void push_back(const T& value) { elems.push_back(value); }
    void push_back(T&& value) { elems.push_back(value); }

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
};

template <class T>
Array<T, 1>::Array(size_type n, T* ptr) : elems(n)
{
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = ptr[i];
    }
}

template <class T>
template <std::size_t n>
Array<T, 1>::Array(const T (&a)[n]) : elems(n)
{
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = a[i];
    }
}

template <class T>
template <class U>
Array<T, 1>::Array(const Array_ref<U, 1>& a) : elems(a.size())
{
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = a[i];
    }
}

template <class T>
template <class F>
Array<T, 1>::Array(const Array& a, F f)
{
    Expects(size() == a.size());
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = f(a[i]);
    }
}

template <class T>
template <class F, class Arg>
Array<T, 1>::Array(const Array& a, F f, const Arg& value)
{
    Expects(size() == a.size());
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = f(a[i], value);
    }
}

template <class T>
template <class U>
Array<T, 1>& Array<T, 1>::operator=(const Array_ref<U, 1>& a)
{
    resize(a.size());

    for (size_type i = 0; i < size(); ++i) {
        elems[i] = a[i];
    }
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator=(std::initializer_list<T> ilist)
{
    elems = ilist;
    return *this;
}

template <class T>
inline T& Array<T, 1>::operator()(size_type i)
{
#ifdef NDEBUG
    return elems[i];
#else
    return elems.at(i);
#endif
}

template <class T>
inline const T& Array<T, 1>::operator()(size_type i) const
{
#ifdef NDEBUG
    return elems[i];
#else
    return elems.at(i);
#endif
}

template <class T>
inline T& Array<T, 1>::operator[](size_type i)
{
#ifdef NDEBUG
    return elems[i];
#else
    return elems.at(i);
#endif
}

template <class T>
inline const T& Array<T, 1>::operator[](size_type i) const
{
#ifdef NDEBUG
    return elems[i];
#else
    return elems.at(i);
#endif
}

template <class T>
inline Array_ref<T, 1> Array<T, 1>::slice(size_type ifirst, size_type ilast)
{
    Expects(ifirst >= 0 && ifirst < ilast && ilast < size());
    return Array_ref<T, 1>(ilast - ifirst + 1, 1, data() + ifirst);
}

template <class T>
inline Array_ref<const T, 1> Array<T, 1>::slice(size_type ifirst,
                                                size_type ilast) const
{
    Expects(ifirst >= 0 && ifirst < ilast && ilast < size());
    return Array_ref<const T, 1>(ilast - ifirst + 1, 1, data() + ifirst);
}

template <class T>
template <class F>
inline Array<T, 1>& Array<T, 1>::apply(F f)
{
    for (auto& v : elems) {
        f(v);
    }
    return *this;
}

template <class T>
template <class F>
inline Array<T, 1>& Array<T, 1>::apply(F f, const T& value)
{
    for (auto& v : elems) {
        f(v, value);
    }
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator=(const T& value)
{
    apply(Assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator*=(const T& value)
{
    apply(Mul_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator/=(const T& value)
{
    apply(Div_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator%=(const T& value)
{
    apply(Mod_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator+=(const T& value)
{
    apply(Add_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator-=(const T& value)
{
    apply(Minus_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator&=(const T& value)
{
    apply(And_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator|=(const T& value)
{
    apply(Or_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator^=(const T& value)
{
    apply(Xor_assign<T>(), value);
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator!()
{
    apply(Not<T>());
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator-()
{
    apply(Unary_minus<T>());
    return *this;
}

template <class T>
inline Array<T, 1>& Array<T, 1>::operator~()
{
    apply(Complement<T>());
    return *this;
}

template <class T>
Array<T, 1>& Array<T, 1>::operator+=(const Array<T, 1>& a)
{
    Expects(size() == a.size());
    for (size_type i = 0; i < size(); ++i) {
        elems[i] += a[i];
    }
    return *this;
}

template <class T>
Array<T, 1>& Array<T, 1>::operator-=(const Array<T, 1>& a)
{
    Expects(size() == a.size());
    for (size_type i = 0; i < size(); ++i) {
        elems[i] -= a[i];
    }
    return *this;
}

}  // namespace srs

#endif  // SRS_ARRAY1_H
