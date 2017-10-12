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
#include <catch/catch.hpp>
#include <vector>


TEST_CASE("sp_matrix")
{
    std::vector<int> val = {1, 2, 4, 6, 7, 13, 14, 15, 16, 18, 19, 22, 25};
    std::vector<std::size_t> col = {0, 1, 3, 0, 1, 2, 3, 4, 0, 2, 3, 1, 4};
    std::vector<std::size_t> row = {0, 3, 5, 8, 11, 13};

    srs::Sp_matrix<int> spmat(5, 5, val, col, row);

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
}