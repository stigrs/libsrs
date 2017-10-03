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

#ifndef SRS_SLICE_ITER
#define SRS_SLICE_ITER

#include <cstddef>
#include <gsl/gsl>
#include <iterator>


namespace srs {

//
// Slice descriptor.
//
struct Slice {
    Slice(std::size_t start_, std::size_t size_, std::size_t stride_)
        : start{start_}, size{size_}, stride{stride_}
    {
    }

    std::size_t start;
    std::size_t size;
    std::size_t stride;
};

//------------------------------------------------------------------------------

// Forward declarations:

template <class T>
class Slice_iter;

template <class T>
bool operator==(const Slice_iter<T>&, const Slice_iter<T>&);

template <class T>
bool operator!=(const Slice_iter<T>&, const Slice_iter<T>&);

template <class T>
bool operator<(const Slice_iter<T>&, const Slice_iter<T>&);

template <class T>
bool operator>(const Slice_iter<T>&, const Slice_iter<T>&);

template <class T>
bool operator<=(const Slice_iter<T>&, const Slice_iter<T>&);

template <class T>
bool operator>=(const Slice_iter<T>&, const Slice_iter<T>&);

//------------------------------------------------------------------------------

//
// Random access slice iterator class for use by Array_ref class.
//
// This is a modified version of Stroustrup's Slice_iter class (TC++PL, p. 670).
//
template <class T>
class Slice_iter {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::size_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    Slice_iter(T* p, srs::Slice s) : ptr(p), desc(s), curr(s.start) {}

    Slice_iter begin() const
    {
        Slice_iter t = *this;
        return t;
    }

    Slice_iter end() const
    {
        Slice_iter t = *this;
        t.curr       = size();
        return t;
    }

    // Increment operators:

    Slice_iter& operator++()
    {
        ++curr;
        return *this;
    }

    Slice_iter operator++(int)
    {
        Slice_iter t = *this;
        ++curr;
        return t;
    }

    Slice_iter& operator--()
    {
        --curr;
        return *this;
    }

    Slice_iter operator--(int)
    {
        Slice_iter t = *this;
        --curr;
        return t;
    }

    // Capacity.
    size_type size() const { return desc.size; }

    // Element access:

    reference operator[](difference_type i) { return ref(i); }

    // Pointer like operators:

    reference operator*() { return ref(curr); }
    pointer operator->() { return ptr + curr * desc.stride; }

    // Arithmetic:

    Slice_iter& operator+=(difference_type off)
    {
        curr += off;
        return *this;
    }

    Slice_iter& operator-=(difference_type off)
    {
        curr -= off;
        return *this;
    }

    Slice_iter operator+(difference_type off)
    {
        srs::Slice s(desc.start + off, desc.size, desc.stride);
        return Slice_iter(ptr, s);
    }

    Slice_iter operator-(difference_type off)
    {
        srs::Slice s(desc.start - off, desc.size, desc.stride);
        return Slice_iter(ptr, s);
    }

    difference_type operator-(const Slice_iter& rhs) { return curr - rhs.curr; }

    // Comparison operators:

    // clang-format off
    friend bool operator==<>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator!=<>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator< <>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator> <>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator<=<>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator>=<>(const Slice_iter& a, const Slice_iter& b);
    // clang-format off

private:
    T* ptr;
    srs::Slice desc;
    std::size_t curr;

    reference ref(std::size_t i) const
    {
#ifndef NDEBUG
        Expects(i >= 0 && i < desc.size);
#endif
        return ptr[i * desc.stride];
    }
};

template <class T>
inline bool operator==(const Slice_iter<T>& a, const Slice_iter<T>& b)
{
    return (a.curr == b.curr) && (a.desc.stride == b.desc.stride);
}

template <class T>
inline bool operator!=(const Slice_iter<T>& a, const Slice_iter<T>& b)
{
    return !(a == b);
}

template <class T>
inline bool operator<(const Slice_iter<T>& a, const Slice_iter<T>& b)
{
    return (a.curr < b.curr) && (a.desc.stride == b.desc.stride);
}

template <class T>
inline bool operator>(const Slice_iter<T>& a, const Slice_iter<T>& b)
{
    return b < a;
}

template <class T>
inline bool operator<=(const Slice_iter<T>& a, const Slice_iter<T>& b)
{
    return !(a > b);
}

template <class T>
inline bool operator>=(const Slice_iter<T>& a, const Slice_iter<T>& b)
{
    return !(a < b);
}

//------------------------------------------------------------------------------

// Forward declarations:

template <class T>
class Cslice_iter;

template <class T>
bool operator==(const Cslice_iter<T>&, const Cslice_iter<T>&);

template <class T>
bool operator!=(const Cslice_iter<T>&, const Cslice_iter<T>&);

template <class T>
bool operator<(const Cslice_iter<T>&, const Cslice_iter<T>&);

template <class T>
bool operator>(const Cslice_iter<T>&, const Cslice_iter<T>&);

template <class T>
bool operator<=(const Cslice_iter<T>&, const Cslice_iter<T>&);

template <class T>
bool operator>=(const Cslice_iter<T>&, const Cslice_iter<T>&);

//------------------------------------------------------------------------------

//
// Random access slice const_iterator for use by Array_ref.
//
// Modified version of Stroustrup's Cslice_iter class (TC++PL, p. 670).
//
template <class T>
class Cslice_iter {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::size_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    Cslice_iter(T* p, srs::Slice s) : ptr(p), desc(s), curr(s.start) {}

