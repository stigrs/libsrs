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

#include <srs/math_impl/grid.h>
#include <srs/utils.h>
#include <map>


void Grid::set(std::istream& from, const std::string& key)
{
    std::map<std::string, srs::Input> input_data;
    input_data["min"]  = srs::Input(a0, 0.0);
    input_data["step"] = srs::Input(d, 1.0);
    input_data["max"]  = srs::Input(an, 100.0);

    // Read input data:

    if (srs::find_section(from, key)) {
        std::string token;
        while (from >> token) {
            if (token == "End") {
                break;
            }
            auto it = input_data.find(token);
            if (it != input_data.end()) {
                from >> it->second;
            }
        }
    }

    // Check if initialized:

    for (auto& it : input_data) {
        if (!it.second.is_init()) {
            throw Grid_error(it.first + " not initialized");
        }
    }

    // Check if data are sensible:

    n = 1 + static_cast<size_type>((an - a0) / d);

    if (an < a0) {
        throw Grid_error("max has bad value: " + srs::to_string(an));
    }
    if (d < 0.0) {
        throw Grid_error("step has bad value: " + srs::to_string(d));
    }
}

//------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& to, const Grid& g)
{
    to << "Min value:\t" << g.start() << '\n'
       << "Max value:\t" << g.max() << '\n'
       << "Step size:\t" << g.step() << '\n';
    return to;
}
