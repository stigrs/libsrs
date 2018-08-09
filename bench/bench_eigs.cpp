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
           const Timer& t_arma,
           const Timer& t_eigs,
           const Timer& t_jacobi)
{
    std::cout << "Eigenvalues for symmetric matrix:\n"
              << "---------------------------------\n"
              << "size =        " << n << " x " << n << '\n'
              << "eigs/arma =   " << t_eigs.count() / t_arma.count() << "\n"
              << "jacobi/arma = " << t_jacobi.count() / t_arma.count()
              << "\n\n";
}

void benchmark(int n)
{
    arma::mat a1 = arma::randu<arma::mat>(n, n);
    arma::mat a2 = a1.t() * a1;
    arma::mat eigvec(n, n);
    arma::vec eigval(n);
    auto t1 = std::chrono::high_resolution_clock::now();
    arma::eig_sym(eigval, eigvec, a2);
    auto t2      = std::chrono::high_resolution_clock::now();
    Timer t_arma = t2 - t1;

    srs::dmatrix b1 = srs::randu(n, n);
    srs::dmatrix b2 = srs::transpose(b1) * b1;
    srs::dvector wr(n);
    t1 = std::chrono::high_resolution_clock::now();
    srs::eigs(b2, wr);
    t2           = std::chrono::high_resolution_clock::now();
    Timer t_eigs = t2 - t1;

    srs::dmatrix c1 = srs::randu(n, n);
    srs::dmatrix c2 = srs::transpose(c1) * c1;
    srs::dvector vr(n);
    t1 = std::chrono::high_resolution_clock::now();
    srs::jacobi(c2, vr);
    t2             = std::chrono::high_resolution_clock::now();
    Timer t_jacobi = t2 - t1;

    print(n, t_arma, t_eigs, t_jacobi);
}

int main()
{
    int n = 10;
    benchmark(n);

    n = 100;
    benchmark(n);

    n = 500;
    benchmark(n);
}
