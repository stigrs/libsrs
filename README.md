# LIBSRS

LIBSRS extents the C++ STL with N-dimensional dense arrays, packed matrix 
storage, band matrix storage, sparse vectors and matrices, mathematical 
functions and other utility methods. 

The design criteria for the array, vector, and matrix classes have been 
numerical correctness, ease of implementation, flexibility, ease of use, 
and easy integration with the Intel MKL. No efforts have been attempted to 
avoid the creation of temporaries during arithmetic operations since this
significantly adds to the code complexity. If fast numerical performance is 
needed, the performance-critical parts of the code, identified through 
profiling, code should be replaced with Intel MKL functions. 

## Code of Conduct

This project has adopted the [Covenant Code of Conduct](CODE_OF_CONDUCT.md).

## Licensing

LIBSRS is released under the [MIT](LICENSE) license.

## Usage of Third Party Libraries

This project makes use of the [Catch2](https://github.com/catchorg/Catch2) 
testing library. Please see the [ThirdPartyNotices.txt](ThirdPartyNotices.txt) 
file for details regarding the licensing of Catch2.

## Quick Start 

### Requirements

* [CMake](https://cmake.org) 3.4.3
* [Boost](http://www.boost.org/) 1.65.0
* [GSL](https://github.com/Microsoft/GSL)
* [Intel MKL](https://software.intel.com/en-us/mkl) 2018 Initial Release
* [Armadillo](http://arma.sourceforge.net) 7.950.1 (for testing and benchmarks)

### Supported Platforms

The test suite that exercises LIBSRS has been built and passes successfully 
on the following platforms:
* GNU/Linux using GCC 5.4.0, 7.3.0
* GNU/Linux using Clang 3.8.0
* GNU/Linux using Intel C++ 18.0.1 20171018, Intel C++ 18.0.3 20180410
* OS X El Capitan (10.12) using Apple LLVM 8.1.0
* OS X High Sierra (10.13) using Apple LLVM 9.1.0, GCC 7.2.0
* Windows using Visual Studio 2017

### Obtaining the Source Code

The source code can be obtained from

        git clone git@github.com:stigrs/libsrs.git

### Building the Software

These steps assumes that the source code of this repository has been cloned
into a directory called `libsrs`.

1. Create a directory to contain the build outputs:

        cd libsrs
        mkdir build
        cd build

2. Configure CMake to use the compiler of your choice (you can see a list by
   running `cmake --help`):

        cmake -G "Visual Studio 15 2017" ..

3. Build the software (in this case in the Release configuration):

        cmake --build . --config Release

4. Run the test suite:

        ctest -C Release

5. Install the software:

        cmake --build . --config Release --target install

All tests should pass, indicating that your platform is fully supported. 
