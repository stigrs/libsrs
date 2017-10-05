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

#include <srs/array.h>
#include <srs/math_impl/linalg.h>


//
// Provides statistical functions.
//
namespace srs {

// Arithmetic mean.
inline double mean(const dvector& x)
{
    return sum(x) / static_cast<double>(x.size());
}

// Geometric mean.
inline double geomean(const dvector& x)
{
    return std::pow(srs::prod(x), static_cast<double>(1.0 / x.size()));
}

// Harmonic mean.
double harmmean(const dvector& x);

// Median.
double median(dvector& x);

// Variance.
double var(const dvector& x);

// Standard deviation.
inline double stddev(const dvector& x) { return std::sqrt(var(x)); }

// Covariance.
double cov(const dvector& x, const dvector& y);

// Mean absolute deviation.
double mad(const dvector& x);

// Root-mean-square deviation.
double rms(const dvector& x);

}  // namespace srs

#endif  // SRS_MATH_STATISTICS_H
