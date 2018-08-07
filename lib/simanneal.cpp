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

#include <srs/math_impl/core.h>
#include <srs/math_impl/linalg.h>
#include <srs/math_impl/simanneal.h>
#include <srs/utils.h>
#include <cmath>
#include <limits>
#include <map>


Simanneal::Simanneal(std::function<double(const srs::dvector&)>& fn,
                     const srs::dvector& x0,
                     std::istream& from,
                     const std::string& key)
    : func(fn)
{
    std::string anneal_func;
    std::string cool_schedule;

    double stepsize = 0.01;
    double tmin     = std::numeric_limits<double>::epsilon();
    double emin_def = -std::numeric_limits<double>::max();

    int seed = 0;

    std::map<std::string, srs::Input> input_data;
    input_data["anneal_func"]   = srs::Input(anneal_func, "fast");
    input_data["stepsize"]      = srs::Input(stepsize, stepsize);
    input_data["seed"]          = srs::Input(seed, seed);
    input_data["cool_schedule"] = srs::Input(cool_schedule, "exp");
    input_data["tinit"]         = srs::Input(tinit, 298.15);
    input_data["tmin"]          = srs::Input(tmin, tmin);
    input_data["etol"]          = srs::Input(etol, 1.0e-8);
    input_data["emin"]          = srs::Input(emin, emin_def);
    input_data["nminima"]       = srs::Input(nminima, 10);
    input_data["maxiter"]       = srs::Input(maxiter, 10000);
    input_data["maxiter"]       = srs::Input(maxiter, 1000);
    input_data["maxreject"]     = srs::Input(maxreject, 300);
    input_data["reanneal_int"]  = srs::Input(reanneal_int, 100);

    // Read input:

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

    for (auto it = input_data.begin(); it != input_data.end(); ++it) {
        if (!it->second.is_init()) {
            throw Simanneal_error(it->first + " not initialized");
        }
    }

    // TODO: Implement validation of input data

    anneal = std::make_unique<Annealfunc>(anneal_func, stepsize, seed);
    cool   = std::make_unique<Coolschedule>(tinit, tmin, cool_schedule);

    tcurr = tinit;
    xcurr = x0;
    xbest = xcurr;
    ecurr = func(xcurr);
    ebest.push_back(ecurr);

    kiter     = 1;
    nreject   = 0;
    naccept   = 0;
    nreanneal = 0;

    // Seed random number engine:

    if (seed == 0) {
        std::random_device rd;
        std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        mt.seed(seq);
    }
    else {
        mt.seed(seed);  // should only be used for testing purposes
    }
}

void Simanneal::solve(double& eglobal, srs::dvector& xglobal)
{
    while (!finished()) {
        new_point();
        update();
    }
    eglobal = srs::min(ebest);
    xglobal = xbest;
}

//------------------------------------------------------------------------------

bool Simanneal::finished() const
{
    bool result = false;
    if (ecurr <= emin) {
        result = true;
    }
    if (kiter >= maxiter) {
        result = true;
    }
    if (nreject >= maxreject) {
        result = true;
    }
    if (!ebest.empty()) {
        double ediff = ebest.end()[-1] - ebest.end()[-2];
        if ((std::abs(ediff) <= etol) && (kiter >= miniter)) {
            result = true;
        }
    }
    return result;
}

void Simanneal::new_point()
{
    auto xnew = anneal->generate(xcurr, tcurr);
    auto enew = func(xnew);
    if (check_accept(enew)) {
        xcurr = xnew;
        ecurr = enew;
        naccept += 1;
    }
}

bool Simanneal::check_accept(double enew)
{
    bool accept  = false;
    double ediff = enew - ecurr;
    if (ediff < 0.0) {
        accept  = true;
        nreject = 0;
    }
    else {
        double h = std::exp(-ediff / tcurr);
        std::uniform_real_distribution<double> rnd;
        if (h > rnd(mt)) {
            accept = true;
        }
        else {
            nreject += 1;
        }
    }
    return accept;
}

void Simanneal::update()
{
    kiter += 1;
    if ((naccept >= reanneal_int) && (kiter > 1)) {  // reanneal if needed
        reanneal();
    }
    tcurr = cool->cool(kiter);      // create new temperature
    if (ecurr < srs::min(ebest)) {  // update global minimum
        ebest.push_back(ecurr);
        xbest = xcurr;
    }
}

void Simanneal::reanneal()
{
    tcurr   = tinit;
    xcurr   = xbest;
    ecurr   = srs::min(ebest);
    naccept = 1;
    nreject = 0;
    nreanneal += 1;
}
