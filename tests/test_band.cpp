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

    srs::Band_matrix<int> ab(2, 1, a);
    for (auto v : ab) {
        std::cout << v << std::endl;
    }
    std::cout << ab << std::endl;
}