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

#include <srs/utils.h>
#include <srs/utils_impl/input.h>


namespace srs {

std::istream& operator>>(std::istream& from, Input& inp)
{
    switch (inp.type) {
    case Input::t_int:
        inp.read_int(from);                                                    
        break;
    case Input::t_long:
        inp.read_long(from);
        break;
    case Input::t_uint:
        inp.read_uint(from);
        break;
    case Input::t_ulint:                                                        
        inp.read_ulint(from);
        break;
    case Input::t_double:
        inp.read_double(from);
        break;
    case Input::t_string:
        inp.read_string(from);
        break;
    case Input::t_ivector:
        inp.read_ivector(from);
        break;
    case Input::t_uvector:
        inp.read_uvector(from);
        break;
    case Input::t_dvector:
        inp.read_dvector(from);
        break;
    case Input::t_noval:
        throw Input_invalid("cannot read data with no type");
    default:
        throw Input_invalid("cannot read data with unknown type");
    }
    inp.state = Input::init;
    return from;
}

std::ostream& operator<<(std::ostream& to, const Input& inp)
{
    if (inp.state == Input::not_init) {
        to << "not initialized";
    }
    else {
        srs::ivector iv;
        srs::uvector uv;
        srs::dvector dv;

        switch (inp.type) {
        case Input::t_int:
            to << *static_cast<int*>(inp.data);
            break;
        case Input::t_long:
            to << *static_cast<long*>(inp.data);
            break;
        case Input::t_uint:
            to << *static_cast<unsigned*>(inp.data);
            break;
        case Input::t_ulint:
            to << *static_cast<unsigned long*>(inp.data);
            break;
        case Input::t_double:
            to << *static_cast<double*>(inp.data);
            break;
        case Input::t_string:
            to << *static_cast<std::string*>(inp.data);
            break;
        case Input::t_ivector:
            iv = *static_cast<srs::ivector*>(inp.data);
            to << iv;
            break;
        case Input::t_uvector:
            uv = *static_cast<srs::uvector*>(inp.data);
            to << uv;
            break;
        case Input::t_dvector:
            dv = *static_cast<srs::dvector*>(inp.data);
            to << dv;
            break;
        default:
            throw Input_invalid("unknown type; cannot write data");
        }
    }
    return to;
}

void Input::read_int(std::istream& from)
{
    int& i = *static_cast<int*>(data);

    std::string buf;
    from >> buf;

    i = srs::from_string<int>(buf);
}

void Input::read_long(std::istream& from)
{
    long& i = *static_cast<long*>(data);

    std::string buf;
    from >> buf;

    i = srs::from_string<long>(buf);
}

void Input::read_uint(std::istream& from)
{
    unsigned& i = *static_cast<unsigned*>(data);

    std::string buf;
    from >> buf;

    i = srs::from_string<unsigned>(buf);
}

void Input::read_ulint(std::istream& from)
{
    unsigned long& i = *static_cast<unsigned long*>(data);

    std::string buf;
    from >> buf;

    i = srs::from_string<unsigned long>(buf);
}

void Input::read_double(std::istream& from)
{
    double& d = *static_cast<double*>(data);

    std::string buf;
    from >> buf;

    d = srs::from_string<double>(buf);
}

void Input::read_string(std::istream& from)
{
    std::string& s = *static_cast<std::string*>(data);

    std::string buf;
    from >> buf;

    s = srs::from_string<std::string>(buf);
}

void Input::read_ivector(std::istream& from)
{
    srs::ivector& v = *static_cast<srs::ivector*>(data);
    from >> v;
}

void Input::read_uvector(std::istream& from)
{
    srs::uvector& v = *static_cast<srs::uvector*>(data);
    from >> v;
}

void Input::read_dvector(std::istream& from)
{
    srs::dvector& v = *static_cast<srs::dvector*>(data);
    from >> v;
}

}  // namespace srs
