#****************************************************************************************
#                                                                                       #
#    _       _ _(_)_     |       >=<     Julia module for reading HiPO (4&5) files      # 
#   (_)     | (_) (_)    |  ,.--'  ''-.  Implemented with wooping 17  hours of Julia    #
#    _ _   _| |_  __ _   |  (  )  ',_.'  experience. Use with causion. I have spoken    #
#   | | | | | | |/ _` |  |   Xx'xX                              Jefferson Lab (2024)    #
#   | | |_| | | | (_| |  |  ~~~~~~~~~~~~~                                               #
#  _/ |\__'_|_|_|\__'_|  |  Date: 07/03/2024                                            #
# |__/                   |  Author: G.Gavalian                                          #
#                        |  Advice:     if you notice something done wrong, keep it     #
#                        |              to yourself (or in extreame cases contact       #
#                        |              the author). This is the way !                  #
#****************************************************************************************

module HipoLib

import Printf.@printf
import Base.Libc.Libdl.dlopen
using DataFrames

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

#-----------------------------------------------------------------------------------------
# OPEN a HIPO file. returns a file handler that has to be used in all subsequent requests
#  Several files can be opened at once, for accessing each file the proper file handler
# must be used
#-----------------------------------------------------------------------------------------
function open(filename::String)
  hid = @ccall fusion_open(filename::Cstring)::Int32
end
#-----------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------
function define(fid::Int32,bank::String)
    @ccall fusion_define(fid::Int32,bank::Cstring)::Cvoid
end
#-----------------------------------------------------------------------------------------
# Read next event in the file with provided file handler
#-----------------------------------------------------------------------------------------
function next(fid::Int32)
    @ccall fusion_next(fid::Int32)::Cint
end
#-----------------------------------------------------------------------------------------
# Read specified bank into a DataFrame, Note ! the data frame has to be created first
# by calling getBank(filehandler,bankname), then the dataframe ojbject has to be passed
# to this function
#-----------------------------------------------------------------------------------------
function read(fid::Int32, bank::String, df::DataFrame)
   nrows  = getRows(fid,bank)
   dataResize(df,nrows)
   n_cols = size(df, 2)
   for i in 1:n_cols
     col = df[!,i]
     element = i - 1
     if(typeof(col)==Vector{Int32})
	@ccall fusion_get_int_array(fid::Cint, bank::Cstring, element::Cint, col::Ptr{Int32},nrows::Cint )::Cvoid
     end
     if(typeof(col)==Vector{Float32})
	@ccall fusion_get_float_array(fid::Cint, bank::Cstring, element::Cint, col::Ptr{Int32},nrows::Cint )::Cvoid
     end
     if(typeof(col)==Vector{Int16})
        @ccall fusion_get_short_array(fid::Cint, bank::Cstring, element::Cint, col::Ptr{Int32},nrows::Cint )::Cvoid
     end
     if(typeof(col)==Vector{Int8})
	@ccall fusion_get_byte_array(fid::Cint, bank::Cstring, element::Cint, col::Ptr{Int32},nrows::Cint )::Cvoid
     end
   end
end
#-----------------------------------------------------------------------------------------
# internal function to retrieve the schema for given bank from the file dictionary.
# used by getBank function
#-----------------------------------------------------------------------------------------
function getSchema(fid::Int32, bank::String)
  length = @ccall fusion_schema_length(fid::Int32, bank::Cstring)::Cint
  format = repeat("A",length+1)
  @ccall fusion_schema_string(fid::Int32, bank::Cstring, format::Ptr{Cchar})::Cvoid
  return format
end
#-----------------------------------------------------------------------------
# Parse schema format and construct a data frame where the bank will be read
#-----------------------------------------------------------------------------
function getBank(fid::Int32, bank::String)
   define(fid,bank)
   format = getSchema(fid,bank)
   matches = eachmatch(r"\{(.*?)\}", format)
   data = [m.captures[1] for m in matches]
   entries = split(data[2],",")
   df = DataFrame()
   for entry in entries
     fp = split(entry,"/")
     if fp[2] == "I" 
       df[:, Symbol(fp[1])] = Int32[]
     end
     if fp[2] == "B"
       df[:, Symbol(fp[1])] = Int8[]
     end
     if fp[2] == "S"
       df[:, Symbol(fp[1])] = Int16[]
     end
     if fp[2] == "F"
        df[:, Symbol(fp[1])] = Float32[]
     end
     if	fp[2] == "D"
      	df[:, Symbol(fp[1])] = Float64[]
     end
     if fp[2] == "L"
        df[:, Symbol(fp[1])] = Int64[]
     end
   end
   return df
end

#-----------------------------------------------------------------------------------------
# Resize internal DataFrame arrays based on the rows of the bank, the Dataframe has
# to have proper size arrays to be passed to "C" interface
#-----------------------------------------------------------------------------------------
function dataResize(df::DataFrame, length::Int32)
   n_cols = size(df, 2)
   for i in 1:n_cols
     resize!(df[!,i],length)
   end
end
#-----------------------------------------------------------------------------------------
# Returns the number of rows for the given bank. Used to resize the DataFrame arrays
# before passing them to read function
#-----------------------------------------------------------------------------------------
function getRows(fid::Int32,bank::String)
    size = @ccall fusion_bankSize(fid::Int32,bank::Cstring)::Int32
    return size
end

end
