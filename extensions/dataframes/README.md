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
(.dylib on Mac) library together with the HIPO library using "cmake".

Here are the steps:
```bash
git clone https://github.com/gavalian/hipo.git
cd hipo
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/location ..
make -j8 install
```

This will install the hipo4 library (both .a and .so) in 
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

## Using the DataFrame

[See the ROOT documentation for RDataFrame](https://root.cern/doc/master/classROOT_1_1RDataFrame.html)

The basics:

```bash
root 
root [0] .L libHipoDataFrame.so
root [1] auto df = MakeHipoDataFrame("rec_clas_016214.evio.00000.hipo")
root [2] df.GetColumnNames() // Shows what is in the file.
root [3] auto h_px = df.Histo1D("REC::Particle.px")
root [4] h_px->DrawClone();
```
