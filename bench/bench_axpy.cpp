//////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////

#include <srs/array.h>
#include <srs/math.h>
#include <armadillo>
#include <chrono>
#include <iostream>
#include <valarray>


typedef std::chrono::duration<double, std::milli> Timer;

void print(int n,
           const Timer& t_arma,
           const Timer& t_srs,
           const Timer& t_val,
           const Timer& t_axpy)
{
    std::cout << "Vector addition:\n"
              << "----------------\n"
              << "size =      " << n << '\n'
              << "srs/arma =  " << t_srs.count() / t_arma.count() << "\n"
              << "srs/val =   " << t_srs.count() / t_val.count() << "\n"
              << "axpy/arma = " << t_axpy.count() / t_arma.count() << "\n"
              << "axpy/val =  " << t_axpy.count() / t_val.count() << "\n\n";
}

void benchmark(int n)
{
    arma::vec aa(n);
    arma::vec ab(n);
    aa.fill(1.0);
    ab.fill(1.0);
    auto t1      = std::chrono::high_resolution_clock::now();
    ab           = 2.0 * aa + ab;
    auto t2      = std::chrono::high_resolution_clock::now();
    Timer t_arma = t2 - t1;

    srs::Array<double, 1> va(n, 1.0);
    srs::Array<double, 1> vb(n, 1.0);
    t1          = std::chrono::high_resolution_clock::now();
    vb          = 2.0 * va + vb;
    t2          = std::chrono::high_resolution_clock::now();
    Timer t_srs = t2 - t1;

    srs::Array<double, 1> ta(n, 1.0);
    srs::Array<double, 1> tb(n, 1.0);
    t1 = std::chrono::high_resolution_clock::now();
    srs::axpy<double>(2.0, ta, tb);
    t2           = std::chrono::high_resolution_clock::now();
    Timer t_axpy = t2 - t1;

    std::valarray<double> wa(1.0, n);
    std::valarray<double> wb(1.0, n);
    t1          = std::chrono::high_resolution_clock::now();
    wb          = 2.0 * wa + wb;
    t2          = std::chrono::high_resolution_clock::now();
    Timer t_val = t2 - t1;

    print(n, t_arma, t_srs, t_val, t_axpy);

    for (int i = 0; i < n; ++i) {
        if (ab(i) != vb(i)) {
            std::cout << "Different\n";
        }
    }
}

int main()
{
    int n = 10;
    benchmark(n);

    n = 100;
    benchmark(n);

    n = 1000;
    benchmark(n);

    n = 10000;
    benchmark(n);

    n = 100000;
    benchmark(n);
}
