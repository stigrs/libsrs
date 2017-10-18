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

#ifndef SRS_MATH_SIGNAL_H
#define SRS_MATH_SIGNAL_H

#include <srs/array.h>


//
// Provides signal processing methods.
//
namespace srs {

// Vector convolution.
template <class T>
Array<T, 1> conv(const Array<T, 1>& a, const Array<T, 1>& b)
{
    using size_type = Array<T, 1>::size_type;

    const auto na = a.size();
    const auto nb = b.size();
    const auto nc = na + nb - 1;

    Array<T, 1> result(nc);

    for (size_type i = 0; i < nc; ++i) {
        result(i) = T(0);
        auto jmin = (i >= (nb - 1)) ? (i - (nb - 1)) : 0;
        auto jmax = (i < (na - 1)) ? i : (na - 1);
        for (size_type j = jmin; j <= jmax; ++j) {
            result(i) += a(j) * b(i - j);
        }
    }
    return result;
}

}  // namespace srs

#endif  // SRS_MATH_SIGNAL_H
