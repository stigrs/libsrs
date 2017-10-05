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

#ifndef SRS_STRING_H
#define SRS_STRING_H

#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

//
// String utility methods.
//

namespace srs {

// Derived class for reporting string cast errors.
struct Bad_from_string : std::runtime_error {
    Bad_from_string(std::string s) : std::runtime_error(s) {}
};

// Derived class for handling string find errors.
struct String_find_error : std::runtime_error {
    String_find_error(std::string s) : runtime_error(s) {}
};

// Simple convert to string method.
template <typename T>
std::string to_string(const T& t)
{
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

// Simple extract from string method.
template <typename T>
T from_string(const std::string& s)
{
    std::istringstream iss(s);
    T t;
    if (!(iss >> t)) {
        throw Bad_from_string("bad cast from string '" + s + "'");
    }
    return t;
}

// Convert Fortran scientific D format to double.
inline double from_fortran_sci_fmt(const std::string& s)
{
    std::string ss             = s;
    std::string::size_type pos = ss.find('D');
    if (pos != std::string::npos) {
        ss.replace(pos, 1, 1, 'e');
    }
    return from_string<double>(ss);
}

// Trim leading and trailing characters from string.
inline std::string trim(const std::string& str, const char* sep)
{
    const std::string::size_type pos = str.find_first_not_of(sep);
    return (pos == std::string::npos)
               ? std::string()
               : str.substr(pos, str.find_last_not_of(sep) - pos + 1);
}

// Strip suffix from filename.
inline std::string strip_suffix(const std::string& filename,
                                const std::string& suffix)
{
    std::string basename       = filename;
    std::string::size_type pos = basename.rfind(suffix);
    if (pos == std::string::npos) {
        throw String_find_error(filename + " does not contain " + suffix);
    }
    return basename.erase(pos, basename.size() - pos);
}

// Get suffix from filename.
inline std::string get_suffix(const std::string& filename)
{
    std::string suffix         = filename;
    std::string::size_type pos = suffix.rfind(".");
    if (pos == std::string::npos) {
        throw String_find_error(filename + " does not have a suffix");
    }
    return suffix.erase(0, pos);
}

// Check if string has only blank characters.
inline bool str_has_only_blanks(const std::string& str)
{
    if (!str.empty()) {
        if (str.find_first_not_of(" \t") != std::string::npos) {
            return false;
        }
    }
    return true;
}

// Case-insensitive string comparison.
//
// Note: Can only handle characters in the default C locale!
inline bool stricmp(const std::string& str1, const std::string& str2)
{
    std::string s1;
    std::string s2;
    std::string::const_iterator it;
    for (it = str1.begin(); it < str1.end(); ++it) {
        if (::isalpha(*it)) {  // only convert alphabetic characters
            s1 += static_cast<char>(::tolower(*it));
        }
        else {
            s1 += *it;
        }
    }
    for (it = str2.begin(); it < str2.end(); ++it) {
        if (::isalpha(*it)) {
            s2 += static_cast<char>(::tolower(*it));
        }
        else {
            s2 += *it;
        }
    }
    return s1 == s2;
}

}  // namespace srs

#endif  // SRS_STRING_H
