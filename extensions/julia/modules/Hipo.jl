#****************************************************************************************
#                                                                                       #
#    _       _ _(_)_     |       >=<     Julia module for reading HiPO (4&5) files      # 
#   (_)     | (_) (_)    |  ,.--'  ''-.  Implemented with wooping 25 minutes of Julia   #
#    _ _   _| |_  __ _   |  (  )  ',_.'  experience. Use with causion. I have spoken    #
#   | | | | | | |/ _` |  |   Xx'xX                              Jefferson Lab (2024)    #
#   | | |_| | | | (_| |  |  ~~~~~~~~~~~~~                                               #
#  _/ |\__'_|_|_|\__'_|  |  Date: 07/03/2024                                            #
# |__/                   |  Author: G.Gavalian                                          #
#                        |  Advice:     if you notice something done wrong, keep it     #
#                        |              to yourself (or in extreame cases contact       #
#                        |              the author). This is the way !                  #
#****************************************************************************************

module Hipo

import Printf.@printf
import Base.Libc.Libdl.dlopen

function __init__()
  @printf("\n Loading Hipo Library interface....\n\n")
  liblz4a = dlopen("../../slib/liblz4.so")
  libhipo = dlopen("../../slib/libhipo4.so")    
  @printf(" +------------------------------------------------------------+\n")
  @printf(" | HiPO interface to Julia language   (Jefferson Lab, 2024)   |\n")
  @printf(" | Release Date: 07/02/2024 ,            Author: G.Gavalian   |\n")
  @printf(" | Disclaimer:   Implemented by extreamly novice Julia user   |\n")
  @printf(" +------------------------------------------------------------+\n\n")
end

function open(filename::String)
  hid = @ccall fusion_open(filename::Cstring)::Int32
end

function define(fid::Int32,bank::String)
    @ccall fusion_define(fid::Int32,bank::Cstring)::Cvoid
end

function next(fid::Int32)
    @ccall fusion_next(fid::Int32)::Cvoid
end

function getRows(fid::Int32,bank::String)
    size = @ccall fusion_bankSize(fid::Int32,bank::Cstring)::Int32
    return size
end

function getInt(fid::Int32,bank::String,element::String,row::Int32)
   value = @ccall fusion_get_int(fid::Int32,bank::Cstring,element::Cstring,row::Int32)::Int32
   return value
end

function getFloat(fid::Int32,bank::String,element::String,row::Int32)
   value = @ccall fusion_get_float(fid::Int32,bank::Cstring,element::Cstring,row::Int32)::Float32
   return value
end

function describe(fid::Int32,bank::String)
    @ccall fusion_describe(fid::Int32,bank::Cstring)::Cvoid
end

end
