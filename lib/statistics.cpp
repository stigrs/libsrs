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

#include <srs/math_impl/statistics.h>
#include <algorithm>
#include <cmath>
#include <gsl/gsl>


double srs::harmmean(const srs::dvector& x)
{
    double sumi = 0.0;
    for (srs::size_t i = 0; i < x.size(); ++i) {
        Expects(x(i) != 0.0);
        sumi += 1.0 / x(i);
    }
    Ensures(sumi != 0.0);
    return x.size() / sumi;
}

double srs::median(srs::dvector& x)
{
    auto first = x.begin();
    auto last  = x.end();
    auto mid   = first + (last - first) / 2;

    std::nth_element(first, mid, last);
    double med = *mid;

    if ((x.size() % 2) == 0) {  // size is even
        mid = first + (last - first) / 2 - 1;
        std::nth_element(first, mid, last);
        med = (med + *mid) / 2.0;
    }
    return med;
}

double srs::var(const srs::dvector& x)
{
    // Two-pass algorithm:
    double n     = static_cast<double>(x.size());
    double xmean = srs::mean(x);
    double sum2  = 0.0;

    for (srs::size_t i = 0; i < x.size(); ++i) {
        sum2 += std::pow(x(i) - xmean, 2.0);
    }
    return sum2 / (n - 1.0);
}

double srs::mad(const srs::dvector& x)
{
    double xmean  = mean(x);
    double sumdev = 0.0;

    for (srs::size_t i = 0; i < x.size(); ++i) {
        sumdev = std::abs(x(i) - xmean);
    }
    return sumdev / static_cast<double>(x.size());
}

double srs::rms(const srs::dvector& x)
{
    double sum2 = 0.0;

    for (srs::size_t i = 0; i < x.size(); ++i) {
        sum2 += x(i) * x(i);
    }
    return std::sqrt(sum2 / static_cast<double>(x.size()));
}

double srs::cov(const srs::dvector& x, const srs::dvector& y)
{
    Expects(x.size() == y.size() && x.size() > 0);

    double n     = x.size();
    double xmean = srs::mean(x);
    double ymean = srs::mean(y);
    double cov   = 0.0;

    for (srs::size_t i = 0; i < x.size(); ++i) {
        double a = x(i) - xmean;
        double b = y(i) - ymean;
        cov += a * b / (n - 1.0);
    }
    return cov;
}
