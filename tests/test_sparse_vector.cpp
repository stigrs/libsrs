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
#include <srs/sparse.h>
#include <algorithm>
#include <catch/catch.hpp>


TEST_CASE("sp_vector")
{
    srs::Sparse_vector<int> spvec = {{1, 10}, {4, 20}, {9, 30}};

    SECTION("element_access")
    {
        CHECK(spvec.num_nonzero() == 3);
        CHECK(spvec(1) == 10);
        CHECK(spvec(4) == 20);
        CHECK(spvec(5) == 0);
        CHECK(spvec(9) == 30);
        CHECK(spvec(10) == 0);
    }

    SECTION("insert")
    {
        srs::Sparse_vector<int> spv1(spvec);
        spv1.insert(40, 5);
        CHECK(spv1.num_nonzero() == 4);
        CHECK(spv1(5) == 40);
    }

    SECTION("swap")
    {
        srs::Sparse_vector<int> spv1 = {{2, 20}, {4, 30}, {7, 40}};
        srs::Sparse_vector<int> spv2(spvec);
        std::swap(spv2, spv1);
        CHECK(spv2.num_nonzero() == 3);
        CHECK(spv2(1) == 0);
        CHECK(spv2(2) == 20);
        CHECK(spv2(4) == 30);
        CHECK(spv2(7) == 40);
        CHECK(spv2(9) == 0);
    }

    SECTION("addition")
    {
        srs::ivector x(10, 1);
        auto y = 2 * spvec + x;
        CHECK(y(0) == 1);
        CHECK(y(1) == 21);
        CHECK(y(2) == 1);
        CHECK(y(3) == 1);
        CHECK(y(4) == 41);
        CHECK(y(5) == 1);
        CHECK(y(6) == 1);
        CHECK(y(7) == 1);
        CHECK(y(8) == 1);
        CHECK(y(9) == 61);
    }

    SECTION("scatter")
    {
        srs::ivector y = srs::sp_scatter(spvec);
        CHECK(y(0) == 0);
        CHECK(y(1) == 10);
        CHECK(y(2) == 0);
        CHECK(y(3) == 0);
        CHECK(y(4) == 20);
        CHECK(y(5) == 0);
        CHECK(y(6) == 0);
        CHECK(y(7) == 0);
        CHECK(y(8) == 0);
        CHECK(y(9) == 30);
    }
}