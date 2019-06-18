# hipo
High Performance Output (HIPO) is a data format for experimental Physics.
It uses chunked data blocks to store data with LZ4 compression. Provides highly 
efficient read speeds.

## Clone the package

Package has a dependency on LZ4 compression library, which is a submodule.
Use command:

 git clone --recurse-submodules https://github.com/gavalian/hipo

to clone the distribution.


## Package Structure

hipo4 directory contains sources and include files for version 4 of hipo.
Several examples are included (in the exmaples folder) for writing a simple
file with declared dictionary and a reader to read and display information
in the banks for each event.


