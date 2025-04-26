/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package j4np.hipo5.examples;

import j4np.hipo5.data.Bank;
import j4np.hipo5.data.Event;
import j4np.hipo5.data.Node;
import j4np.hipo5.io.HipoReader;

/**
 *
 * @author gavalian
 */
public class ReadFile {
    
    /**
     * Reads the file created by code WriteFile.java. Reads the bank 
     * in each event, and then iterates over the rows of the bank
     * and prints out clusters of type==5, and with number of hits
     * in the cluster between 5 and 11 inclusive.
     */
    public static void readFileWithBank(){
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
    }
    /**
     * Reading file containing different types of nodes, the file is originally
     * created by code from class WriteFile, and contains 3 nodes in each
     * event of the file. The reader reads every event in the file, reads nodes
     * and prints the content on the screen, also calculated the average 
     * of the numbers in the node with the type float.
     */
    public static void readFileWithNodes(){
        HipoReader r = new HipoReader("nodes.h5");
        Event event = new Event();
        int counter = 0;
        
        double accumulate = 0.0;
        int       nvalues = 0;
        while(r.next(event)==true){
            
            counter++;
            // each node has unique identifiers, group and item
            // these numbers can be used to group relevant nodes together
            // otherwise they are arbitrary numbers used at users discretion.
            
            Node nb = event.read(12, 1); // 12,1 are unique identifiers of the node
            Node ni = event.read(12, 2); 
            Node nf = event.read(12, 3);
            
            //-- iterating over the node
            for(int j = 0; j < nf.getDataSize(); j++){
                accumulate += nf.getFloat(j);
            }
            nvalues += nf.getDataSize();
            System.out.println("---- event # " + counter);
            nb.show(); ni.show(); nf.show();
        }
        
        System.out.printf("\n\n>>> average for float node (12,3) = %f\n",accumulate/nvalues);
    }
    /**
     * Reads each event in the file and prints out the information about the
     * content of the event. prints all the objects in the event, with their 
     * unique identifiers, types and sizes
     * @param file - the file name to scan
     */
    public static void showFileContent(String file){
        HipoReader r = new HipoReader(file);
        Event event = new Event();
        
        while(r.next(event)==true){
            event.scanShow();
        }
    }
    
    public static void main(String[] args){
        ReadFile.readFileWithBank();
        ReadFile.showFileContent("nodes.h5");
        
        ReadFile.readFileWithNodes();
    }
}
