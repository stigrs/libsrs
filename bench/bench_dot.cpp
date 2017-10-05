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


typedef std::chrono::duration<double, std::micro> Timer;

void print(int n, const Timer& t_arma, const Timer& t_srs, const Timer& t_ddot)
{
    std::cout << "Dot product:\n"
              << "------------\n"
              << "size =      " << n << '\n'
              << "srs/arma =  " << t_srs.count() / t_arma.count() << "\n"
              << "ddot/arma = " << t_ddot.count() / t_arma.count() << "\n\n";
}

void benchmark(int n)
{
    arma::vec aa(n);
    arma::vec ab(n);
    aa.fill(1.0);
    ab.fill(2.0);
    auto t1      = std::chrono::high_resolution_clock::now();
    double arma  = arma::dot(aa, ab);
    auto t2      = std::chrono::high_resolution_clock::now();
    Timer t_arma = t2 - t1;

    srs::Array<double, 1> va(n, 1.0);
    srs::Array<double, 1> vb(n, 2.0);
    t1          = std::chrono::high_resolution_clock::now();
    auto srs    = srs::dot(va, vb);
    t2          = std::chrono::high_resolution_clock::now();
    Timer t_srs = t2 - t1;

    t1 = std::chrono::high_resolution_clock::now();
    srs::mkl_ddot(va, vb);
    t2           = std::chrono::high_resolution_clock::now();
    Timer t_ddot = t2 - t1;

    print(n, t_arma, t_srs, t_ddot);
    if (arma != srs) {
        std::cout << "Different\n";
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