    Cslice_iter begin() const
    {
        Cslice_iter t = *this;
        return t;
    }

    Cslice_iter end() const
    {
        Cslice_iter t = *this;
        t.curr       = size();
        return t;
    }

    // Increment operators:

    Cslice_iter& operator++()
    {
        ++curr;
        return *this;
    }

    Cslice_iter operator++(int)
    {
        Cslice_iter t = *this;
        ++curr;
        return t;
    }

    Cslice_iter& operator--()
    {
        --curr;
        return *this;
    }

    Cslice_iter operator--(int)
    {
        Cslice_iter t = *this;
        --curr;
        return t;
    }

    // Capacity.
    size_type size() const { return desc.size; }

    // Element access:

    reference operator[](difference_type i) { return ref(i); }

    // Pointer like operators:

    reference operator*() { return ref(curr); }
    pointer operator->() { return ptr + curr * desc.stride; }

    // Arithmetic:

    Cslice_iter& operator+=(difference_type off)
    {
        curr += off;
        return *this;
    }

    Cslice_iter& operator-=(difference_type off)
    {
        curr -= off;
        return *this;
    }

    Cslice_iter operator+(difference_type off)
    {
        srs::Slice s(desc.start + off, desc.size, desc.stride);
        return Cslice_iter(ptr, s);
    }

    Cslice_iter operator-(difference_type off)
    {
        srs::Slice s(desc.start - off, desc.size, desc.stride);
        return Cslice_iter(ptr, s);
    }

    difference_type operator-(const Cslice_iter& rhs) { return curr - rhs.curr; }

    // Comparison operators:

    // clang-format off
    friend bool operator==<>(const Cslice_iter& a, const Cslice_iter& b);
    friend bool operator!=<>(const Cslice_iter& a, const Cslice_iter& b);
    friend bool operator< <>(const Cslice_iter& a, const Cslice_iter& b);
    friend bool operator> <>(const Cslice_iter& a, const Cslice_iter& b);
    friend bool operator<=<>(const Cslice_iter& a, const Cslice_iter& b);
    friend bool operator>=<>(const Cslice_iter& a, const Cslice_iter& b);
    // clang-format off

private:
    T* ptr;
    srs::Slice desc;
    std::size_t curr;

    reference ref(std::size_t i) const
    {
#ifndef NDEBUG
        Expects(i >= 0 && i < desc.size);
#endif
        return ptr[i * desc.stride];
    }
};

template <class T>
inline bool operator==(const Cslice_iter<T>& a, const Cslice_iter<T>& b)
{
    return (a.curr == b.curr) && (a.desc.stride == b.desc.stride);
}

template <class T>
inline bool operator!=(const Cslice_iter<T>& a, const Cslice_iter<T>& b)
{
    return !(a == b);
}

template <class T>
inline bool operator<(const Cslice_iter<T>& a, const Cslice_iter<T>& b)
{
    return (a.curr < b.curr) && (a.desc.stride == b.desc.stride);
}

template <class T>
inline bool operator>(const Cslice_iter<T>& a, const Cslice_iter<T>& b)
{
    return b < a;
}

template <class T>
inline bool operator<=(const Cslice_iter<T>& a, const Cslice_iter<T>& b)
{
    return !(a > b);
}

template <class T>
inline bool operator>=(const Cslice_iter<T>& a, const Cslice_iter<T>& b)
{
    return !(a < b);
}

}  // namespace srs

#endif  // SRS_SLICE_ITER
