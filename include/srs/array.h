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

#ifndef SRS_ARRAY_H
#define SRS_ARRAY_H

#include <complex>
#include <cstddef>
#include <stdexcept>
#include <string>


namespace srs {

//
// N-dimensional dense array class.
//
// Features:
// - Create 0D, 1D, 2D, 3D, and 4D arrays.
// - Continuous storage of elements.
// - Column-major storage order (base 0).
// - Value semantics.
// - Range-checked element access unless NDEBUG is defined.
// - Sub-array views (slicing).
//
// Note:
// - The general Array_slice template exists only to allow specializations.
// - Use e.g. Intel MKL for improved numerical performance.
//
template <class T, std::size_t N>
class Array {
private:
    Array();
};

//------------------------------------------------------------------------------

// Error reporting:

struct Array_error : std::runtime_error {
    Array_error(std::string s) : std::runtime_error(s) {}
};

//------------------------------------------------------------------------------

// Convenient typedefs:

typedef Array<int, 1> ivector;
typedef Array<std::size_t, 1> uvector;
typedef Array<double, 1> dvector;
typedef Array<std::complex<double>, 1> zvector;

typedef Array<int, 2> imatrix;
typedef Array<std::size_t, 2> umatrix;
typedef Array<double, 2> dmatrix;
typedef Array<std::complex<double>, 2> zmatrix;

typedef Array<int, 3> icube;
typedef Array<std::size_t, 3> ucube;
typedef Array<double, 3> dcube;
typedef Array<std::complex<double>, 3> zcube;

}  // namespace srs

#include <srs/array_impl/array0.h>
#include <srs/array_impl/array1.h>
#include <srs/array_impl/array2.h>
#include <srs/array_impl/array3.h>
#include <srs/array_impl/array4.h>
#include <srs/array_impl/array_io.h>
#include <srs/array_impl/array_opr.h>

#endif  // SRS_ARRAY_H
