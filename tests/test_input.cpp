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

#include <srs/array.h>
#include <srs/utils.h>
#include <catch/catch.hpp>
#include <map>
#include <string>


TEST_CASE("test_input")
{
    int i;
    double d;
    std::string s;

    srs::ivector iv;
    srs::uvector uv;
    srs::dvector dv;
    srs::ivector iv_ans = {1, 2, 3, 4};
    srs::uvector uv_ans = {5, 6, 7, 8};
    srs::dvector dv_ans = {0.1, 0.2, 0.3, 0.4, 0.5};

    std::map<std::string, srs::Input> data;
    data["integer"] = srs::Input(i);
    data["double"]  = srs::Input(d);
    data["string"]  = srs::Input(s);
    data["ivector"] = srs::Input(iv);
    data["uvector"] = srs::Input(uv);
    data["dvector"] = srs::Input(dv);

    std::ifstream from("test_input.inp");

    std::string key;
    while (from >> key) {
        auto it = data.find(key);
        if (it != data.end()) {
            from >> it->second;
        }
    }

    CHECK(i == 1);
    CHECK(d == 2.0);
    CHECK(s == "hello");

    for (std::size_t it = 0; it < iv_ans.size(); ++it) {
        CHECK(iv(it) == iv_ans(it));
    }
    for (std::size_t it = 0; it < uv_ans.size(); ++it) {
        CHECK(uv(it) == uv_ans(it));
    }
    for (std::size_t it = 0; it < dv_ans.size(); ++it) {
        CHECK(dv(it) == dv_ans(it));
    }
}
