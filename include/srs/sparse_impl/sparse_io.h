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

#ifndef SRS_SPARSE_IO_H
#define SRS_SPARSE_IO_H

#include <srs/sparse_impl/sp_matrix.h>
#include <srs/sparse_impl/sp_vector.h>
#include <iostream>


namespace srs {

// Output stream operator for sparse vectors.
template <class T>
std::ostream& operator<<(std::ostream& to, const Sp_vector<T>& vec)
{
    typedef typename Sp_vector<T>::size_type size_type;
    size_type i = 0;
    to << "[number of non-zero elements: " << vec.num_nonzero() << "]\n\n";
    for (const auto& v : vec) {
        to << "(" << vec.loc(i) << ")\t" << v << '\n';
        ++i;
    }
    to << '\n';
    return to;
}

// Output stream operator for sparse matrices.
template <class T>
std::ostream& operator<<(std::ostream& to, const Sp_matrix<T>& mat)
{
    typedef typename Sp_matrix<T>::size_type size_type;
    to << "[matrix size: " << mat.rows() << " x " << mat.cols()
       << "; number of non-zero elements: " << mat.num_nonzero() << "]\n\n";
    for (size_type i = 0; i < mat.rows(); ++i) {
        for (size_type j = 0; j < mat.cols(); ++j) {
            if (mat(i, j) != T(0)) {
                to << "(" << i << ", " << j << ")\t" << mat(i, j) << '\n';
            }
        }
    }
    to << '\n';
    return to;
}

}  // namespace srs

#endif  // SRS_SPARSE_IO_H
