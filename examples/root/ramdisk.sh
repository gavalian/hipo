#!/bin/sh
#--- create a RAM disk
diskutil erasevolume HFS+ 'RAMDisk' `hdiutil attach -nomount ram://8388608`
