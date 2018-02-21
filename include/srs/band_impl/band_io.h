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

#ifndef SRS_BAND_IO_H
#define SRS_BAND_IO_H

#include <srs/band_impl/band_matrix.h>
#include <iomanip>
#include <iostream>


namespace srs {

template <class T>
std::ostream& operator<<(std::ostream& to, const Band_matrix<T>& ab)
{
    using size_type = typename Band_matrix<T>::size_type;

    to << ab.rows() << " x " << ab.cols() << "\n[";
    for (size_type i = 0; i < ab.rows(); ++i) {
        for (size_type j = 0; j < ab.cols(); ++j) {
            to << std::setw(9) << ab(i, j) << " ";
        }
        if (i != ab.rows() - 1) {
            to << "\n ";
        }
    }
    to << "]\n";
    return to;
}

}  // namespace srs

#endif  // SRS_BAND_IO_H
