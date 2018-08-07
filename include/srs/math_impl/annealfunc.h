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

#ifndef SRS_MATH_ANNEALFUNC_H
#define SRS_MATH_ANNEALFUNC_H

#include <srs/array.h>
#include <random>
#include <string>

//
// Class for generating a new neighbour point for simulated annealing solver.
//
// Available annealing functions:
//   fast:    from Student's t distribution
//   boltz:   from multivariate normal distribution
//   frenkel: from Algorithm 2 of Frenkel and Smit (2002)
//
class Annealfunc {
public:
    Annealfunc() {}

    Annealfunc(const std::string& func_, double step_ = 0.01, int seed = 0);

    // Wrapper function for annealing function.
    srs::dvector generate(const srs::dvector& x, double temp);

private:
    // Generates a point using Student's t distribution.
    srs::dvector anneal_fast(const srs::dvector& x, double temp);

    // Generates a point using multivariate normal distribution.
    srs::dvector anneal_boltz(const srs::dvector& x, double temp);

    // Generates a point using Algorithm 2 of Frenkel and Smit (2002).
    srs::dvector anneal_frenkel(const srs::dvector& x);

    // Generates a point using an algorithm by Joachim Vandekerckhove.
    srs::dvector anneal_vandekerckhove(const srs::dvector& x);

    std::string func;
    double step;

    std::mt19937_64 mt;  // random number engine
};

#endif  // SRS_MATH_ANNEALFUNC_H

