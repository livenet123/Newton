
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

```
sudo apt-get install build-essential libboost-all-dev git cmake
git clone https://github.com/livenet123/Newton.git
export CXXFLAGS="-std=gnu++11"
cd Newton
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make
```

The resulting executables can be found in build/release/src.


### On Windows:
Dependencies: MSVC 2013 or later, CMake 2.8.6 or later, and Boost 1.55. You may download them from:
* http://www.microsoft.com/
* http://www.cmake.org/
* http://www.boost.org/

To build, change to a directory where this file is located, and run this commands:

```
md build && cd build
cmake -G "Visual Studio 12 Win64" .. -DCMAKE_BUILD_TYPE=Release
MSBuild Newton.sln /p:Configuration=Release /m
```
OR you can open Newton Solution in Visual Studio and then do Build.

Good luck!
