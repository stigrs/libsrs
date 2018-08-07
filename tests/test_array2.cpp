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

#include <srs/array.h>
#include <srs/math.h>
#include <catch/catch.hpp>
#include <cmath>
#include <iostream>
#include <utility>


TEST_CASE("test_array2")
{
    srs::Array<double, 2> m{
        {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}};

    SECTION("initializer_list")
    {
        CHECK(m(0, 0) == 1.0);
        CHECK(m(1, 0) == 4.0);
        CHECK(m(2, 0) == 7.0);
        CHECK(m(3, 0) == 10.0);
        CHECK(m(0, 1) == 2.0);
        CHECK(m(1, 1) == 5.0);
        CHECK(m(2, 1) == 8.0);
        CHECK(m(3, 1) == 11.0);
        CHECK(m(0, 2) == 3.0);
        CHECK(m(1, 2) == 6.0);
        CHECK(m(2, 2) == 9.0);
        CHECK(m(3, 2) == 12.0);
    }

    SECTION("operator[]")
    {
        CHECK(m[0][0] == 1.0);
        CHECK(m[1][0] == 4.0);
        CHECK(m[2][0] == 7.0);
        CHECK(m[3][0] == 10.0);
        CHECK(m[0][1] == 2.0);
        CHECK(m[1][1] == 5.0);
        CHECK(m[2][1] == 8.0);
        CHECK(m[3][1] == 11.0);
        CHECK(m[0][2] == 3.0);
        CHECK(m[1][2] == 6.0);
        CHECK(m[2][2] == 9.0);
        CHECK(m[3][2] == 12.0);
    }

    SECTION("capacity")
    {
        CHECK(m.size() == 12);
        CHECK(m.rows() == 4);
        CHECK(m.cols() == 3);
    }

    SECTION("initializer_list_assign")
    {
        srs::Array<double, 2> a = {{1.0, 2.0}, {3.0, 4.0}};
        CHECK(a(0, 0) == 1.0);
        CHECK(a(0, 1) == 2.0);
        CHECK(a(1, 0) == 3.0);
        CHECK(a(1, 1) == 4.0);
    }

    SECTION("nested_array")
    {
        int a[2][3];
        a[0][0] = 0;
        a[0][1] = 1;
        a[0][2] = 2;
        a[1][0] = 3;
        a[1][1] = 4;
        a[1][2] = 5;

        srs::Array<int, 2> aa(a);
        CHECK(aa(0, 0) == 0);
        CHECK(aa(0, 1) == 1);
        CHECK(aa(0, 2) == 2);
        CHECK(aa(1, 0) == 3);
        CHECK(aa(1, 1) == 4);
        CHECK(aa(1, 2) == 5);
        CHECK(aa.rows() == 2);
        CHECK(aa.cols() == 3);
        CHECK(aa.size() == 6);
    }

    SECTION("pointer_array")
    {
        gsl::owner<int*> ptr = new int[4];
        srs::Array<int, 2> a(2, 2, ptr);
        CHECK(a.rows() == 2);
        CHECK(a.cols() == 2);
        delete[] ptr;
    }

    SECTION("a_plus_b")
    {
        srs::Array<int, 2> a = {{1, 2}, {3, 4}};
        srs::Array<int, 2> b = {{10, 20}, {30, 40}};

        auto c = a + b;

        CHECK(c(0, 0) == 11);
        CHECK(c(0, 1) == 22);
        CHECK(c(1, 0) == 33);
        CHECK(c(1, 1) == 44);
    }

    SECTION("a_minus_b")
    {
        srs::Array<int, 2> a = {{10, 20}, {30, 40}};
        srs::Array<int, 2> b = {{1, 2}, {3, 4}};

        auto c = a - b;

        CHECK(c(0, 0) == 9);
        CHECK(c(0, 1) == 18);
        CHECK(c(1, 0) == 27);
        CHECK(c(1, 1) == 36);
    }

    SECTION("copy_ctor")
    {
        srs::Array<double, 2> a = {{1.0, 2.0}, {3.0, 4.0}};
        auto b(a);
        CHECK(a == b);
        CHECK(a.size() == b.size());
        CHECK(a.rows() == b.rows());
        CHECK(a.cols() == b.cols());
    }

    SECTION("assignment")
    {
        srs::Array<double, 2> a = {{1.0, 2.0}, {3.0, 4.0}};
        auto b                  = a;
        CHECK(a == b);
        CHECK(a.size() == b.size());
        CHECK(a.rows() == b.rows());
        CHECK(a.cols() == b.cols());
    }

    SECTION("empty")
    {
        srs::Array<int, 2> a;
        CHECK(a.empty() == true);
    }

    SECTION("transpose_in")
    {
        auto mt = m;
        mt.transpose();
        CHECK(mt.rows() == 3);
        CHECK(mt.cols() == 4);
        CHECK(mt(0, 0) == 1.0);
        CHECK(mt(0, 1) == 4.0);
        CHECK(mt(0, 2) == 7.0);
        CHECK(mt(0, 3) == 10.0);
        CHECK(mt(1, 0) == 2.0);
        CHECK(mt(1, 1) == 5.0);
        CHECK(mt(1, 2) == 8.0);
        CHECK(mt(1, 3) == 11.0);
        CHECK(mt(2, 0) == 3.0);
        CHECK(mt(2, 1) == 6.0);
        CHECK(mt(2, 2) == 9.0);
        CHECK(mt(2, 3) == 12.0);
    }

    SECTION("transpose_out")
    {
        auto mt = transpose(m);
        CHECK(mt.rows() == 3);
        CHECK(mt.cols() == 4);
        CHECK(mt(0, 0) == 1.0);
        CHECK(mt(0, 1) == 4.0);
        CHECK(mt(0, 2) == 7.0);
        CHECK(mt(0, 3) == 10.0);
        CHECK(mt(1, 0) == 2.0);
        CHECK(mt(1, 1) == 5.0);
        CHECK(mt(1, 2) == 8.0);
        CHECK(mt(1, 3) == 11.0);
        CHECK(mt(2, 0) == 3.0);
        CHECK(mt(2, 1) == 6.0);
        CHECK(mt(2, 2) == 9.0);
        CHECK(mt(2, 3) == 12.0);
    }

    SECTION("transpose_square")
    {
        srs::Array<int, 2> a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        a.transpose();
        CHECK(a(0, 0) == 1);
        CHECK(a(0, 1) == 4);
        CHECK(a(0, 2) == 7);
        CHECK(a(1, 0) == 2);
        CHECK(a(1, 1) == 5);
        CHECK(a(1, 2) == 8);
        CHECK(a(2, 0) == 3);
        CHECK(a(2, 1) == 6);
        CHECK(a(2, 2) == 9);
    }

    SECTION("row")
    {
        auto mm(m);
        CHECK(mm == m);
        auto r = mm.row(0);
        CHECK(r.size() == 3);
        CHECK(r(0) == 1.0);
        CHECK(r(1) == 2.0);
        CHECK(r(2) == 3.0);
        r = mm.row(1);
        CHECK(r(0) == 4.0);
        CHECK(r(1) == 5.0);
        CHECK(r(2) == 6.0);
        r = mm.row(2);
        CHECK(r(0) == 7.0);
        CHECK(r(1) == 8.0);
        CHECK(r(2) == 9.0);
        r = mm.row(3);
        CHECK(r(0) == 10.0);
        CHECK(r(1) == 11.0);
        CHECK(r(2) == 12.0);
        r = 0.0;
        CHECK(r(0) == 0.0);
        CHECK(r(1) == 0.0);
        CHECK(r(2) == 0.0);
        CHECK(mm < m);
    }

    SECTION("column")
    {
        auto mm(m);
        CHECK(mm == m);
        auto c = mm.column(0);
        CHECK(c.size() == 4);
        CHECK(c(0) == 1.0);
        CHECK(c(1) == 4.0);
        CHECK(c(2) == 7.0);
        CHECK(c(3) == 10.0);
        c = mm.column(1);
        CHECK(c(0) == 2.0);
        CHECK(c(1) == 5.0);
        CHECK(c(2) == 8.0);
        CHECK(c(3) == 11.0);
        c = mm.column(2);
        CHECK(c(0) == 3.0);
        CHECK(c(1) == 6.0);
        CHECK(c(2) == 9.0);
        CHECK(c(3) == 12.0);
        c = 0.0;
        CHECK(c(0) == 0.0);
        CHECK(c(1) == 0.0);
        CHECK(c(2) == 0.0);
        CHECK(c(3) == 0.0);
        CHECK(mm < m);
    }

    SECTION("diag")
    {
        srs::Array<int, 2> a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        auto d = a.diag();
        CHECK(d.size() == 3);
        CHECK(d(0) == 1);
        CHECK(d(1) == 5);
        CHECK(d(2) == 9);
    }

    SECTION("slicing")
    {
        srs::Array<int, 2> a(4, 4, 1);

        auto asub = a.slice(0, 1, 0, 1);
        CHECK(asub.rows() == 2);
        CHECK(asub.cols() == 2);
        CHECK(asub(0, 0) == 1);
        CHECK(asub(0, 1) == 1);
        CHECK(asub(1, 0) == 1);
        CHECK(asub(1, 1) == 1);
        asub = 0;
        CHECK(asub(0, 0) == 0);
        CHECK(asub(0, 1) == 0);
        CHECK(asub(1, 0) == 0);
        CHECK(asub(1, 1) == 0);
        asub = 1;

        asub = a.slice(1, 2, 1, 2);
        CHECK(asub(0, 0) == 1);
        CHECK(asub(0, 1) == 1);
        CHECK(asub(1, 0) == 1);
        CHECK(asub(1, 1) == 1);
        asub = 0;
        CHECK(asub(0, 0) == 0);
        CHECK(asub(0, 1) == 0);
        CHECK(asub(1, 0) == 0);
        CHECK(asub(1, 1) == 0);
        asub = 1;

        asub = a.slice(2, 3, 2, 3);
        CHECK(asub(0, 0) == 1);
        CHECK(asub(0, 1) == 1);
        CHECK(asub(1, 0) == 1);
        CHECK(asub(1, 1) == 1);
        asub = 0;
        CHECK(asub(0, 0) == 0);
        CHECK(asub(0, 1) == 0);
        CHECK(asub(1, 0) == 0);
        CHECK(asub(1, 1) == 0);
        asub = 1;

        asub = a.slice(1, 2, 1, 3);
        CHECK(asub.rows() == 2);
        CHECK(asub.cols() == 3);
        CHECK(asub(0, 0) == 1);
        CHECK(asub(0, 1) == 1);
        CHECK(asub(0, 2) == 1);
        CHECK(asub(1, 0) == 1);
        CHECK(asub(1, 1) == 1);
        CHECK(asub(1, 2) == 1);
        asub = 0;
        CHECK(asub(0, 0) == 0);
        CHECK(asub(0, 1) == 0);
        CHECK(asub(0, 2) == 0);
        CHECK(asub(1, 0) == 0);
        CHECK(asub(1, 1) == 0);
        CHECK(asub(1, 2) == 0);
    }

    SECTION("copy_slice")
    {
        srs::Array<int, 2> a(4, 4, 1);
        srs::Array<int, 2> b = a.slice(0, 1, 0, 1);

        b = 0;

        CHECK(b.rows() == 2);
        CHECK(b.cols() == 2);
        CHECK(a(0, 0) == 1);
        CHECK(a(0, 1) == 1);
        CHECK(a(1, 0) == 1);
        CHECK(a(1, 1) == 1);
        CHECK(b(0, 0) == 0);
        CHECK(b(0, 1) == 0);
        CHECK(b(1, 0) == 0);
        CHECK(b(1, 1) == 0);
    }

    SECTION("const_slice")
    {
        srs::Array<int, 2> a(4, 4, 1);
        const auto b = a.slice(0, 1, 0, 1);

        // b = 0; should give a compiler error.
        CHECK(b.rows() == 2);
        CHECK(b.cols() == 2);
        CHECK(a(0, 0) == 1);
        CHECK(a(0, 1) == 1);
        CHECK(a(1, 0) == 1);
        CHECK(a(1, 1) == 1);
        CHECK(b(0, 0) == 1);
        CHECK(b(0, 1) == 1);
        CHECK(b(1, 0) == 1);
        CHECK(b(1, 1) == 1);
    }

    SECTION("swap")
    {
        std::swap(m, m);
        CHECK(m == m);

        srs::imatrix a1 = {{-1, 0, -6}, {6, 5, 2}, {11, 12, 3}};
        srs::imatrix a2 = {{-1, 0, -6}, {6, 5, 2}, {11, 12, 3}};
        srs::imatrix a3 = {{11, 12, 3}, {6, 5, 2}, {-1, 0, -6}};
        std::swap(a2, a3);
        CHECK(a3 == a1);
    }

    SECTION("row_subtraction")
    {
        srs::dvector rans = {3.0, 3.0, 3.0};

        const auto mm = m;
        const auto rr = mm.row(1) - mm.row(0);
        auto r        = m.row(1) - m.row(0);

        CHECK(rr == rans);
        CHECK(r == rans);
    }

    SECTION("sort")
    {
        srs::imatrix a = {{-1, 0, 3}, {11, 5, 2}, {6, 12, -6}};

        srs::imatrix aca = {{-1, 0, -6}, {6, 5, 2}, {11, 12, 3}};
        srs::imatrix acd = {{11, 12, 3}, {6, 5, 2}, {-1, 0, -6}};

        srs::imatrix ara = {{-1, 0, 3}, {2, 5, 11}, {-6, 6, 12}};
        srs::imatrix ard = {{3, 0, -1}, {11, 5, 2}, {12, 6, -6}};

        srs::imatrix tmp = a;
        srs::sort(tmp, 2, true);
        CHECK(tmp == aca);

        tmp = a;
        srs::sort(tmp, 2, false);
        CHECK(tmp == acd);

        tmp = a;
        srs::sort(tmp, 1, true);
        CHECK(tmp == ara);

        tmp = a;
        srs::sort(tmp, 1, false);
        CHECK(tmp == ard);
    }

    SECTION("subarray_multiplication")
    {
        const srs::Array<int, 2> a(4, 4, 1);
        const srs::Array<int, 2> b(2, 2, 2);
        const srs::Array<int, 1> c(2, 3);
        const srs::Array<int, 2> mm = {{4, 4}, {4, 4}};
        const srs::Array<int, 1> mv = {6, 6};

        auto asub = a.slice(0, 1, 0, 1);

        CHECK(asub * b == mm);
        CHECK(asub * c == mv);
    }

    SECTION("prod")
    {
        srs::imatrix a = {{-1, 0, 3}, {11, 5, 2}, {6, 12, -6}};
        srs::ivector c = {-66, 0, -36};
        srs::ivector r = {0, 110, -432};
        CHECK(srs::prod(a, 2) == c);
        CHECK(srs::prod(a, 1) == r);
    }
}
