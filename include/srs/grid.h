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
#include <gsl/gsl>
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

    // Generate [amin, amin + 1, .... amax].
    Grid(double amin, double amax) : a0{amin}, an{amax}, d{1}
    {
        n = 1 + static_cast<size_type>(amax - amin);
    }

    // Generate the list [amin, amin + dd, ..., amax].
    Grid(double amin, double amax, double dd) : a0{amin}, an{amax}, d{dd}
    {
        n = 1 + static_cast<size_type>((amax - amin) / dd);
    }

    // Initialize data by reading an input file.
    Grid(std::istream& from, const std::string& key) : a0{0}, an{0}, d{0}, n{0}
    {
        set(from, key);
    }

    // Set new grid data by reading an input file.
    void set(std::istream& from, const std::string& key);

    // Set new grid data.
    void set(double amin, double amax, double dd);

    size_type size() const { return n; }
    double start() const { return a0; }
    double max() const { return an; }
    double step() const { return d; }
    bool empty() const { return n == 0; }

    double operator[](size_type i) const;
    double operator()(size_type i) const;

private:
    double a0;    // start value
    double an;    // maximum value
    double d;     // step size (common difference)
    size_type n;  // number of steps (size of grid)
};

inline void Grid::set(double amin, double amax, double dd)
{
    a0 = amin;
    an = amax;
    d  = dd;
    n  = 1 + static_cast<size_type>((amax - amin) / dd);
}

inline double Grid::operator[](size_type i) const
{
#ifndef NDEBUG
    Expects(i < n);
#endif
    return a0 + i * d;
}

inline double Grid::operator()(size_type i) const
{
#ifndef NDEBUG
    Expects(i < n);
#endif
    return a0 + i * d;
}

#endif  // SRS_GRID_H
