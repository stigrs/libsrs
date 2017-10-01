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
#include <string>


TEST_CASE("test_string")
{
    SECTION("to_string")
    {
        std::string s = srs::to_string<double>(3.14);
        CHECK(s == "3.14");
    }

    SECTION("from_string")
    {
        double d = srs::from_string<double>("3.14");
        CHECK(d == 3.14);
    }

    SECTION("from_fortran_sci_fmt")
    {
        double d = srs::from_fortran_sci_fmt("3.14D-3");
        CHECK(d == 3.14e-3);
    }

    SECTION("strip_suffix")
    {
        std::string basename = srs::strip_suffix("test.txt", ".txt");
        CHECK(basename == "test");
    }

    SECTION("get_suffix")
    {
        std::string suffix = srs::get_suffix("test.txt");
        CHECK(suffix == ".txt");
    }

    SECTION("stricmp") { CHECK(srs::stricmp("Astring", "aString") == true); }
}