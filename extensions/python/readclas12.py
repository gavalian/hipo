# example of reading hipo file from python
from hipolib import hreader
import sys

filename = sys.argv[1]

print('processing file : ', filename)

# the directory with shared libraries provided
reader = hreader('../../slib')
reader.open(filename)
# define banks that will be read with each next() call
reader.define('RUN::config')
# prints the structure of the bank on the screen
reader.describe('RUN::config')

counter = 0

while(reader.next()==True):
    size = reader.getSize('RUN::config')
    print('event # ', counter ,' bank size = ',size)
    # get each column as an array from the bank
    if size>0:
        nevent    = reader.getEntry('RUN::config','event')
        trigger   = reader.getEntry('RUN::config','trigger')
        unixtime  = reader.getEntry('RUN::config','unixtime')
        timestamp = reader.getEntry('RUN::config','timestamp')
        print('event      = ',nevent)
        print('trigger    = ',trigger)
        print('unixtime   = ',unixtime)
        print('timestamp  = ',timestamp)
    counter = counter + 1

print('processed event # ', counter)
