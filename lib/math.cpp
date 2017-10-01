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

#include <mkl.h>
#include <srs/math.h>
#include <boost/math/special_functions/legendre.hpp>
#include <gsl/gsl>
#include <vector>


double srs::hypot(const double a, const double b)
{
    double aa = std::abs(a);
    double ab = std::abs(b);

    if (aa > ab) {
        return aa * std::sqrt(1.0 + std::pow(ab / aa, 2.0));
    }
    else {
        return ab == 0.0 ? 0.0 : ab * std::sqrt(1.0 + std::pow(aa / ab, 2.0));
    }
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

    double tau = radtodeg(std::atan2(y, x));
    if (std::abs(tau) < 1.0e-8) {  // avoid very small angles close to zero
        tau = 0.0;
    }
    return tau;
}

void srs::pdist_matrix(srs::dmatrix& dm, const srs::dmatrix& mat)
{
    dm.resize(mat.rows(), mat.rows(), 0.0);

    for (std::size_t j = 0; j < dm.cols(); ++j) {
        for (std::size_t i = j; i < dm.rows(); ++i) {
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
    for (std::size_t i = 0; i < xyz.rows(); ++i) {
        xyz(i, 0) += dx;
        xyz(i, 1) += dy;
        xyz(i, 2) += dz;
    }
}

void srs::rotate(srs::dmatrix& xyz, const srs::dmatrix& rotm)
{
    Expects(rotm.rows() == 3 && rotm.cols() == 3);

    srs::dvector xold;
    srs::dvector xnew;

    for (std::size_t i = 0; i < xyz.rows(); ++i) {
        auto xyz_new = rotm * xyz.row(i);
        xyz(i, 0)    = xyz_new(0);
        xyz(i, 1)    = xyz_new(1);
        xyz(i, 2)    = xyz_new(2);
    }
}

//------------------------------------------------------------------------------

double srs::trapezoidal(double xlo, double xup, const srs::dvector& y)
{
    const double step = std::abs(xup - xlo) / static_cast<double>(y.size());

    double ans = 0.0;
    ans += 0.5 * y[0];
    ans += 0.5 * y[y.size()];

    for (std::size_t i = 1; i < y.size() - 1; ++i) {
        ans += y[i];
    }
    return ans *= step;
}

double srs::simpsons(double xlo, double xup, const srs::dvector& y)
{
    const double step = std::abs(xup - xlo) / static_cast<double>(y.size());
    double ans;
    double f;

    ans = 0.0;
    for (std::size_t i = 0; i < y.size(); ++i) {
        if ((i == 0) || (i == y.size() - 1)) {
            f = y[i];
        }
        else if ((i % 2) == 1) {
            f = 4.0 * y[i];
        }
        else {
            f = 2.0 * y[i];
        }
        ans += f;
    }
    return ans *= step / 3.0;
}

void srs::gaussleg(int n, srs::dvector& x, srs::dvector& w, double a, double b)
{
    // Given the lower and upper limits of integration a and b, this
    // function returns arrays x(n) and w(n) of length n, containing the
    // abscissas and weights of the Gauss-Legendre n-point quadrature formula.

    Expects(n >= 2);
    Expects(srs::is_even(n));

    x.resize(n);
    w.resize(n);

    std::vector<double> xp = boost::math::legendre_p_zeros<double>(n);

    int nhalf = n / 2;
    for (int i = nhalf; i < n; ++i) {  // find x and w from 0 to +1
        int im    = i - nhalf;
        double pp = boost::math::legendre_p_prime<double>(n, xp[im]);
        x(i)      = xp[im];
        w(i)      = 2.0 / ((1.0 - xp[im] * xp[im]) * pp * pp);
    }
    for (int i = 0; i < nhalf; ++i) {  // add symmetric counterpart
        x(i) = -x(n - i - 1);
        w(i) = w(n - i - 1);
    }
    x = 0.5 * (b - a) * x;
    x += 0.5 * (a + b);  // scale to the desired interval
    w = 0.5 * (b - a) * w;
}

//------------------------------------------------------------------------------

srs::dmatrix srs::hilbert(std::size_t n)
{
    Expects(n > 0);
    srs::dmatrix result(n, n);
    for (std::size_t j = 0; j < n; ++j) {
        for (std::size_t i = 0; i < n; ++i) {
            result(i, j) = 1.0 / static_cast<double>(i + j + 1.0);
        }
    }
    return result;
}

//------------------------------------------------------------------------------

void srs::dgemm(const std::string& transa,
                const std::string& transb,
                const double alpha,
                const srs::dmatrix& a,
                const srs::dmatrix& b,
                const double beta,
                srs::dmatrix& c)
{
    Expects(a.rows() == b.cols());

    const MKL_INT m = a.rows();
    const MKL_INT n = b.cols();
    const MKL_INT k = a.cols();

    MKL_INT lda = k > 1 ? k : 1;
    if ((transa == "N") || (transa == "n")) {
        lda = m > 1 ? m : 1;
    }

    MKL_INT ldb = n > 1 ? n : 1;
    if ((transb == "N") || (transb == "n")) {
        ldb = k > 1 ? k : 1;
    }

    MKL_INT ldc = m > 1 ? m : 1;
    if (c.empty()) {
        c.resize(ldc, n);
    }

    CBLAS_TRANSPOSE cblas_transa;
    if ((transa == "T") || (transa == "t")) {
        cblas_transa = CblasTrans;
    }
    else {
        cblas_transa = CblasNoTrans;
    }

    CBLAS_TRANSPOSE cblas_transb;
    if ((transb == "T") || (transb == "t")) {
        cblas_transb = CblasTrans;
    }
    else {
        cblas_transb = CblasNoTrans;
    }

    cblas_dgemm(CblasColMajor,
                cblas_transa,
                cblas_transb,
                m,
                n,
                k,
                alpha,
                a.data(),
                lda,
                b.data(),
                ldb,
                beta,
                c.data(),
                ldc);
}

//------------------------------------------------------------------------------

double srs::det(const srs::dmatrix& a)
{
    Expects(a.rows() == a.cols());

    double ddet;
    long n = a.rows();

    if (n == 1) {
        ddet = a(0, 0);
    }
    else if (n == 2) {
        ddet = a(0, 0) * a(1, 1) - a(1, 0) * a(0, 1);
    }
    else if (n == 3) {
        ddet = a(0, 0) * (a(1, 1) * a(2, 2) - a(1, 2) * a(2, 1))
               - a(0, 1) * (a(1, 0) * a(2, 2) - a(1, 2) * a(2, 0))
               + a(0, 2) * (a(1, 0) * a(2, 1) - a(1, 1) * a(2, 0));
    }
    else {  // use LU decomposition
        srs::dmatrix tmp = a;
        srs::ivector ipiv(n);

        LAPACKE_dgetrf(LAPACK_COL_MAJOR, n, n, tmp.data(), n, ipiv.data());

        long permut = 0;
        for (long i = 1; i <= n; ++i) {
            if (i != ipiv(i - 1)) {  // Fortran uses base 1
                permut++;
            }
        }
        ddet   = 1.0;
        auto d = tmp.diag();
        for (std::size_t i = 0; i < d.size(); ++i) {
            ddet *= d(i);
        }
        ddet *= std::pow(-1.0, static_cast<double>(permut));
    }
    return ddet;
}

void srs::jacobi(srs::dmatrix& a, srs::dvector& wr)
{
    // Algorithm:
    // ----------
    // Kopp, J. Efficient numerical diagonalization of hermitian 3x3 matrices.
    // Int. J. Mod. Phys. C, 2008, vol. 19, pp. 523-548
    // (arXiv:physics/0610206).
    //
    // This function is intended for small matrices where high accuracy
    // and robustness is needed. For larger matrices the function eig()
    // is recommended as this function uses LAPACK and is consequently
    // significantly faster.
    //
    // Note: No check of symmetry is done.

    std::size_t n = a.cols();

    Expects(n == a.rows());

    // Initialize vr to the identity matrix:

    srs::dmatrix vr = srs::identity(n);

    // Initialize wr to diag(a):

    if (wr.size() != n) {
        wr.resize(n);
    }
    wr = a.diag();

    // Calculate the square of the trace of matrix a:

    double sd = 0.0;
    for (auto v : wr) {
        sd += std::abs(v);
    }
    sd *= sd;

    // Main iteration loop:

    const int max_iter = 100;
    int iter;

    for (iter = 0; iter < max_iter; ++iter) {
        // Test for convergence:
        double so = 0.0;

        for (std::size_t p = 0; p < n; ++p) {
            for (std::size_t q = p + 1; q < n; ++q) {
                so += std::abs(a(p, q));
            }
        }
        if (so == 0.0) {
            break;
        }

        double thresh;

        if (iter < 4) {
            thresh = 0.2 * so / static_cast<double>(n * n);
        }
        else {
            thresh = 0.0;
        }

        // Do sweeps:

        for (std::size_t p = 0; p < n; ++p) {
            for (std::size_t q = p + 1; q < n; ++q) {
                double theta;
                double g;
                double h;
                double t;
                double c;
                double s;
                double z;

                g = 100.0 * std::abs(a(p, q));

                if ((iter > 4) && (std::abs(wr(p)) + g == std::abs(wr(p)))
                    && (std::abs(wr(q)) + g == std::abs(wr(q)))) {
                    a(p, q) = 0.0;
                }
                else if (std::abs(a(p, q)) > thresh) {
                    // Calculate Jacobi transformation:
                    h = wr(q) - wr(p);
                    if (std::abs(h) + g == std::abs(h)) {
                        t = a(p, q) / h;
                    }
                    else {
                        theta = 0.5 * h / a(p, q);
                        if (theta < 0.0) {
                            t = -1.0 / (std::sqrt(1.0 + theta * theta) - theta);
                        }
                        else {
                            t = 1.0 / (std::sqrt(1.0 + theta * theta) + theta);
                        }
                    }
                    c = 1.0 / std::sqrt(1.0 + t * t);
                    s = t * c;
                    z = t * a(p, q);

                    // Apply Jacobi transformation:

                    a(p, q) = 0.0;
                    wr[p] -= z;
                    wr[q] += z;

                    for (std::size_t r = 0; r < p; ++r) {
                        t       = a(r, p);
                        a(r, p) = c * t - s * a(r, q);
                        a(r, q) = s * t + c * a(r, q);
                    }
                    for (std::size_t r = p + 1; r < q; ++r) {
                        t       = a(p, r);
                        a(p, r) = c * t - s * a(r, q);
                        a(r, q) = s * t + c * a(r, q);
                    }
                    for (std::size_t r = q + 1; r < n; ++r) {
                        t       = a(p, r);
                        a(p, r) = c * t - s * a(q, r);
                        a(q, r) = s * t + c * a(q, r);
                    }

                    // Update eigenvectors:

                    for (std::size_t r = 0; r < n; ++r) {
                        t        = vr(r, p);
                        vr(r, p) = c * t - s * vr(r, q);
                        vr(r, q) = s * t + c * vr(r, q);
                    }
                }
            }
        }
    }
    if (iter >= max_iter) {
        throw Math_error("srs::jacobi(): too many iterations");
    }

    // Sort eigenvalues in descending order:

    for (std::size_t ii = 1; ii < n; ++ii) {
        std::size_t i = ii - 1;
        std::size_t k = i;
        double p      = wr[i];
        for (std::size_t j = ii; j < n; ++j) {
            if (wr(j) >= p) {
                continue;
            }
            k = j;
            p = wr(j);
        }
        if (k == i) {
            continue;
        }
        wr(k) = wr(i);
        wr(i) = p;
        for (std::size_t j = 0; j < n; ++j) {
            p        = vr(j, i);
            vr(j, i) = vr(j, k);
            vr(j, k) = p;
        }
    }
    a = vr;
}

//------------------------------------------------------------------------------

double srs::harmmean(const srs::dvector& x)
{
    double sumi = 0.0;
    for (std::size_t i = 0; i < x.size(); ++i) {
        Expects(x(i) != 0.0);
        sumi += 1.0 / x(i);
    }
    Ensures(sumi != 0.0);
    return x.size() / sumi;
}

double srs::median(srs::dvector& x)
{
    auto first = x.begin();
    auto last  = x.end();
    auto mid   = first + (last - first) / 2;

    std::nth_element(first, mid, last);
    double med = *mid;

    if ((x.size() % 2) == 0) {  // size is even
        mid = first + (last - first) / 2 - 1;
        std::nth_element(first, mid, last);
        med = (med + *mid) / 2.0;
    }
    return med;
}

double srs::var(const srs::dvector& x)
{
    double n     = static_cast<double>(x.size());
    double xmean = srs::mean(x);
    double sum2  = 0.0;
    double sum3  = 0.0;

    for (std::size_t i = 0; i < x.size(); ++i) {
        sum2 += std::pow(x(i) - xmean, 2.0);
        sum3 += x(i) - xmean;
    }
    return (sum2 - sum3 * sum3 / n) / (n - 1.0);
}

double srs::mad(const srs::dvector& x)
{
    double xmean  = mean(x);
    double sumdev = 0.0;

    for (std::size_t i = 0; i < x.size(); ++i) {
        sumdev = std::abs(x(i) - xmean);
    }
    return sumdev / static_cast<double>(x.size());
}

double srs::rms(const srs::dvector& x)
{
    double sum2 = 0.0;

    for (std::size_t i = 0; i < x.size(); ++i) {
        sum2 += x(i) * x(i);
    }
    return std::sqrt(sum2 / static_cast<double>(x.size()));
}

double srs::cov(const srs::dvector& x, const srs::dvector& y)
{
    Expects(x.size() == y.size() && x.size() > 0);

    double n     = x.size();
    double xmean = srs::mean(x);
    double ymean = srs::mean(y);
    double cov   = 0.0;

    for (std::size_t i = 0; i < x.size(); ++i) {
        double a = x(i) - xmean;
        double b = y(i) - ymean;
        cov += a * b / n;
    }
    return cov;
}
