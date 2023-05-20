//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Example program for writing HIPO-4 Files..
//* Includes defining schemas, opening a file with dictionary
//*--
//* Author: G.Gavalian
//*


#include <iostream>
#include <stdlib.h>
#include "table.h"
#include "utils.h"
#include "bank.h"

void translation_benchmark(int iter){
    coda::table table;
    table.read("dc_tt.txt");
    coda::descriptor_t desc(41,3,46);
    int nrows = 108;
    hipo::benchmark  decode;
    decode.resume();
    for(int k = 0; k < iter; k++){
        for(int i = 0; i < nrows; i++)
            table.decode(desc);
    }
    decode.pause();
    printf("processed events %d time = %10.5f sec\n", iter, decode.getTimeSec());
}
/**
*  Main Program ........
*/
int main(int argc, char** argv){

    printf("--->>> example program for data translation\n");

    hipo::composite bank;
    bank.parse(std::string("bsiiifff"));
    bank.show();
    bank.print();

    printf("datasize = %d header = %d, size = %d\n" , 
         bank.getDataSize(), bank.getHeaderSize(),bank.getSize() );
    
    bank.putInt(0,0,4);
    bank.putInt(0,2,25);
    bank.putInt(0,4,35);
    bank.putFloat(1,6,1.2);
    bank.putFloat(2,7,2.2);
    printf("datasize = %d header = %d, size = %d\n" , 
         bank.getDataSize(), bank.getHeaderSize(),bank.getSize() );

         bank.print();
    //bank.putFloat(1,6,1.2);
//printf("datasize = %d header = %d, size = %d\n" , 
  //       bank.getDataSize(), bank.getHeaderSize(),bank.getSize() );
    /*bank.putInt(0,1,15);
    bank.putInt(0,2,25);
    bank.putInt(0,4,35);
    bank.putFloat(1,6,1.2);
    bank.putFloat(2,7,2.2);
    
    bank.show();
    bank.print();

    printf("datasize = %d\n" , bank.getDataSize()  );*/
    //translation_benchmark(1000000);
    /*
    coda::table table;
    table.read("dc_tt.txt");
    long value = table.encode(1,1,1,1);
    printf("%016lX\n",value);

    coda::descriptor_t desc;
    desc.crate = 41;
    desc.slot  =  3;
    desc.channel = 46;

    table.print(desc);
    table.decode(desc);
    table.print(desc);
    */
}
