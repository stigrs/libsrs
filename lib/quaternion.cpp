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

#include <srs/math_impl/linalg.h>
#include <srs/math_impl/quaternion.h>
#include <limits>

srs::dmatrix srs::quat2rotm(const srs::dvector& quat)
{
    const double tol = 2.0 * std::numeric_limits<double>::epsilon();

    srs::dmatrix rotm = srs::identity(3);

    double w = quat(0);
    double x = quat(1);
    double y = quat(2);
    double z = quat(3);

    double n = w * w + x * x + y * y + z * z;
    if (n > tol) {
        double s  = 2.0 / n;
        double wx = s * w * x;
        double wy = s * w * y;
        double wz = s * w * z;
        double xx = s * x * x;
        double xy = s * x * y;
        double xz = s * x * z;
        double yy = s * y * y;
        double yz = s * y * z;
        double zz = s * z * z;

        double a11 = 1.0 - (yy + zz);
        if (std::abs(a11) < tol) {
            a11 = 0.0;
        }
        double a12 = xy - wz;
        if (std::abs(a12) < tol) {
            a12 = 0.0;
        }
        double a13 = xz + wy;
        if (std::abs(a13) < tol) {
            a13 = 0.0;
        }
        double a21 = xy + wz;
        if (std::abs(a21) < tol) {
            a21 = 0.0;
        }
        double a22 = 1.0 - (xx + zz);
        if (std::abs(a22) < tol) {
            a22 = 0.0;
        }
        double a23 = yz - wx;
        if (std::abs(a23) < tol) {
            a23 = 0.0;
        }
        double a31 = xz - wy;
        if (std::abs(a31) < tol) {
            a31 = 0.0;
        }
        double a32 = yz + wx;
        if (std::abs(a32) < tol) {
            a32 = 0.0;
        }
        double a33 = 1.0 - (xx + yy);
        if (std::abs(a33) < tol) {
            a33 = 0.0;
        }
        rotm = {{a11, a12, a13}, {a21, a22, a23}, {a31, a32, a33}};
    }
    return rotm;
}
