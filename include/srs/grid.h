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

#ifndef SRS_GRID_H
#define SRS_GRID_H

#include <srs/types.h>
#include <stdexcept>
#include <string>

//------------------------------------------------------------------------------

// Error reporting:

struct Grid_error : std::runtime_error {
    Grid_error(std::string s) : std::runtime_error(s) {}
};

//------------------------------------------------------------------------------

//
// Class for handling grids with arithmetic progression.
//
class Grid {
public:
    typedef srs::Int_t size_type;

    Grid() : a0{0}, an{0}, d{0}, n{0} {}

    // Generate [0, 1, ..., amax].
    Grid(double amax) : a0{0}, an{amax}, d{1}
    {
        n = 1 + static_cast<size_type>(amax);
    }

private:
    double a0;    // start value
    double an;    // maximum value
    double d;     // step size (common difference)
    size_type n;  // number of steps (size of grid)
};

#endif  // SRS_GRID_H
