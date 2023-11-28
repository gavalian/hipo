# hipo

High Performance Output (HIPO) is a data format for experimental Physics.
It uses chunked data blocks to store data with LZ4 compression. Provides highly 
efficient read speeds. Originally implemented in Java for physics reconstruction
software using service oriented architecture (SOA), now has C++ implementaion 
with wrappers to interface FORTRAN and PTYHON. The wrapper does not provide full 
functionality that exists in C++ and Java libraries.


## Installing the Package

Package has a dependency on LZ4 compression library, which is a submodule.
Use command:

```
 git clone --recurse-submodules git@github.com:gavalian/hipo.git
```

to clone the distribution. Then compile using 'make', or 'cmake', and start using.

## Usage

The package contains example with code to write and read hipo files. Here are
a few quick examples:

read two banks from a file, and print the content on the screen:

```c++
#include "reader.h"
int main(){
   std::string file = "mydatafile.hipo";
   hipo::reader   r(file);
   hipo::banklist list = r.getBanks({"REC::Particle","REC::Event"});
   int counter = 0;
   while( r.next(list)&&counter<350){ counter++; list[0].show(); list[1].show();}
}
```

Looking into large amount of data frequently require data histogramming for
numerical data quantitive representation. The hipo package includes a simple
(to be improved and extended) histogramming package and a very simple
visualization tool (ascii only) for debugging and exploratory purposes.
Here is an example code to create a histogram and fill it with data from
a bank and display it on the excreen.

```c++
#include "reader.h"
#include "twig.h"
int main(){
   std::string file = "mydatafile.hipo";
   twig::h1d hz(120,-15,5);
   hipo::reader   r(file);
   hipo::banklist list = r.getBanks({"REC::Particle"});
   while( r.next(list)){
      for(int r = 0; r < list[0].getRows(); r++){
         if(list[0].getInt("charge",r)!=0) hz.fill(list[0].getFloat("vz",r));
      }
   }
   hz.print();
}
```

Output:

<img src="https://github.com/gavalian/hipo/blob/master/documents/screenshots/histogram_vz.png" width="800">


## Package Structure

hipo4 directory contains sources and include files for version 4 of hipo.
Several examples are included (in the exmaples folder) for writing a simple
file with declared dictionary and a reader to read and display information
in the banks for each event.

## Python Interface

In modern evolving world of Artificial Intelligence there is a need for reading
HIPO files from Python. There is a python interface included in the package
(in directory python) implemented using ctypes that provides direct access to
the hipo4 shared library. No external dependencies and no compilation required.
Just provide the directory location where the lz4 and hipo4 shared libraries can
be found and use it.
For this example to work, compile the main directory producing the shared libraries.
The go to examples directory and compile, then run the writeFile.exe code, which
produces a file with a name "example_output.hipo". Here is an eaxmple of how to
read a file produced by the example code writeFile.cpp (in examples directory):


``` python
 # example of reading hipo file from python
 from hipolib import hreader

 filename = '../../examples/example_output.hipo'
 # the directory with shared libraries provided
 reader = hreader('../../slib')
 reader.open(filename)
 # define banks that will be read with each next() call
 reader.define('event::particle')
 counter = 0

 while(reader.next()==True):
     size = reader.getSize('event::particle')
     print('bank size = ',size)
     # get each column as an array from the bank
     array_px  = reader.getEntry('event::particle','px')
     array_pid = reader.getEntry('event::particle','pid')
     print('pid = ',array_pid)
     print('px  = ',array_px)    
     counter = counter + 1

 print('processed event # ', counter)
```

The output will look something lie this:

``` bash
bank size =  2
pid =  [7, 5]
px  =  [0.10791015625, 0.8607100248336792]
bank size =  6
pid =  [11, 12, 1, 12, 12, 8]
px  =  [0.73046875, 0.7046864032745361, 0.48323971033096313, 0.24342545866966248, 0.4932733476161957, 0.2452908456325531]
...
...
bank size =  11
pid =  [7, 6, 17, 7, 14, 2, 13, 16, 16, 8, 15]
px  =  [0.203125, 0.24804462492465973, 0.7553998827934265, 0.9020906686782837, 0.6647433638572693, 0.14718711376190186, 0.3066171407699585, 0.12799464166164398, 0.1626734435558319, 0.13374938070774078, 0.7719628810882568]
bank size =  8
pid =  [3, 11, 5, 0, 17, 18, 2, 5]
px  =  [0.10888671875, 0.3256213665008545, 0.2945103943347931, 0.956960141658783, 0.7667134404182434, 0.38611966371536255, 0.4307321012020111, 0.9494407773017883]
bank size =  9
pid =  [12, 0, 9, 2, 8, 15, 19, 19, 12]
px  =  [0.58984375, 0.344686359167099, 0.8593308329582214, 0.2144325226545334, 0.5664244294166565, 0.3212900757789612, 0.3333880305290222, 0.7160753011703491, 0.8818097114562988]
```


## Tagging Distribution

The command used to tag the distribution are:

```
git tag <tagname> -a -m"this tag is the best"
git push origin --tags
```
