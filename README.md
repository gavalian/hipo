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

## Write File Example (C++)

``` c++
    hipo::schema  schemaPart("event::particle",100,1);
    hipo::schema  schemaDet( "event::detector",100,2);
    //---------------------------------------------------------
    // Defining structure of the schema (bank)
    // The columns in the banks (or leafs, if you like ROOT)
    // are given comma separated with type after the name.
    // Available types : I-integer, S-short, B-byte, F-float,
    //                   D-double, L-long
    schemaPart.parse("pid/S,px/F,py/F,pz/F");
    schemaDet.parse("pindex/I,detectorid/I,x/F,y/F,z/F,time/F,energy/F");
    //---------------------------------------------------------
    // Open a writer and register schemas with the writer.
    // The schemas have to be added to the writer before openning
    // the file, since they are written into the header of the file.
    hipo::writer  writer;
    writer.getDictionary().addSchema(schemaPart);
    writer.getDictionary().addSchema(schemaDet);
    writer.open("outputFile.h5");

    //hipo::bank partBank(schemaPart,30);
    //hipo::bank detBank( schemaDet ,30);

    hipo::event outEvent;

    for(int i = 0; i < 25000; i++){

        int  nparts = 2 + rand()%10;
        int   ndets = 5 + rand()%20;
        //-------------------------------------------------
        // Create banks with random rows based on schemas
        hipo::bank partBank(schemaPart,nparts);
        hipo::bank detBank( schemaDet ,ndets);
        // Fill banks with random numbers
        dataFill(partBank); 
        dataFill(detBank);
        // Printout on the screen content of the banks
        printf("particles = %d, detectors = %d\n",nparts, ndets);
        partBank.show();
        detBank.show();
        //-------------------------------------------------
        // Clear output event Event append banks to the Event
        // and write to the output file
        outEvent.reset();
        outEvent.addStructure(partBank);
        outEvent.addStructure(detBank);
        writer.addEvent(outEvent);
   }
  writer.close();
```

Here is the subroutine that fills the banks:

``` c++
void dataFill(hipo::bank &bank){
    int    nrows = bank.getRows();
    int nentries = bank.getSchema().getEntries();
    for(int row = 0; row < nrows; row++){
       for(int e = 0 ; e < nentries; e++){
           int type = bank.getSchema().getEntryType(e);
           if(type==1||type==2||type==3){
              int inum = rand()%20;
              bank.putInt(bank.getSchema().getEntryName(e).c_str(),row,inum);
           }
           if(type==4){
             float ifloat = ((float) rand()) / (float) RAND_MAX;
             bank.putFloat(bank.getSchema().getEntryName(e).c_str(),row,ifloat);
           }
       }
    }
}
```

## Reading File Example (C++)

Example of reading file created with previous example, and showing the content of the banks.

``` c++
   hipo::reader  reader;
   reader.open("outputFile.h5");
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   factory.show();

   hipo::bank  particles(factory.getSchema("event::particle"));
   hipo::bank  detectors(factory.getSchema("event::detector"));
   hipo::event      event;
   int counter = 0;
   while(reader.next()==true){
      reader.read(event);

      event.read(particles);
      event.read(detectors);

      particles.show();
      detectors.show();

      int nrows = particles.getRows();
      printf("---------- PARTICLE BANK CONTENT -------\n");
      for(int row = 0; row < nrows; row++){
         int   pid = particles.getInt("pid",row);
         float  px = particles.getFloat("px",row);
         float  py = particles.getFloat("py",row);
         float  pz = particles.getFloat("pz",row);
         printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
      }
      printf("---------- END OF PARTICLE BANK -------\n");

      counter++;
   }
   printf("processed events = %d\n",counter);
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
