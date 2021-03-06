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

#include <mkl.h>
#include <srs/math_impl/core.h>
#include <srs/math_impl/linalg.h>
#include <cmath>
#include <gsl/gsl>
#include <iostream>
#include <random>


srs::dmatrix srs::hilbert(int n)
{
    Expects(n > 0);
    srs::dmatrix result(n, n);
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            result(i, j) = 1.0 / static_cast<double>(i + j + 1.0);
        }
    }
    return result;
}

srs::ivector srs::randi(int n, int a, int b)
{
    srs::ivector result(n);

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<> rdist(a, b);

    for (int i = 0; i < n; ++i) {
        result(i) = rdist(mt);
    }
    return result;
}

srs::dvector srs::randu(int n)
{
    srs::dvector result(n);

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution<> rdist;

    for (int i = 0; i < n; ++i) {
        result(i) = rdist(mt);
    }
    return result;
}

srs::imatrix srs::randi(int m, int n, int a, int b)
{
    srs::imatrix result(m, n);

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<> rdist(a, b);

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            result(i, j) = rdist(mt);
        }
    }
    return result;
}

srs::dmatrix srs::randu(int m, int n)
{
    srs::dmatrix result(m, n);

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution<> rdist;

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            result(i, j) = rdist(mt);
        }
    }
    return result;
}

//------------------------------------------------------------------------------

double srs::det(const srs::dmatrix& a)
{
    Expects(a.rows() == a.cols());

    double ddet;
    srs::size_t n = a.rows();

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
        srs::ivector ipiv;
        lu(tmp, ipiv);

        int permut = 0;
        for (srs::size_t i = 1; i <= n; ++i) {
            if (i != ipiv(i - 1)) {  // Fortran uses base 1
                permut++;
            }
        }
        ddet   = 1.0;
        auto d = tmp.diag();
        for (srs::size_t i = 0; i < d.size(); ++i) {
            ddet *= d(i);
        }
        ddet *= std::pow(-1.0, static_cast<double>(permut));
    }
    return ddet;
}

void srs::inv(srs::dmatrix& a)
{
    Expects(a.rows() == a.cols());

    if (det(a) == 0.0) {
        throw Math_error("srs::inv(): matrix is not invertible");
    }
    MKL_INT n = a.rows();

    srs::ivector ipiv(n);
    lu(a, ipiv);  // perform LU factorization

    MKL_INT info
        = LAPACKE_dgetri(LAPACK_COL_MAJOR, n, a.data(), n, ipiv.data());
    if (info != 0) {
        throw Math_error("dgetri: matrix inversion failed");
    }
}

//------------------------------------------------------------------------------

void srs::lu(srs::dmatrix& a, srs::ivector& ipiv)
{
    MKL_INT m = a.rows();
    MKL_INT n = a.cols();
    ipiv.resize(std::min(m, n));

    MKL_INT info
        = LAPACKE_dgetrf(LAPACK_COL_MAJOR, m, n, a.data(), m, ipiv.data());
    if (info < 0) {
        throw Math_error("dgetrf: illegal input parameter");
    }
    if (info > 0) {
        throw Math_error("dgetrf: U matrix is singular");
    }
}

//------------------------------------------------------------------------------

void srs::eigs(srs::dmatrix& a, srs::dvector& wr)
{
    Expects(a.rows() == a.cols());

    MKL_INT n  = a.rows();
    MKL_INT il = 1;
    MKL_INT iu = n;

    wr.resize(n);
    srs::dmatrix z(n, n);
    srs::ivector isuppz(2 * n);

    double abstol = -1.0;  // use default value
    double vl     = 0.0;
    double vu     = 0.0;

    // clang-format off
    MKL_INT info = LAPACKE_dsyevr(
        LAPACK_COL_MAJOR, 'V', 'A', 'U', n, a.data(), n, vl, vu, il, iu, abstol, 
        &n, wr.data(), z.data(), n, isuppz.data());
    // clang-format on
    if (info != 0) {
        throw Math_error("dsyevr failed");
    }
    a = z;
}

