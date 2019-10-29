########################################################
## Python interface for reading HIPO files.
## Author: G.Gavalian (2019)
## Jefferson Lab
########################################################
import ctypes


class hiporeader:
    def __init__(self,libfilename):
        self.libPath    = libfilename
        self.dataReader = ctypes.CDLL(self.libPath)
        self.status     = ctypes.c_int(0)
        
    def open(self,filename):
        self.inputFile  = filename
        self.dataReader.hipo_file_open(ctypes.c_char_p(self.inputFile.encode('ascii')))


    def nextEvent(self):
        self.dataReader.hipo_file_next_(ctypes.byref(self.status))
        if self.status.value==0: return True
        return False

if __name__=="__main__":
    print('executing main testing routine')
    filename = '/Users/gavalian/Work/DataSpace/clas12/ai_dst_005038.hipo'
    reader = hiporeader('../slib/libhipo4.so')
    reader.open(filename)
    counter = 0
    while reader.nextEvent()==True:
        counter = counter + 1

    print('processed event # ', counter)
