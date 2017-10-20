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

#ifndef SRS_PACKED_MATRIX_H
#define SRS_PACKED_MATRIX_H

#include <srs/array_impl/functors.h>
#include <srs/types.h>
#include <gsl/gsl>
#include <vector>


namespace srs {

template <class T>
class Packed_matrix {
public:
    typedef T value_type;
    typedef Int_t size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

    // Constructors:

    Packed_matrix() : elems(), extent{0} {}

    explicit Packed_matrix(size_type n) : elems(n * (n + 1) / 2), extent{n} {}

    Packed_matrix(size_type n, const T& value)
        : elems(n * (n + 1) / 2, value), extent{n}
    {
    }

    Packed_matrix(size_type n, T* ptr);

    template <Int_t np>
    Packed_matrix(size_type n, const T (&a)[np]);

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
    size_type rows() const { return extent; }
    size_type cols() const { return extent; }
    size_type dim1() const { return extent; }
    size_type dim2() const { return extent; }
    size_type size() const { return elems.size(); }
    size_type max_size() const { return elems.max_size(); }
    size_type capacity() const { return elems.capacity(); }

    // Modifiers:

    void clear();
    void swap(const Packed_matrix& ap);
    void resize(size_type n);

    // Access underlying arrays:

    T* data() { return elems.data(); }
    const T* data() const { return elems.data(); }

    // Element-wise operations:

    template <class F>
    Packed_matrix& apply(F f);

    template <class F>
    Packed_matrix& apply(F f, const T& value);

    Packed_matrix& operator=(const T& value);

    Packed_matrix& operator*=(const T& value);
    Packed_matrix& operator/=(const T& value);
    Packed_matrix& operator+=(const T& value);
    Packed_matrix& operator-=(const T& value);

    Packed_matrix& operator-();

    Packed_matrix& operator+=(const Packed_matrix& ap);
    Packed_matrix& operator-=(const Packed_matrix& ap);

private:
    std::vector<T> elems;
    size_type extent;

    T& ref(size_type i, size_type j);
    const T& ref(size_type i, size_type j) const;

    size_type index(size_type i, size_type j) const;
};

template <class T>
Packed_matrix<T>::Packed_matrix(size_type n, T* ptr)
    : elems(n * (n + 1) / 2), extent{n}
{
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = ptr[i];
    }
}

template <class T>
template <Int_t np>
Packed_matrix<T>::Packed_matrix(size_type n, const T (&a)[np])
    : elems(np), extent{n}
{
    Expects(np == n * (n + 1) / 2);
    for (size_type i = 0; i < size(); ++i) {
        elems[i] = a[i];
    }
}

template <class T>
inline T& Packed_matrix<T>::at(size_type i, size_type j)
{
    Expects(i >= 0 && i < extent);
    Expects(j >= 0 && j < extent);
    return ref(i, j);
}

template <class T>
inline const T& Packed_matrix<T>::at(size_type i, size_type j) const
{
    Expects(i >= 0 && i < extent);
    Expects(j >= 0 && j < extent);
    return ref(i, j);
}

template <class T>
inline T& Packed_matrix<T>::operator()(size_type i, size_type j)
{
#ifndef NDEBUG
    return at(i, j);
#else
    return ref(i, j);
#endif
}

template <class T>
inline const T& Packed_matrix<T>::operator()(size_type i, size_type j) const
{
#ifndef NDEBUG
    return at(i, j);
#else
    return ref(i, j);
#endif
}

template <class T>
inline void Packed_matrix<T>::clear()
{
    elems.clear();
    extent = 0;
}

template <class T>
inline void Packed_matrix<T>::swap(const Packed_matrix& ap)
{
    elems.swap(ap.elems);
    std::swap(extent, ap.extent);
}

template <class T>
void Packed_matrix<T>::resize(size_type n)
{
    elems.resize(n * n);
    extent = n;
}

template <class T>
template <class F>
Packed_matrix<T>& Packed_matrix<T>::apply(F f)
{
    for (auto& v : elems) {
        f(v);
    }
    return *this;
}

template <class T>
template <class F>
Packed_matrix<T>& Packed_matrix<T>::apply(F f, const T& value)
{
    for (auto& v : elems) {
        f(v, value);
    }
    return *this;
}

template <class T>
inline Packed_matrix<T>& Packed_matrix<T>::operator=(const T& value)
{
    apply(Assign<T>(), value);
    return *this;
}

template <class T>
inline Packed_matrix<T>& Packed_matrix<T>::operator*=(const T& value)
{
    apply(Mul_assign<T>(), value);
    return *this;
}

template <class T>
inline Packed_matrix<T>& Packed_matrix<T>::operator/=(const T& value)
{
    apply(Div_assign<T>(), value);
    return *this;
}

template <class T>
inline Packed_matrix<T>& Packed_matrix<T>::operator+=(const T& value)
{
    apply(Add_assign<T>(), value);
    return *this;
}

template <class T>
inline Packed_matrix<T>& Packed_matrix<T>::operator-=(const T& value)
{
    apply(Minus_assign<T>(), value);
    return *this;
}

template <class T>
inline Packed_matrix<T>& Packed_matrix<T>::operator-()
{
    apply(Unary_minus<T>());
    return *this;
}

template <class T>
Packed_matrix<T>& Packed_matrix<T>::operator+=(const Packed_matrix<T>& ap)
{
    Expects(extent == ap.extent);
    for (size_type i = 0; i < size(); ++i) {
        elems[i] += ap.data()[i];
    }
    return *this;
}

template <class T>
Packed_matrix<T>& Packed_matrix<T>::operator-=(const Packed_matrix<T>& ap)
{
    Expects(extent == ap.extent);
    for (size_type i = 0; i < size(); ++i) {
        elems[i] -= ap.data()[i];
    }
    return *this;
}

template <class T>
inline T& Packed_matrix<T>::ref(size_type i, size_type j)
{
    if (j < i) {
        return elems[index(j, i)];
    }
    else {
        return elems[index(i, j)];
    }
}

template <class T>
inline const T& Packed_matrix<T>::ref(size_type i, size_type j) const
{
    if (j < i) {
        return elems[index(j, i)];
    }
    else {
        return elems[index(i, j)];
    }
}

template <class T>
inline Int_t Packed_matrix<T>::index(size_type i, size_type j) const
{
    return i + j * (j + 1) / 2;
}

}  // namespace srs

#endif  // SRS_PACKED_MATRIX_H
