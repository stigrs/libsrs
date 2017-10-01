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
#include <armadillo>
#include <chrono>
#include <iostream>


typedef std::chrono::duration<double, std::milli> Timer;

void print(
    int n, int m, const Timer& t_arma, const Timer& t_in, const Timer& t_out)
{
    std::cout << "Matrix transpose:\n"
              << "-----------------\n"
              << "size =     " << n << " x " << m << '\n'
              << "t(in) =    " << t_in.count() << '\n'
              << "t(out) =   " << t_out.count() << '\n'
              << "t(arma) =  " << t_arma.count() << '\n'
              << "in/arma =  " << t_in.count() / t_arma.count() << '\n'
              << "out/arma = " << t_out.count() / t_arma.count() << "\n\n";
}

void benchmark(int n, int m)
{
    arma::mat m1 = arma::ones<arma::mat>(n, m);
    auto t1      = std::chrono::high_resolution_clock::now();
    m1.t();
    auto t2      = std::chrono::high_resolution_clock::now();
    Timer t_arma = t2 - t1;

    srs::dmatrix m2(n, m, 1.0);
    t1 = std::chrono::high_resolution_clock::now();
    m2.transpose();
    t2         = std::chrono::high_resolution_clock::now();
    Timer t_in = t2 - t1;

    srs::dmatrix m3(n, m, 1.0);
    t1          = std::chrono::high_resolution_clock::now();
    m3          = transpose(m3);
    t2          = std::chrono::high_resolution_clock::now();
    Timer t_out = t2 - t1;

    print(n, m, t_arma, t_in, t_out);
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
