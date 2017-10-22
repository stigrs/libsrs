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

#ifndef SRS_BAND_MATRIX_H
#define SRS_BAND_MATRIX_H

#include <srs/types.h>
#include <array>
#include <vector>


namespace srs {

template <class T>
class Band_matrix {
public:
    typedef T value_type;
    typedef Int_t size_type;
    typedef std::vector<T>::iterator iterator;
    typedef std::vector<T>::const_iterator const_iterator;

    Band_matrix(), elems(0), extents{0, 0}, bwidth{0, 0} {}

    Band_matrix(size_type m, size_type n, size_type kl, size_type ku)
        : elems((kl + ku + 1) * n), extents{m, n}, bwidth{kl, ku}
    {
    }

    Band_matrix(
        size_type m, size_type n, size_type kl, size_type ku, const T& value)
        : elems((kl + ku + 1) * n, value), extents{m, n}, bwidth{kl, ku}
    {
    }

private:
    std::vector<T> elems;
    std::array<size_type, 2> extents;
    std::array<size_type, 2> bwidth;
};

}  // namespace srs

#endif  // SRS_BAND_MATRIX_H
