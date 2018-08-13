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

#ifndef SRS_MATH_CORE_H
#define SRS_MATH_CORE_H

#include <srs/array.h>
#include <cmath>
#include <stdexcept>
#include <string>


//
// Provides core mathematical functions.
//
namespace srs {

// Math exceptions.
struct Math_error : std::runtime_error {
    Math_error(std::string s) : std::runtime_error(s) {}
};

//------------------------------------------------------------------------------

// Check if integer is even.
inline bool is_even(int n) { return n % 2 ? false : true; }

// Check if integer is odd.
inline bool is_odd(int n) { return n % 2 ? true : false; }

// Compute Kronecker delta.
inline int krond(int i, int j) { return i == j; }
inline long krond(long i, long j) { return i == j; }
inline std::size_t krond(std::size_t i, std::size_t j) { return i == j; }

// Round double to nearest integer type.
template <typename T>
inline T round(double x)
{
    return static_cast<T>((x > 0.0) ? x + 0.5 : x - 0.5);
}

// Round double to nearest integer.
inline int nint(double x)
{
    return static_cast<int>((x > 0.0) ? x + 0.5 : x - 0.5);
}

// Round double to nearest long integer.
inline int nlong(double x)
{
    return static_cast<long>((x > 0.0) ? x + 0.5 : x - 0.5);
}

// Shift function.
template <typename T>
inline void shift(T& a, T& b, T& c, const T& d)
{
    a = b;
    b = c;
    c = d;
}

// Sign transfer function.
inline double sign(const double x, const double y)
{
    return (y >= 0) ? std::abs(x) : -std::abs(x);
}

/// Raise argument to the power of 2.
template <typename T>
inline T sqr(const T& x)
{
    return x * x;
}

//------------------------------------------------------------------------------

// Floating point comparison:

inline bool approx_equal(double a,
                         double b,
                         double tol,
                         const std::string& method = "absdiff")
{
    bool equal = false;
    if (method == "absdiff") {
        equal = std::abs(a - b) <= tol;
    }
    else if (method == "reldiff") {
        equal = (std::abs(a - b) / std::max(a, b)) <= tol;
    }
    return equal;
}

template <int N>
bool approx_equal(const Array<double, N>& a,
                  const Array<double, N>& b,
                  double tol,
                  const std::string& method = "absdiff")
{
    using size_type = typename Array<double, N>::size_type;

    bool equal = false;
    if (a.size() != b.size()) {
        equal = false;
    }
    for (size_type i = 0; i < a.size(); ++i) {
        equal = approx_equal(a.data()[i], b.data()[i], tol, method);
    }
    return equal;
}

}  // namespace srs

#endif  // SRS_MATH_CORE_H
