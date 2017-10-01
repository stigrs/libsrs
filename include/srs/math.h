//////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////

#ifndef SRS_MATH_H
#define SRS_MATH_H

#include <srs/array.h>
#include <srs/datum.h>
#include <algorithm>
#include <cmath>
#include <gsl/gsl>
#include <numeric>
#include <stdexcept>
#include <string>


namespace srs {

// Math exceptions:

struct Math_error : std::runtime_error {
    Math_error(std::string s) : std::runtime_error(s) {}
};

//------------------------------------------------------------------------------

// Mathematical functions:

// Check if integer is even.
inline bool is_even(int n) { return n % 2 ? false : true; }

// Check if integer is odd.
inline bool is_odd(int n) { return n % 2 ? true : false; }

// Compute Kronecker delta.
inline int krond(int i, int j) { return i == j; }
inline long krond(long i, long j) { return i == j; }
inline std::size_t krond(std::size_t i, std::size_t j) { return i == j; }

// Round double to nearest integer.
inline int nint(double x)
{
    return static_cast<int>((x > 0.0) ? x + 0.5 : x - 0.5);
}

// Round double to nearest long integer.
inline int nlong(double x)
{
    return static_cast<long>((x > 0.0) ? x + 0.5 : x - 0.5);
}

// Convert radians to degrees.
inline double radtodeg(double rad) { return rad * 180.0 / datum::pi; }

// Convert degrees to radians.
inline double degtorad(double deg) { return deg * datum::pi / 180.0; }

// Shift function.
template <typename T>
inline void shift(T& a, T& b, T& c, const T& d)
{
    a = b;
    b = c;
    c = d;
}

// Sign transfer function.
inline double sign(const double x, const double y)
{
    return (y >= 0) ? std::abs(x) : -std::abs(x);
}

/// Raise argument to the power of 2.
template <typename T>
inline T sqr(const T& x)
{
    return x * x;
}

//------------------------------------------------------------------------------

// Geometry:

// Find hypotenuse without overflow or destructive underflow.
double hypot(const double a, const double b);

// Compute distance between two points.
inline double distance(const srs::dvector& a, const srs::dvector& b)
{
    return srs::norm(b - a);
}

// Compute angle in degrees between three points.
inline double angle(const srs::dvector& a,
                    const srs::dvector& b,
                    const srs::dvector& c)
{
    const auto& ab = srs::normalize(a - b);
    const auto& bc = srs::normalize(c - b);
    return radtodeg(std::acos(srs::dot(ab, bc)));
}

// Compute dihedral angle in degrees given four points.
double dihedral(const srs::dvector& a,
                const srs::dvector& b,
                const srs::dvector& c,
                const srs::dvector& d);

// Compute the pair-wise distances between observations in n-dim. space.
void pdist_matrix(srs::dmatrix& dm, const srs::dmatrix& mat);

// Perform translation.
void translate(srs::dmatrix& xyz, double dx, double dy, double dz);

// Perform rotation given a rotation matrix.
void rotate(srs::dmatrix& xyz, const srs::dmatrix& rotm);

//------------------------------------------------------------------------------

// Coordinate transformations:

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

//------------------------------------------------------------------------------

// Integration:

// Integrate array of function values using the Trapezoidal rule.
double trapezoidal(double xlo, double xup, const srs::dvector& y);

// Integrate array of function values using Simpson's rule.
double simpsons(double xlo, double xup, const srs::dvector& y);

// Compute abscissas and weights of Gauss-Legendre n-point quadrature formula.
void gaussleg(
    int n, srs::dvector& x, srs::dvector& w, double a = -1.0, double b = 1.0);

//------------------------------------------------------------------------------

// Create special vectors and matrices:

// Create identity matrix.
inline srs::dmatrix identity(std::size_t n)
{
    Expects(n > 0);
    srs::dmatrix result(n, n, 0.0);
    result.diag() = 1.0;
    return result;
}

// Create Hilbert matrix.
srs::dmatrix hilbert(std::size_t n);

//------------------------------------------------------------------------------

// Intel MKL wrappers:

/// Matrix-matrix multiplication.
void dgemm(const std::string& transa,
           const std::string& transb,
           const double alpha,
           const srs::dmatrix& a,
           const srs::dmatrix& b,
           const double beta,
           srs::dmatrix& c);

//------------------------------------------------------------------------------

// Matrix-vector functions:

// Matrix-matrix multiplication.
inline void matmul(const srs::dmatrix& a,
                   const srs::dmatrix& b,
                   srs::dmatrix& result)
{
    dgemm("N", "N", 1.0, a, b, 0.0, result);
}

/// Matrix-matrix multiplication.
inline srs::dmatrix matmul(const srs::dmatrix& a, const srs::dmatrix& b)
{
    srs::dmatrix result;
    dgemm("N", "N", 1.0, a, b, 0.0, result);
    return result;
}

//------------------------------------------------------------------------------

// Linear algebra:

// Determinant of a matrix.
double det(const srs::dmatrix& a);

// Compute eigenvalues and eigenvectors of a real symmetric matrix.
void jacobi(srs::dmatrix& a, srs::dvector& wr);

//------------------------------------------------------------------------------

// Statistical functions:

/// Arithmetic mean.
inline double mean(const srs::dvector& x)
{
    return srs::sum(x) / static_cast<double>(x.size());
}

/// Geometric mean.
inline double geomean(const srs::dvector& x)
{
    return std::pow(srs::prod(x), static_cast<double>(1.0 / x.size()));
}

/// Harmonic mean.
double harmmean(const srs::dvector& x);

/// Median.
double median(srs::dvector& x);

/// Variance.
double var(const srs::dvector& x);

/// Standard deviation.
inline double stddev(const srs::dvector& x) { return std::sqrt(var(x)); }

/// Covariance.
double cov(const srs::dvector& x, const srs::dvector& y);

/// Mean absolute deviation.
double mad(const srs::dvector& x);

/// Root-mean-square deviation.
double rms(const srs::dvector& x);

//------------------------------------------------------------------------------

// Floating point comparison:

inline bool approx_equal(double a,
                         double b,
                         double tol,
                         const std::string& method = "absdiff")
{
    bool equal = false;
    if (method == "absdiff") {
        equal = std::abs(a - b) <= tol;
    }
    else if (method == "reldiff") {
        equal = (std::abs(a - b) / std::max(a, b)) <= tol;
    }
    return equal;
}

template <std::size_t N>
bool approx_equal(const srs::Array<double, N>& a,
                  const srs::Array<double, N>& b,
                  double tol,
                  const std::string& method = "absdiff")
{
    bool equal = false;
    if (a.size() != b.size()) {
        equal = false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        equal = approx_equal(a.data()[i], b.data()[i], tol, method);
    }
    return equal;
}

}  // namespace srs

#endif  // SRS_MATH_H
