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

#include <srs/math_impl/coolschedule.h>
#include <algorithm>
#include <cmath>

double Coolschedule::cool(unsigned k) const
{
    double temp = t0;  // no cooling
    if (schedule == "exp") {
        temp = t0 * std::pow(0.95, k);
    }
    else if (schedule == "fast") {
        temp = t0 / static_cast<double>(k);
    }
    else if (schedule == "boltz") {
        temp = t0 / std::log(static_cast<double>(k));
        if (temp < 0.0) {
            temp = tmin;
        }
    }
    return std::max(tmin, temp);
}

