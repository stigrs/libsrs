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

#ifndef SRS_MATH_EULER_H
#define SRS_MATH_EULER_H

#include <srs/array.h>

namespace srs {

//
// Return rotation matrix around x, y, and z axes.
//
// Convention:
//   Rm = Rm(Z(phi), Y(psi), X(theta)) = ZYX
//   Axis rotation sequence: 3, 2, 1
//
// Algorithm:
//   http://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf
//
// Arguments:
//   z: rotation angle in degrees around z-axis (yaw/phi)
//   y: rotation angle in degrees around y-axis (roll/psi)
//   x: rotation angle in radians around x-axis (pitch/theta)
//
// Returns:
//   3x3 rotation matrix giving same rotation as for given angles
//
srs::dmatrix eul2rotm(double x = 0.0, double y = 0.0, double z = 0.0);

//
// Return Euler angles from rotation matrix.
//
// Convention:
//   Rm = Rm(Z(phi), Y(psi), X(theta)) = ZYX
//   Axis rotation scheme: 3, 2, 1
//
// Algorithm:
//   http://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf
//
// Arguments:
//   rotation matrix
//
// Returns:
//   vector with z, y, x
//   z: rotation angle in degrees around z-axis (yaw/phi)
//   y: rotation angle in degrees around y-axis (roll/psi)
//   x: rotation angle in degrees around x-axis (pitch/theta)
//
srs::dvector rotm2eul(const srs::dmatrix& rotm);

}  // namespace srs

#endif  // SRS_MATH_EULER_H
