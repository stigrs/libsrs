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

#include <srs/math_impl/geometry.h>
#include <srs/math_impl/linalg.h>
#include <gsl/gsl>


double srs::hypot(const double a, const double b)
{
    double aa = std::abs(a);
    double ab = std::abs(b);

    if (aa > ab) {
        return aa * std::sqrt(1.0 + std::pow(ab / aa, 2.0));
    }
    return ab == 0.0 ? 0.0 : ab * std::sqrt(1.0 + std::pow(aa / ab, 2.0));
}

double srs::dihedral(const srs::dvector& a,
                     const srs::dvector& b,
                     const srs::dvector& c,
                     const srs::dvector& d)
{
    auto ab  = srs::normalize(b - a);
    auto bc  = srs::normalize(c - b);
    auto cd  = srs::normalize(d - c);
    auto n1  = srs::cross(ab, bc);
    auto n2  = srs::cross(bc, cd);
    auto m   = srs::cross(n1, bc);
    double x = srs::dot(n1, n2);
    double y = srs::dot(m, n2);

    double tau = srs::radtodeg(std::atan2(y, x));
    if (std::abs(tau) < 1.0e-8) {  // avoid very small angles close to zero
        tau = 0.0;
    }
    return tau;
}

void srs::pdist_matrix(srs::dmatrix& dm, const srs::dmatrix& mat)
{
    using size_type = srs::dmatrix::size_type;

    dm.resize(mat.rows(), mat.rows(), 0.0);

    for (size_type j = 0; j < dm.cols(); ++j) {
        for (size_type i = j; i < dm.rows(); ++i) {
            if (i != j) {
                auto dij = mat.row(i) - mat.row(j);
                dm(i, j) = srs::norm(dij);
                dm(j, i) = dm(i, j);
            }
        }
    }
}

void srs::translate(srs::dmatrix& xyz, double dx, double dy, double dz)
{
    Expects(xyz.cols() == 3);

    using size_type = srs::dmatrix::size_type;

    for (size_type i = 0; i < xyz.rows(); ++i) {
        xyz(i, 0) += dx;
        xyz(i, 1) += dy;
        xyz(i, 2) += dz;
    }
}

void srs::rotate(srs::dmatrix& xyz, const srs::dmatrix& rotm)
{
    Expects(rotm.rows() == 3 && rotm.cols() == 3);

    using size_type = srs::dmatrix::size_type;

    srs::dvector xold;
    srs::dvector xnew;

    for (size_type i = 0; i < xyz.rows(); ++i) {
        auto xyz_new = rotm * xyz.row(i);
        xyz(i, 0) = xyz_new(0);
        xyz(i, 1) = xyz_new(1);
        xyz(i, 2) = xyz_new(2);
    }
}
