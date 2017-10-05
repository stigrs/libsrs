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

#ifndef SRS_STREAM_H
#define SRS_STREAM_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

//
// Stream utility methods.
//

namespace srs {

// Derived class for reporting file opening errors.
struct Fopen_error : std::runtime_error {
    Fopen_error(std::string s) : std::runtime_error(s) {}
};

// Derived class for reporting lexical cast errors.
struct Bad_lexical_cast : std::bad_cast {
    const char* what() const throw() { return "bad lexical cast"; }
};

// Wrapper function for opening file stream for input.
inline void fopen(std::ifstream& from,
                  const std::string& filename,
                  std::ios_base::openmode mode = std::ios_base::in)
{
    from.open(filename.c_str(), mode);
    if (!from.is_open()) {
        throw Fopen_error("cannot open " + filename);
    }
}

// Wrapper function for opening file stream for output.
inline void fopen(std::ofstream& to,
                  const std::string& filename,
                  std::ios_base::openmode mode = std::ios_base::out)
{
    to.open(filename.c_str(), mode);
    if (!to.is_open()) {
        throw Fopen_error("cannot open " + filename);
    }
}

// Find section in input stream.
inline bool find_section(std::istream& from, const std::string& key)
{
    from.clear();
    from.seekg(0, std::ios_base::beg);

    std::string buf;
    while (std::getline(from, buf)) {
        std::istringstream iss(buf);
        iss >> buf;
        if (buf == key) {
            return true;
        }
    }
    return false;
}

// Lexical cast (type must be able to stream into and/or out of a string).
template <typename Target, typename Source>
Target lexical_cast(Source arg)
{
    std::stringstream interpreter;
    Target result;

    if (!(interpreter << arg) || !(interpreter >> result)
        || !(interpreter >> std::ws).eof()) {  // stuff left in stream?
        throw Bad_lexical_cast();
    }
    return result;
}

}  // namespace srs

#endif  // SRS_STREAM_H
