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
include("modules/HipoLib.jl")
using .HipoLib
using BenchmarkTools

function readHipoFile()
   file   = HipoLib.open("infile.h5")
   part   = HipoLib.getBank(file,"REC::Particle")
   counter = 0
   while HipoLib.next(file)==1
     HipoLib.read(file,"REC::Particle",part)
     counter += 1
   end
   println("processed $counter events")
   println(part)
end


for i in 1:5
@time readHipoFile()
println()
end
