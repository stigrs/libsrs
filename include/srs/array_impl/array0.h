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

#ifndef SRS_ARRAY0_H
#define SRS_ARRAY0_H

namespace srs {

//
// The type Array<T, 0> is not really an array. It stores a single scalar
// of type T and can only be converted to a reference to that type.
//
template <class T>
class Array<T, 0> {
public:
    static constexpr std::size_t rank = 0;

    typedef T value_type;
    typedef std::size_t size_type;

    Array() = default;

    // Initialize the pseudo-array.
    Array(const T& x) : elem(x) {}

    // Assignment.
    Array& operator=(const T& value)
    {
        elem = value;
        return *this;
    }

    T& operator()() { return elem; }
    const T& operator()() const { return elem; }

    operator T&() { return elem; }
    operator const T&() { return elem; }

private:
    T elem;
};

}  // namespace srs

#endif  // SRS_ARRAY0_H
