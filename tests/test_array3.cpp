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

#include <srs/array.h>
#include <catch/catch.hpp>
#include <cmath>
#include <iostream>

TEST_CASE("test_array3")
{
    // clang-format off
    srs::Array<int, 3> m{
        // depth 0
        {{1, 2, 3}, 
         {4, 5, 6}, 
         {7, 8, 9}},
        // depth 1
        {{10, 11, 12}, 
         {13, 14, 15}, 
         {16, 17, 18}}
    };
    // clang-format on

    SECTION("initializer_list")
    {
        CHECK(m.rows() == 3);
        CHECK(m.cols() == 3);
        CHECK(m.depths() == 2);
        CHECK(m(0, 0, 0) == 1);
        CHECK(m(0, 1, 0) == 2);
        CHECK(m(0, 2, 0) == 3);
        CHECK(m(1, 0, 0) == 4);
        CHECK(m(1, 1, 0) == 5);
        CHECK(m(1, 2, 0) == 6);
        CHECK(m(2, 0, 0) == 7);
        CHECK(m(2, 1, 0) == 8);
        CHECK(m(2, 2, 0) == 9);
        CHECK(m(0, 0, 1) == 10);
        CHECK(m(0, 1, 1) == 11);
        CHECK(m(0, 2, 1) == 12);
        CHECK(m(1, 0, 1) == 13);
        CHECK(m(1, 1, 1) == 14);
        CHECK(m(1, 2, 1) == 15);
        CHECK(m(2, 0, 1) == 16);
        CHECK(m(2, 1, 1) == 17);
        CHECK(m(2, 2, 1) == 18);
    }

    SECTION("depth")
    {
        const auto& d0 = m.depth(0);
        CHECK(d0(0, 0) == 1);
        CHECK(d0(0, 1) == 2);
        CHECK(d0(0, 2) == 3);
        CHECK(d0(1, 0) == 4);
        CHECK(d0(1, 1) == 5);
        CHECK(d0(1, 2) == 6);
        CHECK(d0(2, 0) == 7);
        CHECK(d0(2, 1) == 8);
        CHECK(d0(2, 2) == 9);

        const auto& d1 = m.depth(1);
        CHECK(d1(0, 0) == 10);
        CHECK(d1(0, 1) == 11);
        CHECK(d1(0, 2) == 12);
        CHECK(d1(1, 0) == 13);
        CHECK(d1(1, 1) == 14);
        CHECK(d1(1, 2) == 15);
        CHECK(d1(2, 0) == 16);
        CHECK(d1(2, 1) == 17);
        CHECK(d1(2, 2) == 18);
    }

    SECTION("operator[]")
    {
        CHECK(m.rows() == 3);
        CHECK(m.cols() == 3);
        CHECK(m.depths() == 2);
        CHECK(m[0][0][0] == 1);
        CHECK(m[0][0][1] == 2);
        CHECK(m[0][0][2] == 3);
        CHECK(m[0][1][0] == 4);
        CHECK(m[0][1][1] == 5);
        CHECK(m[0][1][2] == 6);
        CHECK(m[0][2][0] == 7);
        CHECK(m[0][2][1] == 8);
        CHECK(m[0][2][2] == 9);
        CHECK(m[1][0][0] == 10);
        CHECK(m[1][0][1] == 11);
        CHECK(m[1][0][2] == 12);
        CHECK(m[1][1][0] == 13);
        CHECK(m[1][1][1] == 14);
        CHECK(m[1][1][2] == 15);
        CHECK(m[1][2][0] == 16);
        CHECK(m[1][2][1] == 17);
        CHECK(m[1][2][2] == 18);
    }

    SECTION("slice")
    {
        auto s = m.slice(1, 2, 1, 2, 0, 1);
        CHECK(s.rows() == 2);
        CHECK(s.cols() == 2);
        CHECK(s.depths() == 2);
        CHECK(s(0, 0, 0) == 5);
        CHECK(s(0, 1, 0) == 6);
        CHECK(s(1, 0, 0) == 8);
        CHECK(s(1, 1, 0) == 9);
        CHECK(s(0, 0, 1) == 14);
        CHECK(s(0, 1, 1) == 15);
        CHECK(s(1, 0, 1) == 17);
        CHECK(s(1, 1, 1) == 18);

        const auto d = s.depth(0).column(0);
        CHECK(d.size() == 2);
        CHECK(d(0) == 5);
        CHECK(d(1) == 8);
    }
}