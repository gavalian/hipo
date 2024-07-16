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
include("modules/Vector.jl")
using .Vector

a = Vector4(0.0,0.0,10.65,10.65)
b = Vector4(0.0,0.0,  0.0, 0.938)
c = Vector4(0.2,0.6,1.4,2.1)

c.x = 0.25

d = a + b - c

Vector.show(d)
@printf("mass = %f\n",Vector.mass(d))
