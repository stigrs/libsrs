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

#ifndef SRS_MATH_INTEGRATION_H
#define SRS_MATH_INTEGRATION_H

#include <armadillo>
#include <gsl/gsl>


//
// Provides integration functions.
//
namespace srs {

// Integrate array of function values using the Trapezoidal rule.
double trapezoidal(double xlo, double xup, const arma::vec& y);

// Integrate array of function values using Simpson's rule.
double simpsons(double xlo, double xup, const arma::vec& y);

// Compute abscissas and weights of Gauss-Legendre n-point quadrature formula.
void gaussleg(
    int n, arma::vec& x, arma::vec& w, double a = -1.0, double b = 1.0);

}  // namespace srs

#endif  // SRS_MATH_INTEGRATION_H
