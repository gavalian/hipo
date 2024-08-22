# HipoDataFrame

The HipoDataFrame is a library with a class derived from the ROOT7 
RDataSource class to provide an adaptor for RDataFrame to read HIPO 
files. An attempt is made to make this somewhat efficient.

## Build Directions.

### Requirements
To build the HipoDataFrame library, you will need hipo4 (the parent of
this project) and you will need ***A modern version of ROOT***, 
version 6.26 or later is recommended. This version of ROOT must be
compiled with a newer compiler capable of C++17. (recommended gcc 9 or newer, 
llvm 12 or newer ). The version of ROOT must be compiled with the
flag -DCMAKE_CXX_STANDARD=17, so that it has the RDataFrame library.

### Recommended Way
It is recommended that you build the libHipoDataFrame.so 
(.dylib on Mac) library together with the HIPO library using Meson.
See the [main README](/README.md) for guidance.
This will install the hipo4 and HipoDataFrame libraries in
`/path/to/install/location/lib`

You can now load the library into root with:

```bash
source <location of ROOT>/bin/thisroot.sh
export LD_LIBRARY_PATH=/path/to/install/location/lib:${LD_LIBRARY_PATH}
root 
root [0] .L libHipoDataFrame.so
```
(For Mac, replace LD_LIBRARY_PATH with DYLD_LIBRARY_PATH
and the .so with .dylib)

***NOTE***: On a system where the default compiler is NOT c++17 compatible, i.e. Linux at JLab,
you have to specify the compiler to cmake. You can do so with the following lines:
```bash
module use /apps/modulefiles
module load gcc/9.3.0
module load cmake/3.22.1
source ~holtrop/root/bin/thisroot.sh
cmake .. -DCMAKE_INSTALL_PREFIX=${HOME} -DCMAKE_CXX_COMPILER=$(which g++) -DCMAKE_C_COMPILER=$(which gcc)
```

However, unfortunately, ***none*** of the installed versions of ROOT (in /apps, or by CLAS12) are 
currently C++17 compatible, so use the ROOT version installed in `~holtrop/root` or your own.

## Using the DataFrame

[See the ROOT documentation for RDataFrame](https://root.cern/doc/master/classROOT_1_1RDataFrame.html)

***Note***: The names of the Hipo schema are mangled to avoid :: and . (period) in the name, since
these indicate C++ namespace and class member. So instead of :: and . we use underscores (_).

The basics:

```bash
root 
root [0] .L libHipoDataFrame.so
root [1] auto df = MakeHipoDataFrame("rec_clas_016214.evio.00000.hipo")
root [2] df.GetColumnNames() // Shows what is in the file.
root [3] auto h_px = df.Histo1D("REC_Particle_px")
root [4] h_px->DrawClone();
```
The result would be a histogram of the REC::Particle.px, i.e the x component of the particle momentum
for all particles in the events.

The ROOT RDataFrame is very versatile and is intended by the ROOT team as the future of 
data analysis. [See more details on the ROOT website](https://root.cern/doc/master/classROOT_1_1RDataFrame.html). 
