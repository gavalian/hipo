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
