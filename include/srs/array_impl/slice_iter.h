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

//------------------------------------------------------------------------------

//
// Bidirectional slice iterator class for use by Array_ref class.
//
// This is a modified version of Stroustrup's Slice_iter class (TC++PL, p. 670).
//
// TODO: Make it a random access iterator.
//
template <class T>
class Slice_iter {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::size_t size_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    Slice_iter(T* p, const srs::Slice& s) : ptr(p), desc(s), curr(s.start) {}

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

    // Pointer like operators:

    reference operator*() { return ref(curr); }
    pointer operator->() { return ptr + curr * desc.stride; }

    // Comparison operators:

    friend bool operator==<>(const Slice_iter& a, const Slice_iter& b);
    friend bool operator!=<>(const Slice_iter& a, const Slice_iter& b);

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

}  // namespace srs

#endif  // SRS_SLICE_ITER
