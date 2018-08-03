////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018 Stig Rune Sellevag. All rights reserved.
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

#ifndef SRS_MATH_QUATERNION_H
#define SRS_MATH_QUATERNION_H

#include <srs/array.h>

namespace srs {

//
// Return rotation matrix for the qiven quaternions.
//
// Notes:
//   Uses the XYZ convention
//
// Algorithm:
//   https://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
//
// Arguments:
//   Vector with quaternions (q0, q1, q2, q3)
//
// Returns:
//   Rotation matrix
//
srs::dmatrix quat2rotm(const srs::dvector& quat);

}  // namespace srs

#endif  // SRS_MATH_QUATERNION_H
