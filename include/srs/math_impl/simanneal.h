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

#ifndef SRS_MATH_SIMANNEAL_H
#define SRS_MATH_SIMANNEAL_H

#include <srs/array.h>
#include <srs/math_impl/annealfunc.h>
#include <srs/math_impl/coolschedule.h>
#include <srs/math_impl/linalg.h>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>


//------------------------------------------------------------------------------

// Error reporting:

struct Simanneal_error : std::runtime_error {
    Simanneal_error(std::string s) : std::runtime_error(s) {}
};

//------------------------------------------------------------------------------

//
// Class providing simulated annealing solver.
//
class Simanneal {
public:
    Simanneal(std::function<double(const srs::dvector&)>& fn,
              const srs::dvector& x0,
              std::istream& from,
              const std::string& key = "Simanneal");

    // Simulated annealing solver.
    void solve(double& eglobal, srs::dvector& xglobal);

private:
    // Check if simulated annealing solver is finished.
    bool finished() const;

    // Generate a new point.
    void new_point();

    // Acceptance function for simulated annealing solver.
    bool check_accept(double enew);

    // Update simulated annealing solver.
    void update();

    // Reannealing function.
    void reanneal();

    std::function<double(const srs::dvector&)>& func;

    std::unique_ptr<Annealfunc> anneal;  // annealing function
    std::unique_ptr<Coolschedule> cool;  // cooling schedule

    double etol;  // function tolerance
    double emin;  // lowest function value permitted

    double tinit;  // initial temperature
    double tcurr;  // current temperature
    double ecurr;  // current function value

    srs::dvector xinit;  // initial x value
    srs::dvector xcurr;  // current x value
    srs::dvector xbest;  // best x value
    srs::dvector ebest;  // best function values

    unsigned nminima;       // number of local minima stored
    unsigned maxiter;       // maximum number of annealing iterations (k)
    unsigned miniter;       // minimum number of annealing iterations
    unsigned maxreject;     // maximum number of consecutive rejected trials
    unsigned reanneal_int;  // reannealing interval
    unsigned kiter;         // annealing iterator
    unsigned nreject;    // iterator for number of consecutive rejected trials
    unsigned naccept;    // iterator for number of accepted trials
    unsigned nreanneal;  // iterator for number of reannealing runs

    std::mt19937_64 mt;  // random number engine
};

#endif  // SRS_MATH_SIMANNEAL_H

