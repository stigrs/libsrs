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

#ifndef SRS_SPARSE_H
#define SRS_SPARSE_H

//
// Provides sparse vector and matrix class.
//
#include <srs/sparse_impl/sparse_io.h>
#include <srs/sparse_impl/sparse_matrix.h>
#include <srs/sparse_impl/sparse_opr.h>
#include <srs/sparse_impl/sparse_vector.h>

namespace srs {

typedef Sparse_vector<int> sparse_ivector;
typedef Sparse_vector<double> sparse_dvector;

typedef Sparse_matrix<int> sparse_imatrix;
typedef Sparse_matrix<double> sparse_dmatrix;

}  // namespace srs

#endif  // SRS_SPARSE_H
