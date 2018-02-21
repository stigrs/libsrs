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
#include <armadillo>
#include <catch/catch.hpp>

TEST_CASE("test_array1")
{
    srs::Array<int, 1> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    SECTION("element_access")
    {
        CHECK(v.size() == 9);
        CHECK(v(0) == 1);
        CHECK(v(1) == 2);
        CHECK(v(2) == 3);
        CHECK(v(3) == 4);
        CHECK(v(4) == 5);
        CHECK(v(5) == 6);
        CHECK(v(6) == 7);
        CHECK(v(7) == 8);
        CHECK(v(8) == 9);
    }

    SECTION("range_check") { CHECK_THROWS(v.at(10)); }

    SECTION("head")
    {
        srs::Array<int, 1> vh_ans   = {1, 2, 3, 4};
        const srs::Array<int, 1> vh = v.head(4);
        CHECK(vh == vh_ans);
    }

    SECTION("tail")
    {
        srs::Array<int, 1> vt_ans   = {4, 5, 6, 7, 8, 9};
        const srs::Array<int, 1> vt = v.tail(6);
        CHECK(vt == vt_ans);
    }

    SECTION("slice")
    {
        srs::Array<int, 1> vs_ans   = {4, 5, 6};
        const srs::Array<int, 1> vs = v.slice(3, 5);
        CHECK(vs == vs_ans);
    }

    SECTION("modify_slice")
    {
        srs::Array<int, 1> v_ans = {1, 2, 3, 0, 0, 0, 7, 8, 9};
        auto vv(v);
        auto vs = vv.slice(3, 5);
        vs      = 0;
        CHECK(vv == v_ans);
    }

    SECTION("mul_assign")
    {
        srs::Array<int, 1> vv_ans = {2, 4, 6};
        srs::Array<int, 1> vv     = v.slice(0, 2);
        vv *= 2;
        CHECK(vv == vv_ans);
    }

    SECTION("compare_equal")
    {
        srs::Array<double, 1> v1 = {2.0, 4.0, 6.0};
        srs::Array<double, 1> v2 = {2.0, 4.0, 6.0};
        CHECK(v1 == v2);
    }

    SECTION("compare_not_equal")
    {
        srs::Array<double, 1> v1 = {2.0, 4.0, 5.0};
        srs::Array<double, 1> v2 = {2.0, 4.0, 6.0};
        CHECK(v1 != v2);
    }

    SECTION("compare_less")
    {
        srs::Array<double, 1> v1 = {2.0, 4.0, 5.0};
        srs::Array<double, 1> v2 = {2.0, 4.0, 6.0};
        CHECK(v1 <= v2);
    }

    SECTION("compare_larger")
    {
        srs::Array<double, 1> v1 = {2.0, 4.0, 5.0};
        srs::Array<double, 1> v2 = {2.0, 4.0, 6.0};
        CHECK(v2 > v1);
    }

    SECTION("vector_addition")
    {
        srs::Array<double, 1> v_ans = {4.0, 8.0, 11.0};
        srs::Array<double, 1> v1    = {2.0, 4.0, 5.0};
        srs::Array<double, 1> v2    = {2.0, 4.0, 6.0};
        srs::Array<double, 1> v3    = v1 + v2;
        CHECK(v3 == v_ans);
    }
}
