# hipo

High Performance Output (HIPO) is a data format for experimental Physics.
It uses chunked data blocks to store data with LZ4 compression. Provides highly 
efficient read speeds. Originally implemented in Java for physics reconstruction
software using service oriented architecture (SOA), now has C++ implementaion 
with wrappers to interface FORTRAN, JULIA and PTYHON. The wrapper does not provide full 
functionality that exists in C++ and Java libraries.

## Write File Example (Java)

Example writes a simple file with each event containing one bank.

``` java
  SchemaBuilder b = new SchemaBuilder("data::clusters",12,1)
    .addEntry("type","B","cluster type") // B - type Byte
    .addEntry("n", "S", "cluster multiplicity") // S - Type Short
    .addEntry("x", "F", "x position") // F - type float
    .addEntry("y","F","y position") // F - type float
    .addEntry("z", "F", "z position"); // F - type Float
        
  Schema schema = b.build();
        
  HipoWriter w = new HipoWriter();
  w.getSchemaFactory().addSchema(schema);
        
  w.open("clusters.h5");
        
  Random r = new Random();
  r.setSeed(123456);
        
  Event event = new Event();
  for(int i = 0; i < 1200; i++){
      event.reset();
      int nrows = r.nextInt(12)+4; // rows 4-16 randomly generated
      Bank bank = new Bank(schema,nrows);
      for(int row = 0; row < nrows; row++){
          bank.putByte( "type", row,  (byte) (r.nextInt(8)+1));
          bank.putShort(   "n", row, (short) (r.nextInt(15)+1));
          bank.putFloat(   "x", row, r.nextFloat());
          bank.putFloat(   "y", row, r.nextFloat());
          bank.putFloat(   "z", row, r.nextFloat());
      }
            
      event.write(bank); // write the bank to the event, event can contain multiple
      w.addEvent(event); // add event to the file
```

## Read File Example (Java)

Example shows how to read the file created using previous example code, and do sone analysis 
on the bank written.

``` java
HipoReader r = new HipoReader("clusters.h5");        
Bank[] banks = r.getBanks("data::clusters");
while(r.nextEvent(banks)){
  int nrows = banks[0].getRows();
  for(int row = 0; row < nrows; row++){
    if(banks[0].getInt("type", row)==5){
      int nhits = banks[0].getInt("n", row);
      if(nhits>4&&nhits<12){
        System.out.printf("%4d, %5d, %8.5f %8.5f %8.5f\n",
                           banks[0].getInt("type", row),nhits,
                           banks[0].getFloat("x", row),
                           banks[0].getFloat("y", row),
                           banks[0].getFloat("z", row));
      }
   }
  }
}
```


## Installing the Package (Java)

Clone the repository, and compile with maven. The package will be deployed soon.


## Installing the Package (C++)

### Dependencies
Click each for details
<details>
<summary>🔸 Meson and Ninja</summary>

> - Meson: <https://mesonbuild.com/>
> - Ninja: <https://ninja-build.org/>
>
> Likely available in your package manager (`apt`, `brew`, `dnf`, _etc_.),
> but the versions may be too old, in which case, use `pip` (`python -m pip install meson ninja`)
</details>

<details>
<summary>🔸 LZ4</summary>

> - LZ4: <https://lz4.org/>
>
> Likely available in your package manager, but if you do not have it, it will be installed locally for you
</details>

<details>
<summary>🔸 ROOT (optional)</summary>

> - ROOT: <https://root.cern.ch/>
>
> ROOT is _optional_ and _only_ needed for certain extensions and examples, such as [`HipoDataFrame`](/extensions/dataframes);
> if you do not have ROOT, the complete HIPO library will still be built.
</details>

### Building

Use standard [Meson commands](https://mesonbuild.com/Quick-guide.html) to build HIPO.

For example, first create a **build** directory; let's name it `./build`, set the installation location to `./install`, and
run the following command from the source code directory:
```bash
meson setup build --prefix=`pwd`/install
```
<details>
<summary>Click here for more details</summary>

> - you may run this command from _any_ directory; in that case, provide the path to the source code directory (_e.g._,
>   `meson setup build /path/to/source`)
> - the installation prefix must be an _absolute path_; you can change it later (`meson configure`)
</details>

The build directory is where you can compile, test, and more:
```bash
cd build
ninja           # compiles
ninja install   # installs to your specified prefix (../install/, in the example)
ninja test      # runs the tests
ninja clean     # clean the build directory, if you need to start over
