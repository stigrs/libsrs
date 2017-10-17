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
#include <srs/sparse_impl/sp_matrix.h>
#include <srs/sparse_impl/sp_vector.h>
#include <srs/sparse_impl/sparse_io.h>
#include <srs/sparse_impl/sparse_opr.h>

namespace srs {

typedef Sp_vector<int> sp_ivector;
typedef Sp_vector<double> sp_dvector;

typedef Sp_matrix<int> sp_imatrix;
typedef Sp_matrix<double> sp_dmatrix;

}  // namespace srs

#endif  // SRS_SPARSE_H
