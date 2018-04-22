
[![Build Status](https://travis-ci.org/livenet123/Newton.svg?branch=master)](https://travis-ci.org/livenet123/Newton)
[![Build status](https://ci.appveyor.com/api/projects/status/7dg9qrptema5q9ar/branch/master?svg=true)](https://ci.appveyor.com/project/livenet123/newton-awyxq/branch/master)


## Building Newton

### On *nix:

Dependencies: GCC 4.9 or later, CMake 2.8.6 or later, and Boost 1.55.
You may download them from:

* http://gcc.gnu.org/
* http://www.cmake.org/
* http://www.boost.org/

Alternatively, it may be possible to install them using a package manager.

#### Run these commands

```
git clone https://github.com/livenet123/Newton.git
export CXXFLAGS="-std=gnu++11"
cd Newton
chmod +x external/rocksdb/build_tools/build_detect_platform
chmod +x external/rocksdb/build_tools/version.sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC=true .. && make
```

The resulting executables can be found in build/release/src.

#### Advanced options:
Parallel build: instead of `make' run

```
make -j<number of threads>
```
#### Debug build: run
```
make build-debug
```
#### Test suite: run
```
make test-release
```
to run tests in addition to building. Running `make test-debug' will do the same to the debug version.

#### Building with Clang:
it may be possible to use Clang instead of GCC, but this may not work everywhere. To build, before running `make' run
```
export CC=clang CXX=clang++
```

### On Windows:
Dependencies: MSVC 2013 or later, CMake 2.8.6 or later, and Boost 1.55. You may download them from:
* http://www.microsoft.com/
* http://www.cmake.org/
* http://www.boost.org/

To build, change to a directory where this file is located, and run this commands:

```
mkdir build
cd build
cmake -G "Visual Studio 12 Win64" ..
```
And then do Build.

Good luck!
