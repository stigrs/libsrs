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

#ifndef SRS_MATH_COOLSCHEDULE_H
#define SRS_MATH_COOLSCHEDULE_H

#include <limits>
#include <string>

//
// Class providing cooling schedule methods for simulated annealing solver.
//
// Available cooling methods:
//   exp:   exponential cooling
//   fast:  fast cooling
//   boltz: Boltzmann cooling
//
class Coolschedule {
public:
    Coolschedule() {}

    Coolschedule(double tinit,
                 double tmin_ = std::numeric_limits<double>::epsilon(),
                 const std::string& schedule_ = "exp");

    // Temperature cooling function.
    double cool(unsigned k) const;

private:
    double t0;
    double tmin;
    std::string schedule;
};

inline Coolschedule::Coolschedule(double tinit,
                                  double tmin_,
                                  const std::string& schedule_)
{
    t0       = tinit;
    tmin     = tmin_;
    schedule = schedule_;
}

#endif  // SRS_MATH_COOLSCHEDULE_H

