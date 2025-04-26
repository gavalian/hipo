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
include("modules/Vectors.jl")
include("modules/Reaction.jl")

using .Hipo
using .Vectors
using .Reaction
using  Plots

desc = Reaction.define("{1,11,0,0.0005}{1,211,0,0.139}{1,-211,0,0.139}")

Reaction.define("infile.h5",10.6)

cm = Reaction.cm()

data = []
while Reaction.next()==1
  valid = Reaction.isValid()
  if valid==1
     v = Reaction.getVector(desc)
     mx = cm - v 
     mass2 = Reaction.mass2(mx)
     push!(data,mass2)
  end
end

h = Plots.histogram(data,bins = 0.25:0.025:3.75,title = "Reaction ep -> epi+pi-X", xlabel="Missing Mass Square (epi+pi-)", ylabel="Counts")
Plots.display(Plots.plot(h))

readline()



