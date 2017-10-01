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

#include <srs/utils.h>
#include <catch/catch.hpp>
#include <sstream>

TEST_CASE("test_format")
{
    SECTION("fixed")
    {
        srs::Format<double> fix;
        fix.fixed().precision(2);
        double d = 1.23456;
        std::ostringstream s;
        s << fix(d);
        CHECK(s.str() == "1.23");
    }

    SECTION("scientific")
    {
        srs::Format<double> sci;
        sci.scientific().precision(4);
        double d = 1.23456e-3;
        std::ostringstream s;
        s << sci(d);
        CHECK(s.str() == "1.2346e-03");
    }

    SECTION("line")
    {
        srs::Format<char> line;
        line.width(3).fill('-');
        std::ostringstream s;
        s << line('-');
        CHECK(s.str() == "---");
    }
}
