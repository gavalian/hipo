/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package j4np.hipo5.examples;

import j4np.hipo5.data.Bank;
import j4np.hipo5.data.Event;
import j4np.hipo5.data.Node;
import j4np.hipo5.data.Schema;
import j4np.hipo5.data.Schema.SchemaBuilder;
import j4np.hipo5.io.HipoWriter;
import java.util.Random;

/**
 *
 * @author gavalian
 */
public class WriteFile {
    public static Random rand = new Random(123456);
    /**
     * create schema for holding cluster information.
     * @return 
     */
    public static Schema createSchema(){
         SchemaBuilder b = new SchemaBuilder("data::clusters",12,1)
                .addEntry("type","B","cluster type") // B - type Byte
                .addEntry("n", "S", "cluster multiplicity") // S - Type Short
                .addEntry("x", "F", "x position") // F - type float
                .addEntry("y","F","y position") // F - type float
                .addEntry("z", "F", "z position"); // F - type Float
        
        Schema schema = b.build();
        return schema;
    }
    
    /**
     * populate the cluster bank with random numbers.
     * @param bank 
     */
    public static void populate(Bank bank){
        int nrows = bank.getRows();
        for(int row = 0; row < nrows; row++){
                bank.putByte( "type", row,  (byte) (rand.nextInt(8)+1));
                bank.putShort(   "n", row, (short) (rand.nextInt(15)+1));
                bank.putFloat(   "x", row, rand.nextFloat());
                bank.putFloat(   "y", row, rand.nextFloat());
                bank.putFloat(   "z", row, rand.nextFloat());
            }
    }
    /**
     * write a similar file as in writeFileWithBank method, but separate the events
     * in different buckets depending on the number of rows in of the bank.
     * Any criteria can be used to tag the event. 
     */
    public static void writeFileWithBankTags(){
        Schema schema = WriteFile.createSchema();
        
        HipoWriter w = new HipoWriter();
        w.getSchemaFactory().addSchema(schema);
        w.open("clusters_tagged.h5");
        Event event = new Event();
        for(int i = 0; i < 1200; i++){
            int nrows = rand.nextInt(6, 19); // random number between 6-18 inclusive
            Bank bank = new Bank(schema,nrows);
            WriteFile.populate(bank);
            event.reset();
            event.write(bank);
            //-------------------------------------------------------------------
            // This part separates in the file events with rows <10 and rows>10
            // into separate buckets, byt assigning tag to the event. When reading
            // the file only specific tag can be read, minimizing I/O
            if(bank.getRows()>10) event.setEventTag(1); else event.setEventTag(2);
            w.addEvent(event);
        }
        w.close();
    }
    /**
     * This example writes a file with one bank (table like structure) 
     * in each event, the schema must be first created and added to 
     * writer's schema factory to be saved. The example core on how to
     * read the file created by this code can be found in ReadFile.java
     * code.
     */
    public static void writeFileWithBank(){
    
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
        }        
        w.close();
    }
    /**
     * Write a HIPO file with nodes in each event, three nodes are created with
     * random number of length and different types, byte, integer, and float.
     */
    public static void writeFileWithNodes(){
        Random r = new Random();
        r.setSeed(123456);
        
        HipoWriter w = new HipoWriter();
        w.open("nodes.h5");
        Event  event = new Event();
        
        for(int i = 0; i < 200; i++){
            int  nbytes = r.nextInt(3,12);
            int nfloats = r.nextInt(3,12);
            int   nints = r.nextInt(3,12);
            
            byte[]  barray = new   byte[nbytes];
            int[]   iarray = new    int[nints];
            float[] farray = new  float[nfloats];
            
            for(int j = 0; j < barray.length; j++) barray[j] = (byte) r.nextInt(128);
            for(int j = 0; j < iarray.length; j++) iarray[j] =  r.nextInt(2000) - 1000;
            for(int j = 0; j < farray.length; j++) farray[j] =  r.nextFloat()*2.0f;
            
            Node nodeb = new Node(12,1,barray);
            Node nodei = new Node(12,2,iarray);
            Node nodef = new Node(12,3,farray);
            event.reset();
            event.write(nodeb);
            event.write(nodei);
            event.write(nodef);
             w.addEvent(event);
        }
        w.close();
    }
    
    public static void main(String[] args){
        
        WriteFile.writeFileWithBank();                
        WriteFile.writeFileWithNodes();
        WriteFile.writeFileWithBankTags();
    }
}