void srs::eigs(srs::band_dmatrix& ab, srs::dmatrix& v, srs::dvector& w)
{
    Expects(ab.rows() == ab.cols());
    Expects(ab.upper() == ab.lower());

    v.resize(ab.rows(), ab.cols());
    w.resize(ab.cols());

    MKL_INT n    = ab.cols();
    MKL_INT kd   = ab.upper();
    MKL_INT ldab = ab.leading_dim();
    MKL_INT ldz  = ab.cols();
    MKL_INT info = 0;

    // clang-format off
    info = LAPACKE_dsbev(
        LAPACK_COL_MAJOR, 'V', 'U', n, kd, ab.data(), ldab, 
        w.data(), v.data(), ldz);
    // clang-format on
    if (info != 0) {
        throw Math_error("dsbev failed");
    }
}

void srs::eigs(srs::packed_dmatrix& ap, srs::dmatrix& v, srs::dvector& w)
{
    Expects(ap.size() >= w.size() * (w.size() + 1) / 2);

    MKL_INT n    = w.size();
    MKL_INT ldz  = n;
    MKL_INT info = 0;

    v.resize(n, n);

    info = LAPACKE_dspevd(
        LAPACK_COL_MAJOR, 'V', 'U', n, ap.data(), w.data(), v.data(), ldz);

    if (info != 0) {
        throw Math_error("dspevd failed");
    }
}

void srs::eig(srs::dmatrix& a, srs::zmatrix& v, srs::zvector& w)
{
    Expects(a.rows() == a.cols());

    MKL_INT n     = a.cols();
    MKL_INT lwork = 4 * n;
    MKL_INT info  = 0;

    if (w.size() != a.cols()) {
        w.resize(a.cols());
    }
    srs::dvector wr(n);
    srs::dvector wi(n);
    srs::dmatrix vr(n, n);
    srs::dmatrix vl(n, n);
    srs::dvector work(lwork);

    // clang-format off
    dgeev(
        "N", "V", &n, a.data(), &n, wr.data(), wi.data(), vl.data(), &n, 
        vr.data(), &n, work.data(), &lwork, &info);
    // clang-format on
    if (info != 0) {
        throw Math_error("dgeev failed");
    }
    for (int i = 0; i < vr.rows(); ++i) {  // can this be done faster?
        std::complex<double> wii(wr(i), wi(i));
        w(i) = wii;
        for (int j = 0; j < vr.cols(); j += 2) {
            std::complex<double> v1 = {vr(i, j), 0.0};
            std::complex<double> v2 = {vr(i, j + 1), 0.0};
            if (wi(j) != 0.0) {
                v1 = {vr(i, j), vr(i, j + 1)};
                v2 = {vr(i, j), -vr(i, j + 1)};
            }
            v(i, j)     = v1;
            v(i, j + 1) = v2;
        }
    }
}

void srs::eig(double emin,
              double emax,
              const srs::band_dmatrix& ab,
              dmatrix& v,
              dvector& w)
{
    // Initialize FEAST:

    MKL_INT fpm[128];
    feastinit((MKL_INT*)fpm);
#ifndef NDEBUG
    fpm[0] = 1;  // print runtime status
#endif

    // Solve eigenvalue problem:

    MKL_INT n    = ab.cols();   // size of the problem
    MKL_INT kla  = ab.upper();  // number of sub- or super-diagonals within band
    MKL_INT lda  = ab.leading_dim();  // leading dimension of A
    MKL_INT m0   = n;                 // initial guess for subspace dimension
    MKL_INT loop = 0;                 // number of refinement loops
    MKL_INT m    = m0;                // total number of eigenvalues found
    MKL_INT info = 0;                 // error code

    double epsout = 0.0;   // relative error on the trace (not returned)
    srs::dvector res(m0);  // residual vector (not returned)

    v.resize(n, m0);
    w.resize(m0);

    // clang-format off
    dfeast_sbev(
        "F", &n, &kla, ab.data(), &lda, (MKL_INT*)fpm, &epsout, &loop, 
        &emin, &emax, &m0, w.data(), v.data(), &m, res.data(), &info);
    // clang-format on
    if (info != 0) {
        throw Math_error("dfeast_sbev failed");
    }

    // Return the m first eigenvalues and eigenvectors:

    w = w.head(m);
    v = v.slice(0, n - 1, 0, m - 1);
}

