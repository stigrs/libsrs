# LIBSRS

LIBSRS extents the C++ STL with N-dimensional dense arrays, packed matrix 
storage, sparse vectors and matrices, mathematical functions and other 
utility methods. The array, vector, and matrix classes are intended to
facilitate easy integration with Intel MKL for fast numerical performance.

## Code of Conduct

This project has adopted the [Covenant Code of Conduct](CODE_OF_CONDUCT.md).

## Licensing

LIBSRS is released under the [MIT](LICENSE) license.

## Usage of Third Party Libraries

This project makes use of the [Catch](https://https://github.com/philsquared/catch) 
testing library. Please see the [ThirdPartyNotices.txt](ThirdPartyNotices.txt) 
file for details regarding the licensing of Catch.

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
* GNU/Linux using GCC 5.4.0
* GNU/Linux using Clang 3.8.0
* OS X El Capitan (10.12) using Apple LLVM 8.1.0
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
