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

#ifndef SRS_PACKED_IO_H
#define SRS_PACKED_IO_H

#include <srs/packed_impl/packed_matrix.h>
#include <iomanip>
#include <iostream>


namespace srs {

template <class T>
std::ostream& operator<<(std::ostream& to, const Packed_matrix<T>& ap)
{
    using size_type = typename Packed_matrix<T>::size_type;

    to << ap.rows() << " x " << ap.cols() << "\n[";
    for (size_type i = 0; i < ap.rows(); ++i) {
        for (size_type j = 0; j < ap.cols(); ++j) {
            to << std::setw(9) << ap(i, j) << " ";
        }
        if (i != ap.rows() - 1) {
            to << "\n ";
        }
    }
    to << "]\n";
    return to;
}

}  // namespace srs

#endif  // SRS_PACKED_IO_H
