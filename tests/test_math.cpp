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
#include <iostream>

TEST_CASE("test_math")
{
    SECTION("even_odd")
    {
        int even_number = 4;
        CHECK(srs::is_even(even_number) == true);

        int odd_number = -3;
        CHECK(srs::is_odd(odd_number) == true);
    }

    SECTION("integration")
    {
        // Abramowitz and Stegun, Table 25.4, p. 916:
        arma::vec xans = {-0.960289856497536,
                          -0.796666477413627,
                          -0.525532409916329,
                          -0.183434642495659,
                          0.183434642495659,
                          0.525532409916329,
                          0.796666477413627,
                          0.960289856497536};

        // Abramowitz and Stegun, Table 25.4, p. 916:
        arma::vec wans = {0.101228536290376,
                          0.222381034453374,
                          0.313706645877887,
                          0.362683783378362,
                          0.362683783378362,
                          0.313706645877887,
                          0.222381034453374,
                          0.101228536290376};

        int n = 8;
        arma::vec x(n);
        arma::vec w(n);
        srs::gaussleg(n, x, w);

        CHECK(arma::approx_equal(x, xans, "absdiff", 1.0e-10));
        CHECK(arma::approx_equal(w, wans, "absdiff", 1.0e-10));

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

        CHECK(arma::approx_equal(x, xans, "absdiff", 1.0e-8));
        CHECK(arma::approx_equal(w, wans, "absdiff", 1.0e-8));
    }

    SECTION("pdist_matrix")
    {
        arma::mat mat(4, 3);
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

        arma::mat dm;
        srs::pdist_matrix(dm, mat);

        arma::mat dm_ans = arma::zeros<arma::mat>(4, 4);

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

        CHECK(arma::approx_equal(dm, dm_ans, "absdiff", 1.0e-8));
    }

    SECTION("stat")
    {
        arma::vec a = {3.0,
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

        CHECK(srs::approx_equal(
            srs::mean(a), 22.066666666666666, "absdiff", 1.0e-8));
        CHECK(srs::approx_equal(srs::median(a), 23.0, "absdiff", 1.0e-8));
        CHECK(srs::approx_equal(
            srs::stddev(a), 14.49860420211283, "absdiff", 1.0e-8));
        CHECK(srs::approx_equal(
            srs::rms(a), 26.136819495365792, "absdiff", 1.0e-8));

        // clang-format off
        arma::vec b = {3, 13, 7, 5, 21, 23, 23, 40, 23, 14, 12, 56, 23, 29};
        arma::vec c = {3, 13, 7, 5, 21, 23, 39, 23, 40, 23, 14, 12, 56, 23};
        // clang-format on

        CHECK(
            srs::approx_equal(srs::cov(b, c), 59.78021978, "absdiff", 1.0e-8));
    }
}