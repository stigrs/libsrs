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

#include <srs/utils.h>
#include <catch/catch.hpp>
#include <fstream>
#include <string>


TEST_CASE("test_stream")
{
    SECTION("find_section")
    {
        std::ifstream from;
        srs::fopen(from, "test_find_section.inp");
        CHECK(srs::find_section(from, "test_find_section") == true);
    }

    SECTION("fopen")
    {
        std::ifstream from;
        CHECK_THROWS(srs::fopen(from, "non_existing_file"));
    }

    SECTION("lexical_cast")
    {
        using namespace srs;

        std::string s
            = lexical_cast<std::string>(lexical_cast<double>("  3.14  "));
        CHECK(s == "3.14");
    }
}
