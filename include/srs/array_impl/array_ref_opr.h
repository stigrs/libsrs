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

#ifndef SRS_ARRAY_REF_OPR_H
#define SRS_ARRAY_REF_OPR_H

#include <algorithm>
#include <gsl/gsl>


namespace srs {

// Aritmetic operators:

template <class T, std::size_t N>
inline Array<T, N> operator+(const Array_ref<T, N>& a, const Array_ref<T, N>& b)
{
    Array<T, N> result(a);
    return result += b;
}

template <class T, std::size_t N>
inline Array<T, N> operator-(const Array_ref<T, N>& a, const Array_ref<T, N>& b)
{
    Array<T, N> result(a);
    return result -= b;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const Array_ref<T, N>& a, const T& scalar)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T, std::size_t N>
inline Array<T, N> operator*(const T& scalar, const Array_ref<T, N>& a)
{
    Array<T, N> result(a);
    return result *= scalar;
}

template <class T>
inline Array<T, 1> operator*(const Array<T, 2>& a, const Array_ref<T, 1>& v)
{
    return mv_mul(a, v);
}

//------------------------------------------------------------------------------

// Mathematical functions:

// Matrix-vector multiplication.
template <class T>
inline Array<T, 1> mv_mul(const Array<T, 2>& a, const Array_ref<T, 1>& v)
{
    Array<T, 1> result;
    mv_mul(a, v, result);
    return result;
}

// Matrix-vector multiplication.
template <class T>
void mv_mul(const Array<T, 2>& a, const Array_ref<T, 1>& v, Array<T, 1>& w)
{
    typedef typename Array<T, 2>::size_type size_type;

    Expects(v.size() == a.cols());

    w.resize(a.rows());
    w = T(0);

    size_type astr = a.rows();

    for (size_type j = 0; j < a.cols(); ++j) {
        T vj = v(j);
        if (vj != T(0)) {
            for (size_type i = 0; i < a.rows(); ++i) {
                // w(i) += a(i,j) * v[j];
                w(i) += a.data()[i + j * astr] * vj;  // help the compiler
            }
        }
    }
}

}  // namespace srs

#endif  // SRS_ARRAY_REF_OPR_H
