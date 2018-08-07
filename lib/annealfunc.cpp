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

#include <srs/math_impl/annealfunc.h>
#include <srs/math_impl/linalg.h>
#include <algorithm>
#include <cmath>
#include <numeric>


Annealfunc::Annealfunc(const std::string& fn, double step, int seed)
{
    func     = fn;
    stepsize = step;

    if (seed == 0) {  // seed random number engine
        std::random_device rd;
        std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        mt.seed(seq);
    }
    else {
        mt.seed(seed);  // should only be used for testing purposes
    }
}

srs::dvector Annealfunc::generate(const srs::dvector& x, double temp)
{
    srs::dvector result;
    if (func == "fast") {
        result = anneal_fast(x, temp);
    }
    else if (func == "boltz") {
        result = anneal_boltz(x, temp);
    }
    else if (func == "frenkel") {
        result = anneal_frenkel(x);
    }
    else if (func == "vandekerckhove") {
        result = anneal_vandekerckhove(x);
    }
    else {
        result = anneal_fast(x, temp);
    }
    return result;
}

srs::dvector Annealfunc::anneal_fast(const srs::dvector& x, double temp)
{
    std::normal_distribution<double> rnd;
    srs::dvector y(x.size());
    for (srs::size_t i = 0; i < y.size(); ++i) {
        y(i) = rnd(mt);
    }
    return x + temp * srs::normalize(y);
}

srs::dvector Annealfunc::anneal_boltz(const srs::dvector& x, double temp)
{
    std::normal_distribution<double> rnd;
    srs::dvector y(x.size());
    for (srs::size_t i = 0; i < y.size(); ++i) {
        y(i) = rnd(mt);
    }
    return x + std::sqrt(temp) * srs::normalize(y);
}

srs::dvector Annealfunc::anneal_frenkel(const srs::dvector& x)
{
    std::uniform_real_distribution<double> rnd;
    srs::dvector xnew(x.size(), 0.0);
    for (srs::size_t i = 0; i < xnew.size(); ++i) {
        xnew(i) = x(i) + (rnd(mt) - 0.5) * stepsize;
    }
    return xnew;
}

srs::dvector Annealfunc::anneal_vandekerckhove(const srs::dvector& x)
{
    std::normal_distribution<double> rnd;

    srs::ivector perm(x.size());
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), mt);

    srs::dvector xnew = x;
    for (srs::size_t i = 0; i < xnew.size(); ++i) {
        xnew(i) += perm(i) * rnd(mt) * stepsize;
    }
    return xnew;
}
