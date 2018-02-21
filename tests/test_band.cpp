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
#include <srs/band.h>
#include <catch/catch.hpp>
#include <iostream>


TEST_CASE("test_band")
{
    srs::imatrix a = {{11, 12, 0, 0, 0},
                      {21, 22, 23, 0, 0},
                      {31, 32, 33, 34, 0},
                      {0, 42, 43, 44, 45},
                      {0, 0, 53, 54, 55}};

    srs::band_imatrix ab(2, 1, a);

    SECTION("element_access")
    {
        CHECK(ab(0, 0) == 11);
        CHECK(ab(0, 1) == 12);
        CHECK(ab(1, 0) == 21);
        CHECK(ab(1, 1) == 22);
        CHECK(ab(1, 2) == 23);
        CHECK(ab(2, 0) == 31);
        CHECK(ab(2, 1) == 32);
        CHECK(ab(2, 2) == 33);
        CHECK(ab(2, 3) == 34);
        CHECK(ab(3, 1) == 42);
        CHECK(ab(3, 2) == 43);
        CHECK(ab(3, 3) == 44);
        CHECK(ab(3, 4) == 45);
        CHECK(ab(4, 2) == 53);
        CHECK(ab(4, 3) == 54);
        CHECK(ab(4, 4) == 55);
    }
}