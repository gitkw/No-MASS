# No-MASS
## Current Status Of Build
![Build](https://magnum.travis-ci.com/jacoblchapman/No-MASS.svg?token=hNH6EHukhSBPUpNQNYH3&branch=Master "build")

## Guide

This guide is for **linux only**.
Use a compiler with at least the c++11 standard. Most modern linux systems have this by default, if not it can be installed easily.
CMake is used to build the make file.

No-MASS can be built on windows using mingw64. This is more complicated and differs in each windows version.

### Checkout

Clone the repository using git
```sh
git clone https://github.com/jacoblchapman/No-MASS.git

cd No-MASS

git submodule update --init --recursive

```

Alternativly use a GUI such as GitHub Desktop or SourceTree.

### Compiling

Build the boost libraries using the make script

```sh
cd Boost

make

cd ..
```

Build the No-MASS using make (or using CodeBlocks GUI)

```sh
cd FMU

cmake ./

make
```

### Running

Take the compiled library (FMI.so or FML.dll) and place into the FMU zip file with the model description file and the SimulationConfig file.

Place the file in the EnergyPlus folder, define the variables in the IDF and run EnergyPlus.


### Testing

To run the tests enable testing in CMake, build, and run the test program

```sh
cd FMU

cmake -DCMAKE_BUILD_TYPE=Debug -Dtest=on ./

make

./runUnitTests
```
