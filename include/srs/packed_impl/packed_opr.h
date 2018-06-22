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

#ifndef SRS_PACKED_OPR_H
#define SRS_PACKED_OPR_H

#include <algorithm>


namespace srs {

// Comparison operators:

template <class T>
inline bool operator==(const Packed_matrix<T>& a, const Packed_matrix<T>& b)
{
    return std::equal(a.begin(), a.end(), b.begin());
}

template <class T>
inline bool operator!=(const Packed_matrix<T>& a, const Packed_matrix<T>& b)
{
    return !(a == b);
}

template <class T>
inline bool operator<(const Packed_matrix<T>& a, const Packed_matrix<T>& b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template <class T>
inline bool operator>(const Packed_matrix<T>& a, const Packed_matrix<T>& b)
{
    return (b < a);
}

template <class T>
inline bool operator<=(const Packed_matrix<T>& a, const Packed_matrix<T>& b)
{
    return !(a > b);
}

template <class T>
inline bool operator>=(const Packed_matrix<T>& a, const Packed_matrix<T>& b)
{
    return !(a < b);
}

//------------------------------------------------------------------------------

// Matrix addition:

template <class T>
inline Packed_matrix<T> operator+(const Packed_matrix<T>& a,
                                  const Packed_matrix<T>& b)
{
    Packed_matrix<T> result(a);
    return result += b;
}

//------------------------------------------------------------------------------

// Matrix subtraction:

template <class T>
inline Packed_matrix<T> operator-(const Packed_matrix<T>& a,
                                  const Packed_matrix<T>& b)
{
    Packed_matrix<T> result(a);
    return result -= b;
}

//------------------------------------------------------------------------------

// Scalar addition:

template <class T>
inline Packed_matrix<T> operator+(const Packed_matrix<T>& a, const T& scalar)
{
    Packed_matrix<T> result(a);
    return result += scalar;
}

template <class T>
inline Packed_matrix<T> operator+(const T& scalar, const Packed_matrix<T>& a)
{
    Packed_matrix<T> result(a);
    return result += scalar;
}

//------------------------------------------------------------------------------

// Scalar subtraction:

template <class T>
inline Packed_matrix<T> operator-(const Packed_matrix<T>& a, const T& scalar)
{
    Packed_matrix<T> result(a);
    return result -= scalar;
}

template <class T>
inline Packed_matrix<T> operator-(const T& scalar, const Packed_matrix<T>& a)
{
    Packed_matrix<T> result(a);
    return result -= scalar;
}

//------------------------------------------------------------------------------

// Scalar multiplication:

template <class T>
inline Packed_matrix<T> operator*(const Packed_matrix<T>& a, const T& scalar)
{
    Packed_matrix<T> result(a);
    return result *= scalar;
}

template <class T>
inline Packed_matrix<T> operator*(const T& scalar, const Packed_matrix<T>& a)
{
    Packed_matrix<T> result(a);
    return result *= scalar;
}

//------------------------------------------------------------------------------

// Matrix-matrix multiplication:

template <class T>
inline Array<T, 2> operator*(const Packed_matrix<T>& a, const Array<T, 2>& b)
{
    Array<T, 2> result;
    pmm_mul(a, b, result);
    return result;
}

template <class T>
void pmm_mul(const Packed_matrix<T>& a, const Array<T, 2>& b, Array<T, 2>& c)
{
    // Function to form the matrix product
    //     c = a * b
    // where a is an n x n matrix stored in lower triangular compressed form,
    // b is a full n x n matrix stored compressed column-wise. The output
    // matrix c is a full n x n matrix stored compressed column-wise.

    using value_type = typename Array<T, 2>::value_type;
    using size_type  = typename Array<T, 2>::size_type;

    c.resize(a.rows(), b.cols());

	for (size_type j = 0; j < b.cols(); ++j) {
		for (size_type i = 0; i < a.rows(); ++i) {
			c(i, j) = value_type(0);
			for (size_type k = 0; k < a.cols(); ++k) {
				c(i, j) += a(i, k) * b(k, j);
			}
		}
	}
}

//------------------------------------------------------------------------------

// Algorithms:

// Swap matrices.
template <class T>
inline void swap(Packed_matrix<T>& a, Packed_matrix<T>& b)
{
    a.swap(b);
}

}  // namespace srs

#endif  // SRS_PACKED_OPR_H
