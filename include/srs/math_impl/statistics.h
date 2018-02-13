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

#ifndef SRS_MATH_STATISTICS_H
#define SRS_MATH_STATISTICS_H

#include <armadillo>


//
// Provides statistical functions.
//
namespace srs {

// Arithmetic mean.
inline double mean(const arma::vec& x)
{
    return arma::sum(x) / static_cast<double>(x.size());
}

// Geometric mean.
inline double geomean(const arma::vec& x)
{
    return std::pow(arma::prod(x), static_cast<double>(1.0 / x.size()));
}

// Harmonic mean.
double harmmean(const arma::vec& x);

// Median.
double median(arma::vec& x);

// Variance.
double var(const arma::vec& x);

// Standard deviation.
inline double stddev(const arma::vec& x) { return std::sqrt(var(x)); }

// Covariance.
double cov(const arma::vec& x, const arma::vec& y);

// Mean absolute deviation.
double mad(const arma::vec& x);

// Root-mean-square deviation.
double rms(const arma::vec& x);

}  // namespace srs

#endif  // SRS_MATH_STATISTICS_H