void srs::eig(double emin,
              double emax,
              const srs::sparse_dmatrix& a,
              srs::dmatrix& v,
              srs::dvector& w)
{
    // Initialize FEAST:

    MKL_INT fpm[128];
    feastinit((MKL_INT*)fpm);
#ifndef NDEBUG
    fpm[0] = 1;  // print runtime status
#endif

    // Solve eigenvalue problem:

    MKL_INT n    = a.cols();  // size of the problem
    MKL_INT m0   = n;         // initial guess for subspace dimension
    MKL_INT loop = 0;         // number of refinement loops
    MKL_INT m    = m0;        // total number of eigenvalues found
    MKL_INT info = 0;         // error code

    auto ia = a.row_index_one_based();  // FEAST only support one-based indexing
    auto ja = a.columns_one_based();

    double epsout = 0.0;   // relative error on the trace (not returned)
    srs::dvector res(m0);  // residual vector (not returned)

    v.resize(n, m0);
    w.resize(m0);

    // clang-format off
    dfeast_scsrev(
        "F", &n, a.data(), ia.data(), ja.data(), (MKL_INT*)fpm, &epsout, &loop, 
        &emin, &emax, &m0, w.data(), v.data(), &m, res.data(), &info);
    // clang-format on
    if (info != 0) {
        throw Math_error("dfeast_scsrev failed");
    }

    // Return the m first eigenvalues and eigenvectors:

    w = w.head(m);
    v = v.slice(0, n - 1, 0, m - 1);
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

    int n = a.cols();

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

        for (int p = 0; p < n; ++p) {
            for (int q = p + 1; q < n; ++q) {
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

        for (int p = 0; p < n; ++p) {
            for (int q = p + 1; q < n; ++q) {
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
                    wr(p) -= z;
                    wr(q) += z;

                    for (int r = 0; r < p; ++r) {
                        t       = a(r, p);
                        a(r, p) = c * t - s * a(r, q);
                        a(r, q) = s * t + c * a(r, q);
                    }
                    for (int r = p + 1; r < q; ++r) {
                        t       = a(p, r);
                        a(p, r) = c * t - s * a(r, q);
                        a(r, q) = s * t + c * a(r, q);
                    }
                    for (int r = q + 1; r < n; ++r) {
                        t       = a(p, r);
                        a(p, r) = c * t - s * a(q, r);
                        a(q, r) = s * t + c * a(q, r);
                    }

                    // Update eigenvectors:

                    for (int r = 0; r < n; ++r) {
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

    for (int ii = 1; ii < n; ++ii) {
        int i    = ii - 1;
        int k    = i;
        double p = wr(i);
        for (int j = ii; j < n; ++j) {
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
        for (int j = 0; j < n; ++j) {
            p        = vr(j, i);
            vr(j, i) = vr(j, k);
            vr(j, k) = p;
        }
    }
    a = vr;
}

//------------------------------------------------------------------------------

void srs::linsolve(srs::dmatrix& a, srs::dmatrix& b)
{
    Expects(a.rows() == a.cols());
    Expects(b.rows() == a.cols());

    MKL_INT n    = a.cols();
    MKL_INT lda  = a.rows();
    MKL_INT ldb  = b.rows();
    MKL_INT nrhs = b.cols();

    srs::ivector ipiv(n);

    MKL_INT info = LAPACKE_dgesv(
        LAPACK_COL_MAJOR, n, nrhs, a.data(), lda, ipiv.data(), b.data(), ldb);
    if (info != 0) {
        throw Math_error("dgesv: factor U is singular");
    }
}

void srs::linsolve(const srs::sparse_dmatrix& a,
                   srs::dvector& b,
                   srs::dvector& x)
{
    Expects(b.size() == a.rows());
    x.resize(b.size());

    // Initialize PARDISO:

    void* pt[64];         // internal solver memory pointer
    MKL_INT iparm[64];    // PARDISO control parameters
    MKL_INT mtype  = 11;  // real and nonsymmetric matrix
    MKL_INT maxfct = 1;   // max factors kept in memory
    MKL_INT mnum   = 1;   // which matrix to factorize
    MKL_INT phase  = 13;  // analysis, numerical factorization, solve
    MKL_INT nrhs   = 1;   // number of rhs that need to be solved for
    MKL_INT msglvl = 0;   // no print of statistical information
    MKL_INT error  = 0;   // initialize error flag

    pardisoinit((void*)pt, &mtype, (MKL_INT*)iparm);  // set default values
    iparm[34] = 1;                                    // zero-based indexing

    // Solve linear system of equations:

    MKL_INT n = b.size();
    srs::ivector perm(n);

    // clang-format off
    pardiso(
        (void*)pt, &maxfct, &mnum, &mtype, &phase, &n, a.data(), 
        a.row_index().data(), a.columns().data(), perm.data(), &nrhs, 
        (MKL_INT*)iparm, &msglvl, b.data(), x.data(), &error);
    // clang-format on

    if (error != 0) {
        throw Math_error("could not solve sparse linear system of equations");
    }
}

//------------------------------------------------------------------------------

void srs::schmidt(srs::dmatrix& a, srs::size_t n)
{
    srs::size_t n_out   = 0;
    srs::size_t n_orb   = n;
    srs::size_t n_basis = a.rows();

    srs::dvector work(n_basis, 0.0);

    double r_min = 0.1;

    while (n_orb < n_basis) {
        srs::size_t lim = n_orb + n_basis;
        for (srs::size_t i = 0; i < lim; ++i) {
            if (n_out >= n_basis) {
                return;
            }
            auto an = a.column(n_out);
            if (i < n_orb) {
                auto ai = a.column(i);
                an      = ai;
            }
            else {
                an                  = 0.0;
                a(i - n_orb, n_out) = 1.0;
            }
            for (srs::size_t j = 0; j < n_out; ++j) {
                auto aj = a.column(j);
                work(j) = srs::dot(aj, an);
            }
            for (srs::size_t j = 0; j < n_out; ++j) {
                auto aj = a.column(j);
                an      = an - work(j) * aj;
            }
            double r = std::sqrt(srs::dot(an, an));
            if (r >= r_min) {
                ++n_out;
                an /= r;
            }
        }
        r_min /= 10.0;
        n_orb = n_out;
    }
}

//------------------------------------------------------------------------------

void srs::mkl_dgemm(const std::string& transa,
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

    // clang-format off
    cblas_dgemm(
        CblasColMajor, cblas_transa, cblas_transb, m, n, k, alpha, a.data(),
        lda, b.data(), ldb, beta, c.data(), ldc);
    // clang-format on
}

void srs::mkl_dgemv(const std::string& transa,
                    const double alpha,
                    const srs::dmatrix& a,
                    const srs::dvector& x,
                    const double beta,
                    srs::dvector& y)
{
    MKL_INT m = a.rows();
    MKL_INT n = a.cols();

    if ((transa == "N") || (transa == "n")) {
        Expects(x.size() == a.cols());
        if (y.empty()) {
            y.resize(m);
        }
    }
    else {
        Expects(x.size() == a.cols());
        if (y.empty()) {
            y.resize(n);
        }
    }

    MKL_INT lda  = m > 1 ? m : 1;
    MKL_INT incx = 1;
    MKL_INT incy = 1;

    CBLAS_TRANSPOSE cblas_transa;
    if ((transa == "T") || (transa == "t")) {
        cblas_transa = CblasTrans;
    }
    else {
        cblas_transa = CblasNoTrans;
    }

    // clang-format off
    cblas_dgemv(
        CblasColMajor, cblas_transa, m, n, alpha, a.data(), lda, x.data(),
        incx, beta, y.data(), incy);
    // clang-format on
}
