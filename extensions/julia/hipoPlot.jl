##################################################################
## JULIA interface for reading HIPO files.                      ##
## This interface is using fusion class from the hipo4 module   ##
## with "C" extern intraface defined in wrapper.cpp class.      ##
## the fusion interface allows opening multiple files through   ##
## providing unique handles for each opened file.               ##
##                                                              ##
## Author: G.Gavalian (2024)                                    ##
## Jefferson Lab --------------------------------               ##
## Disclaimer: The author was a 20 minute into Learning Julia   ##
##             when he implemented this crap.                   ##   
##################################################################

import Base.Libc.Libdl.dlopen
import Printf.@printf
using  Plots

mutable struct hipofile
       hid::Int32
       function hipofile(name::String)
          liblz4a = dlopen("../../slib/liblz4.so")
          libhipo = dlopen("../../slib/libhipo4.so")
          hid = @ccall fusion_open(name::Cstring)::Int32
       end
end

function hdescribe(fid::Int32,bank::String)
    @ccall fusion_describe(fid::Int32,bank::Cstring)::Cvoid
end

function hdefine(fid::Int32,bank::String)
    @ccall fusion_define(fid::Int32,bank::Cstring)::Cvoid
end

function hnext(fid::Int32)
    @ccall fusion_next(fid::Int32)::Cvoid
end

function hgetint(fid::Int32,bank::String,element::String,row::Int32)
   value = @ccall fusion_get_int(fid::Int32,bank::Cstring,element::Cstring,row::Int32)::Int32
   return value
end

function hgetfloat(fid::Int32,bank::String,element::String,row::Int32)
   value = @ccall fusion_get_float(fid::Int32,bank::Cstring,element::Cstring,row::Int32)::Float32
   return value
end

function hsize(fid::Int32,bank::String)
    size = @ccall fusion_bankSize(fid::Int32,bank::Cstring)::Int32
    return size
end

#---------------------------------------------------
# Main Program
#---------------------------------------------------

@printf("opening file %s\n","infile.h5")

file = hipofile("infile.h5")
println(file)
hdefine(file,"CTOF::adc")
hdescribe(file,"CTOF::adc")

vec = []

for j = 1:15000
  hnext(file)
  size = hsize(file,"CTOF::adc")
  for r = 1:size
     cmp = hgetint(file,"CTOF::adc","component",Int32(r-1))
     adc = hgetint(file,"CTOF::adc","ADC",Int32(r-1))
    time = hgetfloat(file,"CTOF::adc","time",Int32(r-1))
    if adc>300
      push!(vec,Float64(adc)::Float64)
    end
    @printf("%4d [%8d] {%9.3f} , ",cmp, adc, time)
  end
  println()
end

@printf("done..")
h = Plots.histogram(vec,bins = 0:80:4000)
Plots.display(Plots.plot(h))

#-- you need this line to wait before exiting
readline()
