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

#ifndef SRS_MATH_DERIVATION_H
#define SRS_MATH_DERIVATION_H

#include <cmath>
#include <functional>
#include <limits>

namespace srs {

// Compute the numerical first derivative of the function f(x).
double dfdx(std::function<double(double)> f, double x)
{
    double eps = std::numeric_limits<double>::epsilon();
    double h = std::pow(eps, 1.0 / 3.0) * x;  // Numerical Recipes in C, ch. 5.7

    return (f(x + h) - f(x - h)) / (2.0 * h);
}

}  // namespace srs

#endif  // SRS_MATH_DERIVATION_H
