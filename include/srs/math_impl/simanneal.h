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

#ifndef SRS_MATH_SIMANNEAL_H
#define SRS_MATH_SIMANNEAL_H

#include <srs/array.h>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>


//------------------------------------------------------------------------------

// Error reporting:

struct Simanneal_error : std::runtime_error {
    Simanneal_error(std::string s) : std::runtime_error(s) {}
};

//------------------------------------------------------------------------------

//
// Class providing simulated annealing solver.
//
class Simanneal {
public:
    Simanneal(std::function<double(const srs::dvector&)>& fn,
              std::istream& from,
              const std::string& key = "Simanneal");

private:
    std::function<double(const srs::dvector&)> func;
};

#endif  // SRS_MATH_SIMANNEAL_H

