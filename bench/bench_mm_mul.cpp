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

#include <srs/array.h>
#include <srs/math.h>
#include <armadillo>
#include <chrono>
#include <iostream>


using Timer = std::chrono::duration<double, std::milli>;

void print(int n,
           int m,
           const Timer& t_arma,
           const Timer& t_mm_mul,
           const Timer& t_dgemm)
{
    std::cout << "Matrix-matrix multiplication:\n"
              << "-----------------------------\n"
              << "size =        " << n << " x " << m << '\n'
              << "mm_mul/arma = " << t_mm_mul.count() / t_arma.count() << "\n"
              << "dgemm/arma =  " << t_dgemm.count() / t_arma.count() << "\n\n";
}

void benchmark(int n, int m)
{
    arma::mat a1 = arma::ones<arma::mat>(n, m);
    arma::mat a2 = arma::ones<arma::mat>(m, n);
    auto t1      = std::chrono::high_resolution_clock::now();
    arma::mat a3 = a1 * a2;
    auto t2      = std::chrono::high_resolution_clock::now();
    Timer t_arma = t2 - t1;

    srs::dmatrix b1(n, m, 1.0);
    srs::dmatrix b2(m, n, 1.0);
    t1             = std::chrono::high_resolution_clock::now();
    auto b3        = b1 * b2;
    t2             = std::chrono::high_resolution_clock::now();
    Timer t_mm_mul = t2 - t1;

    srs::dmatrix b4;
    t1 = std::chrono::high_resolution_clock::now();
    srs::mkl_dgemm("N", "N", 1.0, b1, b2, 0.0, b4);
    t2            = std::chrono::high_resolution_clock::now();
    Timer t_dgemm = t2 - t1;

    print(n, m, t_arma, t_mm_mul, t_dgemm);
}

int main()
{
    int n = 10;
    int m = 5;
    benchmark(n, m);

    n = 100;
    m = 50;
    benchmark(n, m);

    n = 1000;
    m = 500;
    benchmark(n, m);
}
