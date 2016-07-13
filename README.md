# No-MASS
## Current Status Of Build
![Build](https://magnum.travis-ci.com/jacoblchapman/No-MASS.svg?token=hNH6EHukhSBPUpNQNYH3&branch=Master "build")

## Guide Linux

Use a compiler with at least the c++11 standard. Most modern linux systems have this by default, if not it can be installed easily.
CMake is used to build the make file.

### Checkout

Clone the repository using git, ensure you do a recursive clone to recieve all dependencies.
Needs rapidxml for xml parsing and googletest for testing.

```sh
git clone https://github.com/jacoblchapman/No-MASS.git

cd No-MASS

git submodule update --init --recursive

```

Alternativly use a GUI such as GitHub Desktop or SourceTree.

### Compiling

Build the No-MASS using cmake and make

```sh
cd FMU

mkdir build

cd build

cmake ../

make
```

### Running

Take the compiled library (FMI.so or FML.dll) and place into the FMU zip file with the model description file and the SimulationConfig file.

Place the file in the EnergyPlus folder, define the variables in the IDF and run EnergyPlus.


### Testing

To run the tests enable testing in CMake, build, and run the test program

```sh
cd FMU/build

cmake -DCMAKE_BUILD_TYPE=Debug -Dtest=on ../

make

./runUnitTests
```


## Guide Windows

No-MASS can be built on windows using mingw-w64 or alternativly visual studio. This differs for each windows version, but the general process is:
On windows install CMake https://cmake.org/download/
mingw-w64 installer if not using visual studio http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/
Use the CMake GUI to create the visual studio project or make files from the FMU directory in the checked out source folder.
Open in visual studio to compile or use make command as in linux.
