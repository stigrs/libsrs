/// THE//OFTWARE.////////////////////////////////////
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

#ifndef SRS_SLICE_ITER
#define SRS_SLICE_ITER

#include <cstddef>
#include <gsl/gsl>

namespace srs {

//
// Slice descriptor.
//
struct Slice {
    Slice(std::size_t start_, std::size_t size_, std::size_t stride_)
        : start{start_}, size{size_}, stride{stride_}
    {
    }

    Slice(const Slice& s)
    : start{s.start}, size{s.size}, stride{s.stride}
    {
    }

    Slice& operator=(const Slice& s)
    {
        start = s.start;
        size = s.size;
        stride = s.stride;
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

//------------------------------------------------------------------------------

//
// Slice iterator.
//
template <class T>
class Slice_iter {
    Slice_iter(T* p, srs::Slice s) : ptr(p), desc(s), curr(s.start) {}

    Slice_iter& operator=(const Slice_iter& a)
    {
        ptr = a.ptr;
        desc = a.desc;
        curr = a.curr;
    }
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
        curr++;
        return *this;
    }

    Slice_iter operator++(int)
    {
        Slice_iter t = *this;
        curr++;
        return t;
    }

    Slice_iter& operator--()
    {
        curr--;
        return *this;
    }

    Slice_iter operator--(int)
    {
        Slice_iter t = *this;
        curr--;
        return t;
    }

    // Capacity.
    std::size_t size() const { return desc.size; }

    // Element access:

    T& operator[](std::size_t i) { return ref(i); }
    T& operator*() { return ref(curr); }

    // Comparison operators:

    friend bool operator==<>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator!=<>(const Slice_iter& a, const Slice_iter& b);

    T* ptr;

private:
    srs::Slice desc;
    std::size_t curr;

    T& ref(std::size_t i) const
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

//------------------------------------------------------------------------------

// Forward declarations:

template <class T>
class Cslice_iter;

template <class T>
bool operator==(const Cslice_iter<T>&, const Cslice_iter<T>&);

template <class T>
bool operator!=(const Cslice_iter<T>&, const Cslice_iter<T>&);

//------------------------------------------------------------------------------

//
// Slice iterator.
//
template <class T>
class Cslice_iter {
    Cslice_iter(T* p, srs::Slice s) : ptr(p), desc(s), curr(s.start) {}

    Cslice_iter begin() const
    {
        Cslice_iter t = *this;
        return t;
    }

    Cslice_iter end() const
    {
        Cslice_iter t = *this;
        t.curr        = size();
        return t;
    }

    // Increment operators:

    Cslice_iter& operator++()
    {
        curr++;
        return *this;
    }

    Cslice_iter operator++(int)
    {
        Cslice_iter t = *this;
        curr++;
        return t;
    }

    Cslice_iter& operator--()
    {
        curr--;
        return *this;
    }

    Cslice_iter operator--(int)
    {
        Cslice_iter t = *this;
        curr--;
        return t;
    }

    // Capacity.
    std::size_t size() const { return desc.size; }

    // Element access:

    const T& operator[](std::size_t i) const { return ref(i); }
    const T& operator*() const { return ref(curr); }

    // Comparison operators:

    friend bool operator==<>(const Cslice_iter& a, const Cslice_iter& b);
    friend bool operator!=<>(const Cslice_iter& a, const Cslice_iter& b);

    T* ptr;

private:
    srs::Slice desc;
    std::size_t curr;

    T& ref(std::size_t i) const
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

}  // namespace srs

#endif  // SRS_SLICE_ITER
