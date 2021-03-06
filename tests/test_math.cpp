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

#include <srs/math.h>
#include <armadillo>
#include <catch/catch.hpp>
#include <complex>
#include <functional>

double f(double x) { return x * x; }

TEST_CASE("test_math")
{
    SECTION("even_odd")
    {
        int even_number = 4;
        CHECK(srs::is_even(even_number) == true);

        int odd_number = -3;
        CHECK(srs::is_odd(odd_number) == true);
    }

    SECTION("derivation")
    {
        CHECK(srs::approx_equal(srs::dfdx(f, 2.0), 4.0, 1.0e-10));
    }

    SECTION("integration")
    {
        // Abramowitz and Stegun, Table 25.4, p. 916:
        srs::dvector xans = {-0.960289856497536,
                             -0.796666477413627,
                             -0.525532409916329,
                             -0.183434642495659,
                             0.183434642495659,
                             0.525532409916329,
                             0.796666477413627,
                             0.960289856497536};

        // Abramowitz and Stegun, Table 25.4, p. 916:
        srs::dvector wans = {0.101228536290376,
                             0.222381034453374,
                             0.313706645877887,
                             0.362683783378362,
                             0.362683783378362,
                             0.313706645877887,
                             0.222381034453374,
                             0.101228536290376};

        int n = 8;
        srs::dvector x(n);
        srs::dvector w(n);
        srs::gaussleg(n, x, w);

        CHECK(srs::approx_equal(x, xans, 1.0e-10));
        CHECK(srs::approx_equal(w, wans, 1.0e-10));

        // Numpy:
        xans = {1.01985507,
                1.10166676,
                1.2372338,
                1.40828268,
                1.59171732,
                1.7627662,
                1.89833324,
                1.98014493};

        // Numpy:
        wans = {0.05061427,
                0.11119052,
                0.15685332,
                0.18134189,
                0.18134189,
                0.15685332,
                0.11119052,
                0.05061427};

        srs::gaussleg(n, x, w, 1.0, 2.0);

        CHECK(srs::approx_equal(x, xans, 1.0e-8));
        CHECK(srs::approx_equal(w, wans, 1.0e-8));
    }

    SECTION("pdist_matrix")
    {
        srs::dmatrix mat(4, 3);
        mat(0, 0) = 0.0;
        mat(0, 1) = 0.0;
        mat(0, 2) = 0.0;
        mat(1, 0) = 1.0;
        mat(1, 1) = 1.0;
        mat(1, 2) = 1.0;
        mat(2, 0) = 2.0;
        mat(2, 1) = 2.0;
        mat(2, 2) = 2.0;
        mat(3, 0) = 3.0;
        mat(3, 1) = 3.0;
        mat(3, 2) = 3.0;

        srs::dmatrix dm;
        srs::pdist_matrix(dm, mat);

        srs::dmatrix dm_ans(4, 4, 0.0);

        dm_ans(0, 1) = 1.73205081;
        dm_ans(0, 2) = 3.46410162;
        dm_ans(0, 3) = 5.19615242;
        dm_ans(1, 2) = 1.73205081;
        dm_ans(1, 3) = 3.46410162;
        dm_ans(2, 3) = 1.73205081;
        dm_ans(1, 0) = dm_ans(0, 1);
        dm_ans(2, 0) = dm_ans(0, 2);
        dm_ans(3, 0) = dm_ans(0, 3);
        dm_ans(2, 1) = dm_ans(1, 2);
        dm_ans(3, 1) = dm_ans(1, 3);
        dm_ans(3, 2) = dm_ans(2, 3);

        CHECK(srs::approx_equal(dm, dm_ans, 1.0e-12));
    }

    SECTION("norm")
    {
        arma::vec v1    = {1.0, 2.0, 3.0};
        arma::vec v3    = {0.0, 0.0, 0.0};
        srs::dvector v2 = {1.0, 2.0, 3.0};
        srs::dvector v4 = {0.0, 0.0, 0.0};

        arma::vec n1 = arma::normalise(v1);
        auto n2      = srs::normalize(v2);

        CHECK(srs::approx_equal(srs::norm(v2, 1), arma::norm(v1, 1), 1.0e-12));
        CHECK(srs::approx_equal(srs::norm(v2, 2), arma::norm(v1, 2), 1.0e-12));
        CHECK(srs::approx_equal(srs::norm(v2, 3), arma::norm(v1, 3), 1.0e-12));
        CHECK(srs::approx_equal(srs::norm(v2, 4), arma::norm(v1, 4), 1.0e-12));
        CHECK(srs::approx_equal(
            srs::norm(v2, 40), arma::norm(v1, "inf"), 1.0e-12));

        for (int i = 0; i < v2.size(); ++i) {
            CHECK(srs::approx_equal(n1(i), n2(i), 1.0e-18));
        }

        arma::mat m1    = {{-4, -3, -2}, {-1, 0, 1}, {2, 3, 4}};
        srs::dmatrix m2 = {{-4, -3, -2}, {-1, 0, 1}, {2, 3, 4}};
        CHECK(srs::approx_equal(srs::norm(m2), arma::norm(m1, "fro"), 1.0e-12));
        CHECK(srs::approx_equal(
            srs::norm(m2, srs::L1), arma::norm(m1, 1), 1.0e-12));
        CHECK(srs::approx_equal(
            srs::norm(m2, srs::Inf), arma::norm(m1, "inf"), 1.0e-12));
    }

    SECTION("sum_prod_max_min")
    {
        srs::ivector v = {1, 2, 3, 4};
        CHECK(srs::min(v) == 1);
        CHECK(srs::max(v) == 4);
        CHECK(srs::sum(v) == 10);
        CHECK(srs::prod(v) == 24);
    }

    SECTION("dot_product")
    {
        srs::dvector a1 = {1.0, 2.0, 3.0, 4.0};
        srs::dvector a2 = {5.0, 6.0, 7.0, 8.0};

        arma::vec b1 = {1.0, 2.0, 3.0, 4.0};
        arma::vec b2 = {5.0, 6.0, 7.0, 8.0};

        CHECK(srs::approx_equal(srs::dot(a1, a2), arma::dot(b1, b2), 1.0e-18));
    }

    SECTION("cross_product")
    {
        srs::dvector a1 = {1.0, 2.0, 3.0};
        srs::dvector a2 = {4.0, 5.0, 6.0};
        srs::dvector a3 = srs::cross(a1, a2);

        arma::vec b1 = {1.0, 2.0, 3.0};
        arma::vec b2 = {4.0, 5.0, 6.0};
        arma::vec b3 = arma::cross(b1, b2);

        for (int i = 0; i < a3.size(); ++i) {
            CHECK(srs::approx_equal(a3(i), b3(i), 1.0e-18));
        }
    }

    SECTION("convolution")
    {
        srs::ivector ans = {4, 13, 28, 34, 32, 21};  // numpy
        srs::ivector a   = {1, 2, 3};
        srs::ivector b   = {4, 5, 6, 7};
        srs::ivector c   = srs::conv(a, b);
        CHECK(c == ans);
    }

    SECTION("det")
    {
        const double ans2 = 13.0;
        const double ans3 = 76.0;
        const double ans4 = 242.0;  // armadillo

        srs::dmatrix a2 = {{1.0, 5.0}, {-2.0, 3.0}};

        srs::dmatrix a3 = {{1.0, 5.0, 4.0}, {-2.0, 3.0, 6.0}, {5.0, 1.0, 0.0}};

        srs::dmatrix a4 = {{1.0, 5.0, 4.0, 2.0},
                           {-2.0, 3.0, 6.0, 4.0},
                           {5.0, 1.0, 0.0, -1.0},
                           {2.0, 3.0, -4.0, 0.0}};

        CHECK(srs::approx_equal(srs::det(a2), ans2, 1.0e-12));
        CHECK(srs::approx_equal(srs::det(a3), ans3, 1.0e-12));
        CHECK(srs::approx_equal(srs::det(a4), ans4, 1.0e-12));
    }

    SECTION("jacobi")
    {
        arma::mat aa = {{1.0, 0.5, 1. / 3., 1. / 4., 1. / 5},
                        {0.5, 1. / 3., 1. / 4., 1. / 5., 1. / 6.},
                        {1. / 3., 1. / 4., 1. / 5., 1. / 6., 1. / 7.},
                        {1. / 4., 1. / 5., 1. / 6., 1. / 7., 1. / 8.},
                        {1. / 5., 1. / 6., 1. / 7., 1. / 8., 1. / 9.}};

        arma::mat eigvec;
        arma::vec eigval;
        arma::eig_sym(eigval, eigvec, aa);

        srs::dmatrix a = srs::hilbert(5);
        srs::dvector wr(5);
        srs::jacobi(a, wr);

        for (int i = 0; i < wr.size(); ++i) {
            CHECK(srs::approx_equal(wr(i), eigval(i), 1.0e-12));
        }
        for (int j = 0; j < a.cols(); ++j) {
            for (int i = 0; i < a.rows(); ++i) {
                CHECK(srs::approx_equal(
                    std::abs(a(i, j)), std::abs(eigvec(i, j)), 1.0e-12));
            }
        }
    }

    SECTION("eigs")
    {
        arma::mat aa = {{1.0, 0.5, 1. / 3., 1. / 4., 1. / 5},
                        {0.5, 1. / 3., 1. / 4., 1. / 5., 1. / 6.},
                        {1. / 3., 1. / 4., 1. / 5., 1. / 6., 1. / 7.},
                        {1. / 4., 1. / 5., 1. / 6., 1. / 7., 1. / 8.},
                        {1. / 5., 1. / 6., 1. / 7., 1. / 8., 1. / 9.}};

        arma::mat eigvec;
        arma::vec eigval;
        arma::eig_sym(eigval, eigvec, aa);

        srs::dmatrix a = srs::hilbert(5);
        srs::dvector wr(5);
        srs::eigs(a, wr);

        for (int i = 0; i < wr.size(); ++i) {
            CHECK(srs::approx_equal(wr(i), eigval(i), 1.0e-12));
        }
        for (int j = 0; j < a.cols(); ++j) {
            for (int i = 0; i < a.rows(); ++i) {
                CHECK(srs::approx_equal(
                    std::abs(a(i, j)), std::abs(eigvec(i, j)), 1.0e-12));
            }
        }
    }

    SECTION("eigs_band")
    {
        arma::mat aa = {{1.0, -2.0, 0.0, 0.0, 0.0},
                        {-2.0, -1.0, 3.0, 0.0, 0.0},
                        {0.0, 3.0, 0.0, -1.0, 0.0},
                        {0.0, 0.0, -1.0, 1.0, 4.0},
                        {0.0, 0.0, 0.0, 4.0, 3.0}};

        arma::mat eigvec;
        arma::vec eigval;
        arma::eig_sym(eigval, eigvec, aa);

        srs::dmatrix a = {{1.0, -2.0, 0.0, 0.0, 0.0},
                          {-2.0, -1.0, 3.0, 0.0, 0.0},
                          {0.0, 3.0, 0.0, -1.0, 0.0},
                          {0.0, 0.0, -1.0, 1.0, 4.0},
                          {0.0, 0.0, 0.0, 4.0, 3.0}};

        srs::band_dmatrix ab(1, 1, a);
        srs::dmatrix v;
        srs::dvector w;
        srs::eigs(ab, v, w);

        for (int i = 0; i < w.size(); ++i) {
            CHECK(srs::approx_equal(w(i), eigval(i), 1.0e-12));
        }
        for (int j = 0; j < v.cols(); ++j) {
            for (int i = 0; i < v.rows(); ++i) {
                CHECK(srs::approx_equal(
                    std::abs(v(i, j)), std::abs(eigvec(i, j)), 1.0e-12));
            }
        }
    }

    SECTION("eigs_packed")
    {
        arma::mat aa = {{1.0, 0.5, 1. / 3., 1. / 4., 1. / 5},
                        {0.5, 1. / 3., 1. / 4., 1. / 5., 1. / 6.},
                        {1. / 3., 1. / 4., 1. / 5., 1. / 6., 1. / 7.},
                        {1. / 4., 1. / 5., 1. / 6., 1. / 7., 1. / 8.},
                        {1. / 5., 1. / 6., 1. / 7., 1. / 8., 1. / 9.}};

        arma::mat eigvec;
        arma::vec eigval;
        arma::eig_sym(eigval, eigvec, aa);

        srs::packed_dmatrix ap(srs::hilbert(5));
        srs::dvector w(5);
        srs::dmatrix v(5, 5);
        srs::eigs(ap, v, w);

        for (int i = 0; i < w.size(); ++i) {
            CHECK(srs::approx_equal(w(i), eigval(i), 1.0e-12));
        }
        for (int j = 0; j < v.cols(); ++j) {
            for (int i = 0; i < v.rows(); ++i) {
                CHECK(srs::approx_equal(
                    std::abs(v(i, j)), std::abs(eigvec(i, j)), 1.0e-12));
            }
        }
    }

    SECTION("eig")
    {
        arma::mat a1 = {{1.0, 5.0, 4.0, 2.0},
                        {-2.0, 3.0, 6.0, 4.0},
                        {5.0, 1.0, 0.0, -1.0},
                        {2.0, 3.0, -4.0, 0.0}};

        arma::cx_mat eigvec;
        arma::cx_vec eigval;
        arma::eig_gen(eigval, eigvec, a1);

        srs::dmatrix a2 = {{1.0, 5.0, 4.0, 2.0},
                           {-2.0, 3.0, 6.0, 4.0},
                           {5.0, 1.0, 0.0, -1.0},
                           {2.0, 3.0, -4.0, 0.0}};
        srs::zmatrix v(4, 4);
        srs::zvector w(4);
        srs::eig(a2, v, w);

        for (int i = 0; i < w.size(); ++i) {
            CHECK(srs::approx_equal(w(i).real(), eigval(i).real(), 1.0e-12));
            CHECK(srs::approx_equal(w(i).imag(), eigval(i).imag(), 1.0e-12));
        }
        for (int j = 0; j < v.cols(); ++j) {
            for (int i = 0; i < v.rows(); ++i) {
                CHECK(srs::approx_equal(
                    v(i, j).real(), eigvec(i, j).real(), 1.0e-12));
                CHECK(srs::approx_equal(
                    v(i, j).imag(), eigvec(i, j).imag(), 1.0e-12));
            }
        }
    }

    SECTION("eig_band")
    {
        // Example from Intel MKL:

        double A[77]
            = {0.0, 0.0, 0.0, 5.0, 2.0, 1.0, 1.0, 0.0, 0.0, 2.0, 6.0, 3.0, 1.0,
               1.0, 0.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0, 1.0, 1.0, 3.0, 6.0, 3.0,
               1.0, 1.0, 1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0, 1.0, 1.0, 3.0, 6.0,
               3.0, 1.0, 1.0, 1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0, 1.0, 1.0, 3.0,
               6.0, 3.0, 1.0, 1.0, 1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 0.0, 1.0, 1.0,
               3.0, 6.0, 2.0, 0.0, 0.0, 1.0, 1.0, 2.0, 5.0, 0.0, 0.0, 0.0};

        double eig[6];
        eig[0] = 3.1715728752538100;
        eig[1] = 4.0000000000000000;
        eig[2] = 4.0000000000000000;
        eig[3] = 4.1292484841890931;
        eig[4] = 4.4066499006731521;
        eig[5] = 6.0000000000000000;

        srs::band_dmatrix ab(11, 11, 3, 3, A);
        srs::dmatrix v(ab.rows(), ab.cols());
        srs::dvector w(ab.cols());

        double emin = 3.0;
        double emax = 7.0;

        srs::eig(emin, emax, ab, v, w);
        for (int i = 0; i < 6; ++i) {
            CHECK(srs::approx_equal(w(i), gsl::at(eig, i), 1.0e-12));
        }
    }

    SECTION("eig_sparse")
    {
        // Example from Intel MKL:

        // clang-format off
        int rows[12] = {0, 4, 9, 15, 22, 29, 36, 43, 50, 56, 61, 65};
        int cols[65] = {0,   1,   2,   3,
                        0,   1,   2,   3,   4,
                        0,   1,   2,   3,   4,   5,
                        0,   1,   2,   3,   4,   5,   6,
                             1,   2,   3,   4,   5,   6,   7,
                                  2,   3,   4,   5,   6,   7,   8,
                                       3,   4,   5,   6,   7,   8,  9,
                                            4,   5,   6,   7,   8,  9,  10,
                                                 5,   6,   7,   8,  9,  10,
                                                      6,   7,   8,  9,  10,
                                                           7,   8,  9,  10
        };
        double val[65] = {5.0, 2.0, 1.0, 1.0,
                          2.0, 6.0, 3.0, 1.0, 1.0,
                          1.0, 3.0, 6.0, 3.0, 1.0, 1.0,
                          1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0,
                               1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0,
                                    1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0,
                                         1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0,
                                              1.0, 1.0, 3.0, 6.0, 3.0, 1.0, 1.0,
                                                   1.0, 1.0, 3.0, 6.0, 3.0, 1.0,
                                                        1.0, 1.0, 3.0, 6.0, 2.0,
                                                             1.0, 1.0, 2.0, 5.0
        };
        // clang-format on

        double eig[6];
        eig[0] = 3.1715728752538100;
        eig[1] = 4.0000000000000000;
        eig[2] = 4.0000000000000000;
        eig[3] = 4.1292484841890931;
        eig[4] = 4.4066499006731521;
        eig[5] = 6.0000000000000000;

        srs::sparse_dmatrix a(11, 11, val, cols, rows);
        srs::dmatrix v(a.rows(), a.cols());
        srs::dvector w(a.cols());

        double emin = 3.0;
        double emax = 7.0;

        srs::eig(emin, emax, a, v, w);
        for (int i = 0; i < 6; ++i) {
            CHECK(srs::approx_equal(w(i), gsl::at(eig, i), 1.0e-12));
        }
    }

    SECTION("inv")
    {
        arma::mat aa = {{1.0, 5.0, 4.0, 2.0},
                        {-2.0, 3.0, 6.0, 4.0},
                        {5.0, 1.0, 0.0, -1.0},
                        {2.0, 3.0, -4.0, 0.0}};

        srs::dmatrix a = {{1.0, 5.0, 4.0, 2.0},
                          {-2.0, 3.0, 6.0, 4.0},
                          {5.0, 1.0, 0.0, -1.0},
                          {2.0, 3.0, -4.0, 0.0}};

        aa = arma::inv(aa);
        srs::inv(a);

        for (int j = 0; j < a.cols(); ++j) {
            for (int i = 0; i < a.rows(); ++i) {
                CHECK(srs::approx_equal(a(i, j), aa(i, j), 1.0e-12));
            }
        }
    }

    SECTION("linsolve")
    {
        double a[] = {1, 2, 3, 2, 3, 4, 3, 4, 1};
        double b[] = {14, 20, 14};

        srs::dmatrix A(3, 3, &a[0]);
        srs::dmatrix B(3, 1, &b[0]);

        arma::mat AA(3, 3);
        arma::vec BB = {14.0, 20.0, 14.0};

        for (int j = 0; j < A.cols(); ++j) {
            for (int i = 0; i < A.rows(); ++i) {
                AA(i, j) = A(i, j);
            }
        }

        arma::vec x = arma::solve(AA, BB);
        srs::linsolve(A, B);

        for (int i = 0; i < B.rows(); ++i) {
            CHECK(srs::approx_equal(B(i, 0), x(i), 1.0e-12));
        }
    }

    SECTION("stat")
    {
        srs::dvector a = {3.0,
                          13.0,
                          7.0,
                          5.0,
                          21.0,
                          23.0,
                          39.0,
                          23.0,
                          40.0,
                          23.0,
                          14.0,
                          12.0,
                          56.0,
                          23.0,
                          29.0};

        CHECK(srs::approx_equal(srs::mean(a), 22.066666666666666, 1.0e-8));
        CHECK(srs::approx_equal(srs::median(a), 23.0, 1.0e-8));
        CHECK(srs::approx_equal(srs::stddev(a), 14.49860420211283, 1.0e-8));
        CHECK(srs::approx_equal(srs::rms(a), 26.136819495365792, 1.0e-8));

        // clang-format off
        srs::dvector b = {3, 13, 7, 5, 21, 23, 23, 40, 23, 14, 12, 56, 23, 29};
        srs::dvector c = {3, 13, 7, 5, 21, 23, 39, 23, 40, 23, 14, 12, 56, 23};
        // clang-format on

        CHECK(srs::approx_equal(srs::cov(b, c), 59.78021978, 1.0e-8));
    }

    SECTION("trace")
    {
        srs::imatrix a  = {{-1, 0, 3}, {11, 5, 2}, {6, 12, -6}};
        const auto asub = a.slice(0, 1, 0, 1);
        CHECK(srs::trace(a) == -2);
        CHECK(srs::trace(asub) == 4);
    }

    SECTION("sparse_linsolve")
    {
        using size_type = srs::dvector::size_type;

        srs::dmatrix m = {{1, -1, 0, -3, 0},
                          {-2, 5, 0, 0, 0},
                          {0, 0, 4, 6, 4},
                          {-4, 0, 2, 7, 0},
                          {0, 8, 0, 0, -5}};

        // Armadillo:
        srs::dvector xans = {-2.0015, 0.1994, 1.7314, -1.0670, 0.1190};

        srs::sparse_dmatrix a = srs::sparse_gather(m);
        srs::dvector b        = {1.0, 5.0, 1.0, 4.0, 1.0};
        srs::dvector x(b.size());
        srs::linsolve(a, b, x);

        for (size_type i = 0; i < x.size(); ++i) {
            CHECK(srs::approx_equal(x(i), xans(i), 1.0e-4));
        }
    }

    SECTION("zeros")
    {
        srs::ivector a = srs::zeros<srs::Array<int, 1>>(3);
        srs::imatrix b = srs::zeros<srs::Array<int, 2>>(3, 4);
        CHECK(a.size() == 3);
        CHECK(b.rows() == 3);
        CHECK(b.cols() == 4);
    }

    SECTION("ones")
    {
        srs::ivector a = srs::ones<srs::Array<int, 1>>(3);
        CHECK(a.size() == 3);
        CHECK(a(0) == 1);
        CHECK(a(1) == 1);
        CHECK(a(2) == 1);
    }

    SECTION("euler")
    {
        CHECK(srs::approx_equal(srs::eul2rotm(), srs::identity(3), 1.0e-12));

        srs::dmatrix ans = {{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}};
        CHECK(srs::approx_equal(srs::eul2rotm(0.0, 90.0, 0.0), ans, 1.0e-12));

        srs::dvector eul = {0.0, 90.0, 0.0};
        CHECK(srs::approx_equal(srs::rotm2eul(ans), eul, 1.0e-12));

        eul = {0.0, 0.0, 0.0};
        CHECK(srs::approx_equal(srs::rotm2eul(srs::identity(3)), eul, 1.0e-12));

        srs::dvector qans = {0.7071, 0.0, 0.7071, 0.0};
        CHECK(srs::approx_equal(srs::eul2quat(0.0, 90.0, 0.0), qans, 1.0e-5));
    }

    SECTION("quaternion")
    {
        srs::dvector quat = {0.707107, 0.0, 0.707107, 0.0};
        srs::dmatrix ans = {{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}};
        CHECK(srs::approx_equal(srs::quat2rotm(quat), ans, 1.0e-12));
    }
}
