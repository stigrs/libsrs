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

#ifndef SRS_MATH_GEOMETRY_H
#define SRS_MATH_GEOMETRY_H

#include <srs/datum.h>
#include <armadillo>
#include <cmath>


//
// Provides geometrical functions.
//
namespace srs {

// Convert radians to degrees.
inline double radtodeg(double rad) { return rad * 180.0 / datum::pi; }

// Convert degrees to radians.
inline double degtorad(double deg) { return deg * datum::pi / 180.0; }

// Find hypotenuse without overflow or destructive underflow.
double hypot(const double a, const double b);

// Spherical to cartesian.
inline void sph2cart(
    double azi, double zen, double r, double& x, double& y, double& z)
{
    x = r * std::cos(azi) * std::sin(zen);
    y = r * std::sin(azi) * std::sin(zen);
    z = r * std::cos(zen);
}

// Cartesian to spherical.
inline void cart2sph(
    double x, double y, double z, double& azi, double& zen, double& r)
{
    r   = srs::hypot(srs::hypot(x, y), z);
    azi = std::atan2(y, x);
    zen = std::acos(z / r);
}

// Cylindrical to cartesian.
inline void pol2cart(
    double th, double r, double zin, double& x, double& y, double& z)
{
    x = r * std::cos(th);
    y = r * std::sin(th);
    z = zin;
}

// Cartesian to cylindrical.
inline void cart2pol(
    double x, double y, double z, double& th, double& r, double& zout)
{
    th   = std::atan2(y, x);
    r    = srs::hypot(x, y);
    zout = z;
}

// Polar to cartesian.
inline void pol2cart(double th, double r, double& x, double& y)
{
    x = r * std::cos(th);
    y = r * std::sin(th);
}

// Cartesian to polar.
inline void cart2pol(double x, double y, double& th, double& r)
{
    th = std::atan2(y, x);
    r  = srs::hypot(x, y);
}

// Compute distance between two points.
inline double distance(const arma::vec& a, const arma::vec& b)
{
    return arma::norm(b - a);
}

// Compute angle in degrees between three points.
inline double angle(const arma::vec& a, const arma::vec& b, const arma::vec& c)
{
    const arma::vec ab = arma::normalise(a - b);
    const arma::vec bc = arma::normalise(c - b);
    return radtodeg(std::acos(dot(ab, bc)));
}

// Compute dihedral angle in degrees given four points.
double dihedral(const arma::vec& a,
                const arma::vec& b,
                const arma::vec& c,
                const arma::vec& d);

// Compute the pair-wise distances between observations in n-dim. space.
void pdist_matrix(arma::mat& dm, const arma::mat& mat);

// Perform translation.
void translate(arma::mat& xyz, double dx, double dy, double dz);

// Perform rotation given a rotation matrix.
void rotate(arma::mat& xyz, const arma::mat& rotm);

}  // namespace srs

#endif  // SRS_MATH_GEOMETRY_H
