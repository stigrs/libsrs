////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018 Stig Rune Sellevag. All rights reserved.
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
#include <srs/math_impl/core.h>
#include <srs/math_impl/simanneal.h>
#include <srs/utils.h>
#include <catch/catch.hpp>
#include <fstream>
#include <functional>


double camel(const srs::dvector& p)
{
    double x = p(0);
    double y = p(1);
    return (4.0 - 2.1 * x * x + x * x * x * x / 3.0) * x * x + x * y
           + 4.0 * (y * y - 1.0) * y * y;
}

TEST_CASE("test_simanneal")
{
    double fglobal       = -1.0316;
    srs::dvector xglobal = {-0.0899, 0.7127};

    srs::dvector x0 = {0.0, 0.0};

    std::ifstream from;
    srs::fopen(from, "test_simanneal.inp");

    std::function<double(const srs::dvector&)> fn = camel;

    double e;
    srs::dvector x;

    Simanneal sim(fn, x0, from);
    sim.solve(e, x);

    CHECK(srs::approx_equal(e, fglobal, 1.0e-4, "reldiff"));
    CHECK(srs::approx_equal(x, xglobal, 1.0e-3, "reldiff"));
}
