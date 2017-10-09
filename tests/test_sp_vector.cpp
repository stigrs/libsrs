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

#include <srs/sparse.h>
#include <algorithm>
#include <catch/catch.hpp>


TEST_CASE("sp_vector")
{
    srs::Sp_vector<int> spvec = {{10, 1}, {20, 4}, {30, 9}};

    SECTION("element_access")
    {
        CHECK(spvec.size() == 3);
        CHECK(spvec(1) == 10);
        CHECK(spvec(4) == 20);
        CHECK(spvec(5) == 0);
        CHECK(spvec(9) == 30);
        CHECK(spvec(10) == 0);
    }

    SECTION("insert")
    {
        spvec.insert(40, 5);
        CHECK(spvec.size() == 4);
        CHECK(spvec(5) == 40);
    }

    SECTION("swap")
    {
        srs::Sp_vector<int> spv1 = {{20, 2}, {30, 4}, {40, 7}};
        std::swap(spvec, spv1);
        CHECK(spvec.size() == 3);
        CHECK(spvec(1) == 0);
        CHECK(spvec(2) == 20);
        CHECK(spvec(4) == 30);
        CHECK(spvec(7) == 40);
        CHECK(spvec(9) == 0);
    }
}