# HiPO coda tools

This library can be used to translate the detector data stored in EVIO
into HiPO data format, assigning detector id's trnslated from hardware
addresses using translation tables.

To run this code, the translation tables must be downloaded.

```
prompt> wget https://userweb.jlab.org/~gavalian/software/translation/dc_tt.txt
```

Then compile the code and run:

```
prompt> make
prompt> ./translate.exe input.evio
```
