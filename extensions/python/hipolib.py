##################################################################
## Python interface for reading HIPO files.                     ##
## This interface is using fusion class from the hipo4 module   ##
## with "C" extern intraface defined in wrapper.cpp class.      ##
## the fusion interface allows opening multiple files through   ##
## providing unique handles for each opened file.               ##
##                                                              ##
## Author: G.Gavalian (2022)                                    ##
## Jefferson Lab --------------------------------               ##
##################################################################

import ctypes

class hreader:

    def __init__(self,libfilename):
        """ hipo reader python class interface to hipo fusion wrapper
            (use it wisely)
        """
        self.libPath    = libfilename
        self.lz4lib     = ctypes.CDLL(self.libPath+'/liblz4.dylib');
        self.hipo4lib   = ctypes.CDLL(self.libPath+'/libhipo4.so')
        self.status     = ctypes.c_int(0)
        self.handle     = -1
        
    def open(self,filename):
        """ open the file and save the handle returned by fusion wrapper.
            each opened file has unique handle, so many files can be opened
            in parallel.
        """
        self.inputFile = filename
        self.handle    = self.hipo4lib.fusion_open(ctypes.c_char_p(self.inputFile.encode('ascii')))
        print('file open handle = ',self.handle)

    def define(self,bankname):
        """ define is used to declare a bank that will be read by the fusion wrapper
            each time next() is called. The banks are stored in the internal map for
            each opened file handle.
        """
        self.hipo4lib.fusion_define(self.handle,ctypes.c_char_p(bankname.encode('ascii')))

    def getSize(self,bankname):
        """ returns size of the bank that was read for current event.
        """
        size = self.hipo4lib.fusion_bankSize(self.handle,ctypes.c_char_p(bankname.encode('ascii')))
        return size

    def next(self):
        """ reads next event in the file, and reads all the banks that were decalred with
            hreader.define(bankname) function call.
        """
        status = self.hipo4lib.fusion_next(self.handle)
        return status

    def getInt(self,bank,entry,row):
        """ returns an integer value for entry and row from requested bank. call getSize()
            first to make sure that the row is within the allowable range to avoid hard crashes.
        """
        a1 = ctypes.c_char_p(bank.encode('ascii'))
        a2 = ctypes.c_char_p(entry.encode('ascii'))
        self.hipo4lib.fusion_get_float.restype = ctypes.c_int
        value = self.hipo4lib.fusion_get_int(self.handle,a1,a2,row)
        return value
    
    def getFloat(self,bank,entry,row):
        """ returns an float value for entry and row from requested bank. call getSize()
        """
        a1 = ctypes.c_char_p(bank.encode('ascii'))
        a2 = ctypes.c_char_p(entry.encode('ascii'))
        self.hipo4lib.fusion_get_float.restype = ctypes.c_float
        value = ctypes.c_float(self.hipo4lib.fusion_get_float(self.handle,a1,a2,row)).value
        return value

    def getType(self,bank,entry):
        """ returns type of the entry for the given bank. this is used to determine weather to
            use getInt or getFloat function.
        """
        a1 = ctypes.c_char_p(bank.encode('ascii'))
        a2 = ctypes.c_char_p(entry.encode('ascii'))
        type = self.hipo4lib.fusion_entry_type(self.handle,a1,a2)
        return type

    def getEntry(self,bank,entry):
        """ returns a python array containing all rows for given column. the determination of the
            is done internaly.
        """
        rows = self.getSize(bank)
        type = self.getType(bank,entry)
        array = []
        if(type==1 or type==2 or type==3):
            for row in range(rows):
                array.append(self.getInt(bank,entry,row))
        if(type==4):
            for row in range(rows):
                array.append(self.getFloat(bank,entry,row))
        return array
    
    def show(self,bank,entry):
        """  simple printout of the given column (entry) from the bank for debugging purposes
        """
        rows = self.getSize(bank)        
        type = self.getType(bank,entry)
        print('\t entry = ',entry,'   type = ', type)
        
        if(type==4):
            for row in range(rows):
                value = self.getFloat(bank,entry,row)
                print(row,' => ', value)
        if(type==1 or type==2 or type==3):
            for row in range(rows):
                value = self.getInt(bank,entry,row)
                print (row, ' => ', value)
        
    def test(self):
        """ test method for debugging array creation in C and conversion to python with ctypes
        """
        array_type = ctypes.c_float*10
        array_pointer = ctypes.cast(self.hipo4lib.fusion_create_array(10),ctypes.POINTER(array_type))
        print([i for i in array_pointer().contents])
        
#******************************************************************
# The main module for internal integrity testting. use this from
# the current directory, since the paths here are hardcoded.
#
# before running this code, compile the examples folder and run 
# example code : writeFile to produce the exemple file used
# by this code.
#******************************************************************
if __name__=="__main__":
    print('\nHIPO4 python interface.....')
    print('>>> executing main testing routine')
    filename = '../examples/example_output.hipo'
    reader = hreader('../slib')
    reader.open(filename)
    reader.define('event::particle')
    counter = 0
    while(reader.next()==True):
        size = reader.getSize('event::particle')
        print('bank size = ',size)
        counter = counter + 1

    print('processed event # ', counter)
