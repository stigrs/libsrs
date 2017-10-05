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
#include <catch/catch.hpp>
#include <cmath>
#include <iostream>

TEST_CASE("test_array4")
{
    // clang-format off
    srs::Array<int, 4> m{  
        { // layer 0
            {{1, 2, 3}, 
             {4, 5, 6}, 
             {7, 8, 9}},
            {{10, 11, 12}, 
             {13, 14, 15}, 
             {16, 17, 18}}
        },
        { // layer 1
            {{19, 20, 21}, 
             {22, 23, 24}, 
             {25, 26, 27}},
            {{28, 29, 30}, 
             {31, 32, 33}, 
             {34, 35, 36}}
        }
    };
    // clang-format on

    SECTION("initializer_list")
    {
        CHECK(m.dim1() == 3);
        CHECK(m.dim2() == 3);
        CHECK(m.dim3() == 2);
        CHECK(m.dim4() == 2);
        int iter = 1;
        for (std::size_t l = 0; l < m.dim4(); ++l) {
            for (std::size_t k = 0; k < m.dim3(); ++k) {
                for (std::size_t i = 0; i < m.dim1(); ++i) {
                    for (std::size_t j = 0; j < m.dim2(); ++j) {
                        CHECK(m(i, j, k, l) == iter);
                        ++iter;
                    }
                }
            }
        }
    }
}