#*************************************************************************************
#                                                                                    #
#    _       _ _(_)_     |  This file is part of the example files using Hipo Data   #
#   (_)     | (_) (_)    |  Format from Julia programming language. Implemented for  #
#    _ _   _| |_  __ _   |  smothenning the transition of CLAS12 software to Julia   #
#   | | | | | | |/ _` |  |                                                           #
#   | | |_| | | | (_| |  |  Date: 07/03/2024                                         #
#  _/ |\__'_|_|_|\__'_|  |  Author: G.Gavalian                                       #
# |__/                   |  Disclaimer: Implemented by a very novice Julia user, so  #
#                        |              if you notice something done wrong, keep it  #
#                        |              to yourself (or in extreame cases contact    #
#                        |              the author). This is the way !               #
#*************************************************************************************

import Printf.@printf
include("modules/Hipo.jl")
using .Hipo
using  Plots

#unicodeplots() #- uncomment this line to use ASCII plotting backend

file = Hipo.open("infile.h5")
Hipo.define(file,"CTOF::adc")

vec = [] 
for j = 1:15000

   Hipo.next(file)
   rows = Hipo.getRows(file,"CTOF::adc")
  
   for r = 1:rows
     cmp = Hipo.getInt(   file,"CTOF::adc","component",Int32(r-1))
     adc = Hipo.getInt(   file,"CTOF::adc","ADC",Int32(r-1))
    time = Hipo.getFloat( file,"CTOF::adc","time",Int32(r-1))
    if adc>300
      push!(vec,Float64(adc)::Float64) # I don't know if the second ::Float64 is neccessary
    end
  end
end

h = Plots.histogram(vec,bins = 0:80:4000,title = "ctof ADC", xlabel="ADC", ylabel="Counts")
Plots.display(Plots.plot(h))

@printf("done..\n now plotting the adc spectra....\n\n\n press any key to exit (if you don't have a key named 'any', press 'enter')\n\n")
#-- you need this line to wait before exiting
readline()
