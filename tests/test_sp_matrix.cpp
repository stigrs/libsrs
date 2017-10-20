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
#include <srs/sparse.h>
#include <catch/catch.hpp>
#include <vector>


TEST_CASE("sp_matrix")
{
    srs::imatrix mat = {{1, 2, 0, 4, 0},
                        {6, 7, 0, 0, 0},
                        {0, 0, 13, 14, 15},
                        {16, 0, 18, 19, 0},
                        {0, 22, 0, 0, 25}};

    srs::Sp_matrix<int> spmat = srs::sp_gather(mat);

    SECTION("element_access")
    {
        CHECK(spmat(0, 0) == 1);
        CHECK(spmat(0, 2) == 0);
    }

    SECTION("insert")
    {
        spmat.insert(0, 2, 3);
        spmat.insert(0, 4, 5);
        spmat.insert(1, 2, 8);
        spmat.insert(1, 3, 9);
        spmat.insert(1, 4, 10);
        spmat.insert(2, 0, 11);
        spmat.insert(2, 1, 12);
        spmat.insert(3, 1, 17);
        spmat.insert(3, 4, 20);
        spmat.insert(4, 0, 21);
        spmat.insert(4, 2, 23);
        spmat.insert(4, 3, 24);

        int iter = 1;
        for (std::size_t i = 0; i < 5; ++i) {
            for (std::size_t j = 0; j < 5; ++j) {
                CHECK(spmat(i, j) == iter);
                ++iter;
            }
        }
    }

    SECTION("resize")
    {
        srs::Sp_matrix<int> m;
        m.resize(1000, 500, 3);

        m.insert(0, 1, 1);
        m.insert(100, 50, 2);
        m.insert(300, 2, 3);
        CHECK(m(0, 1) == 1);
        CHECK(m(100, 50) == 2);
        CHECK(m(300, 2) == 3);
        CHECK(m(999, 499) == 0);
    }

    SECTION("scatter") { CHECK(srs::sp_scatter(spmat) == mat); }

    SECTION("mv_mul")
    {
        srs::ivector x   = {1, 2, 3, 4, 5};
        srs::ivector ans = {21, 20, 170, 146, 169};
        CHECK(ans == spmat * x);
    }
}