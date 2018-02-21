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

#ifndef SRS_BAND_OPR_H
#define SRS_BAND_OPR_H

#include <algorithm>


namespace srs {

// Comparison operators:

template <class T>
inline bool operator==(const Band_matrix<T>& a, const Band_matrix<T>& b)
{
    return std::equal(a.begin(), a.end(), b.begin());
}

template <class T>
inline bool operator!=(const Band_matrix<T>& a, const Band_matrix<T>& b)
{
    return !(a == b);
}

//------------------------------------------------------------------------------

// Scalar addition:

template <class T>
inline Band_matrix<T> operator+(const Band_matrix<T>& a, const T& scalar)
{
    Band_matrix<T> result(a);
    return result += scalar;
}

template <class T>
inline Band_matrix<T> operator+(const T& scalar, const Band_matrix<T>& a)
{
    Band_matrix<T> result(a);
    return result += scalar;
}

//------------------------------------------------------------------------------

// Scalar subtraction:

template <class T>
inline Band_matrix<T> operator-(const Band_matrix<T>& a, const T& scalar)
{
    Band_matrix<T> result(a);
    return result -= scalar;
}

template <class T>
inline Band_matrix<T> operator-(const T& scalar, const Band_matrix<T>& a)
{
    Band_matrix<T> result(a);
    return result -= scalar;
}

//------------------------------------------------------------------------------

// Scalar multiplication:

template <class T>
inline Band_matrix<T> operator*(const Band_matrix<T>& a, const T& scalar)
{
    Band_matrix<T> result(a);
    return result *= scalar;
}

template <class T>
inline Band_matrix<T> operator*(const T& scalar, const Band_matrix<T>& a)
{
    Band_matrix<T> result(a);
    return result *= scalar;
}

//------------------------------------------------------------------------------

// Algorithms:

// Swap matrices.
template <class T>
inline void swap(Band_matrix<T>& a, Band_matrix<T>& b)
{
    a.swap(b);
}

}  // namespace srs

#endif  // SRS_BAND_OPR_H
