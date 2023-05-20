#!/bin/sh

for t in {1..4}
do
thread=$(($t*2))
#echo $thread
./denoise2.exe $1 $thread 12 | grep finally 
done
