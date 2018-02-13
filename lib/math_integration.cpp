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

#include <srs/math_impl/core.h>
#include <srs/math_impl/integration.h>
#include <boost/math/special_functions/legendre.hpp>
#include <vector>


double srs::trapezoidal(double xlo, double xup, const arma::vec& y)
{
    const double step = std::abs(xup - xlo) / static_cast<double>(y.size());

    double ans = 0.0;
    ans += 0.5 * y(0);
    ans += 0.5 * y(y.size());

    for (arma::uword i = 1; i < y.size() - 1; ++i) {
        ans += y(i);
    }
    return ans *= step;
}

double srs::simpsons(double xlo, double xup, const arma::vec& y)
{
    const double step = std::abs(xup - xlo) / static_cast<double>(y.size());
    double ans;
    double f;

    ans = 0.0;
    for (arma::uword i = 0; i < y.size(); ++i) {
        if ((i == 0) || (i == y.size() - 1)) {
            f = y(i);
        }
        else if ((i % 2) == 1) {
            f = 4.0 * y(i);
        }
        else {
            f = 2.0 * y(i);
        }
        ans += f;
    }
    return ans *= step / 3.0;
}

void srs::gaussleg(int n, arma::vec& x, arma::vec& w, double a, double b)
{
    // Given the lower and upper limits of integration a and b, this
    // function returns arrays x(n) and w(n) of length n, containing the
    // abscissas and weights of the Gauss-Legendre n-point quadrature formula.

    Expects(n >= 2);
    Expects(srs::is_even(n));

    x.set_size(n);
    w.set_size(n);

    std::vector<double> xp = boost::math::legendre_p_zeros<double>(n);

    int nhalf = n / 2;
    for (int i = nhalf; i < n; ++i) {  // find x and w from 0 to +1
        int im    = i - nhalf;
        double pp = boost::math::legendre_p_prime<double>(n, xp[im]);
        x(i)      = xp[im];
        w(i)      = 2.0 / ((1.0 - xp[im] * xp[im]) * pp * pp);
    }
    for (int i = 0; i < nhalf; ++i) {  // add symmetric counterpart
        x(i) = -x(n - i - 1);
        w(i) = w(n - i - 1);
    }
    x = 0.5 * (b - a) * x;
    x += 0.5 * (a + b);  // scale to the desired interval
    w = 0.5 * (b - a) * w;
}
