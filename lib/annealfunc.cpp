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
#include <cmath>

Annealfunc::Annealfunc(const std::string& func_, double step_, int seed)
{
    func = func_;
    step = step_;

    // Seed random number engine:

    if (seed == 0) {
        std::random_device rd;
        std::seed_seq seed_seq_{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        mt.seed(seed_seq_);
    }
    else {
        mt.seed(seed);  // should only be used for testing purposes
    }
}

srs::dvector Annealfunc::generate(const srs::dvector& x, double temp)
{
    if (func == "fast") {
        return anneal_fast(x, temp);
    }
    else if (func == "boltz") {
        return anneal_boltz(x, temp);
    }
    else if (func == "frenkel") {
        return anneal_frenkel(x);
    }
    else {
        return anneal_fast(x, temp);
    }
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
        xnew(i) = x(i) + (rnd(mt) - 0.5) * step;
    }
    return xnew;
}

