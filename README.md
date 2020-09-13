x2boost
=======

_Under development now_

x2boost is a C++ port of [x2](https://github.com/jaykang920/x2) based on C++98
and [Boost](http://www.boost.org/) libraries.

Requirements
------------

* [CMake](https://cmake.org/) 2.8.8 or newer (and C++ compiler, of course) to build
* Boost C++ libraries (1.53 or newer) with the following modules built
  * atomic
  * chrono
  * filesystem
  * program_options
  * regex
  * thread
  * unit_test_framework (for test)

Installation
------------

### Boost

x2boost depends on a few Boost libraries that require compilation, and you
should have those libraries built on your machine.

https://theboostcpplibraries.com/introduction-installation

#### Windows

Follows an example case building Boost on Windows:

* Open your Visual Studio developer command prompt.
* Run *bootstrap.bat* to prepare Boost build system.
* Determine your toolset version and run the following commands.
  * Visual Studio 11 2012: msvc-11.0
  * Visual Studio 12 2013: msvc-12.0
  * Visual Studio 14 2015: msvc-14.0
  * Visual Studio 15 2017: msvc-14.1
  * Visual Studio 16 2019: msvs-14.2

```
rem your toolset spec here
set toolset=msvc-12.0

.\b2 --build-type=complete --stagedir=stage/x86 toolset=%toolset% address-model=32 link=static threading=multi runtime-link=static stage
.\b2 --build-type=complete --stagedir=stage/x64 toolset=%toolset% address-model=64 link=static threading=multi runtime-link=static stage
.\b2 --build-type=complete --stagedir=stage/x86 toolset=%toolset% address-model=32 link=static threading=multi runtime-link=shared stage
.\b2 --build-type=complete --stagedir=stage/x64 toolset=%toolset% address-model=64 link=static threading=multi runtime-link=shared stage
.\b2 --build-type=complete --stagedir=stage/x86 toolset=%toolset% address-model=32 link=shared threading=multi runtime-link=static stage
.\b2 --build-type=complete --stagedir=stage/x64 toolset=%toolset% address-model=64 link=shared threading=multi runtime-link=static stage
.\b2 --build-type=complete --stagedir=stage/x86 toolset=%toolset% address-model=32 link=shared threading=multi runtime-link=shared stage
.\b2 --build-type=complete --stagedir=stage/x64 toolset=%toolset% address-model=64 link=shared threading=multi runtime-link=shared stage

```

#### Unix Variants

You might find a binary distribution of Boost libraries for your platform, in
which case you don't have to get into any trouble trying to build them.

### CMake

* Get to the [build](https://github.com/jaykang920/x2boost/tree/master/build)
subdirectory.
* Create a new subdirectory that will hold your CMake output files. You probably
want to give it a meaningful name.
* Get into the new subdirectory just created, and run *cmake* command.
```
cmake ../..
```
You can specify additional options to tune your build. For example, if you're
using Visual Studio 2013 on Windows and want 64-bit build, use the following
generator specification. For more CMake option details, please look up the cmake
command help.
```
cmake -G "Visual Studio 12 2013 Win64" ..\..
```
x2boost provides several custom option flags that can be configured through CMake command-line or GUI.
 * BUILD_SHARED_LIBS (OFF by default) : Build x2boost as a shared library (DLL on Windows)
 * BUILD_FORCE_SHARED_CRT (OFF by default) : Target shared (DLL on Windows) version of C runtime library
 * BUILD_XPILER (ON by default) : Build xpiler executable
 * BUILD_TEST (OFF by default) : Build unit tests
 * BUILD_SAMPLES (OFF by default) : Build samples

On command-line, you can set an option flag as follows:
```
cmake -D BUILD_TEST=ON ../..
```

#### Build Scripts

The *build* subdirectory contains some sample scripts that run CMake to generate
build environments. You also may create your own build scripts here.

##### Windows

Unlike unix variant platforms, Windows doesn't have common library installation
locations. So you need to pass CMake hints to find Boost libraries, either as
environment variables or as cmake option definitions.

About configuring environment variables to help find Boost, please take a look at
[build/boostvars.bat](https://github.com/jaykang920/x2boost/blob/master/build/boostvars.bat)
.

##### Unix Variants

When you generate Unix Makefiles using CMake, you may want to specify the build
type option as follows:
```
cmake -D CMAKE_BUILD_TYPE=Release ../..
```

License
-------

x2boost is distributed under [MIT License](http://opensource.org/licenses/MIT).
See the file LICENSE for details.
