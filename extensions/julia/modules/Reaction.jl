module Reaction
import Base.Libc.Libdl.dlopen

include("Vectors.jl")
using .Vectors

import Printf.@printf

mutable struct Descriptor
    vec::Vector4
    buff::Vector{Float64}
    sign::Vector{Int32}
     pid::Vector{Int32}
    skip::Vector{Int32}
    mass::Vector{Float64}
end

export Descriptor

function __init__()
  @printf("loading the Julia interface library\n")
  libjulia = dlopen("libjulia.so")
end

function extract_braces_content(input_string::String)
    # Use a regular expression to find all matches
    matches = eachmatch(r"\{(.*?)\}", input_string)
    # Extract and return the content within the braces, without the braces
    return [m.captures[1] for m in matches]
end

function define(line::String)
    desc = Descriptor(create4(0.0,0.0,0.0,0.0),Float64[0.0,0.0,0.0,0.0],Int32[],Int32[],Int32[],Float64[])
    h = extract_braces_content(line)
    for s in h
    	p = split(s,",")
	push!(desc.sign,parse(Int32,p[1]))
	push!(desc.pid,parse(Int32,p[2]))
	push!(desc.skip,parse(Int32,p[3]))
	push!(desc.mass,parse(Float64,p[4]))
    end
    return desc
end

function create4(x::Float64,y::Float64,z::Float64,e::Float64)
  v = Vector4(x,y,z,e)
end

function define(fname::String, energy::Float64)
   particles = Int32[11,211,-211]
   counts    = Int32[1,1,1]
   n = Int32(3)
   @ccall reaction_define(fname::Cstring,energy::Cdouble,particles::Ptr{Int32},counts::Ptr{Int32}, n::Int32)::Cvoid
end

function next()
   status = @ccall reaction_next()::Cint
end

function mass(a::Vector4)
  sqrt(a.e*a.e-a.x*a.x-a.y*a.y-a.z*a.z)
end

function mass2(a::Vector4)
  return a.e*a.e-a.x*a.x-a.y*a.y-a.z*a.z
end

function show(a::Vector4)
   @printf("vector3 [%8.5f,%8.5f,%8.5f] {%f} \n",a.x,a.y,a.z,a.e)
end

function getVector()
    V = Float64[0.0,0.0,0.0,0.0]
    S = Int32[1,1,1]
    P = Int32[11,211,-211]
    SK = Int32[0,0,0]
    M = Float64[0.0005,0.139,0.139]
    N = Int32(3)
    @ccall reaction_get_vector(V::Ptr{Cdouble}, S::Ptr{Cint},P::Ptr{Cint},SK::Ptr{Cint},M::Ptr{Cdouble},N::Cint)::Cvoid
    v = Vector4(0.0,0.0,0.0,0.0)
    #@printf("FROM MODULE %f %f %f %f\n",V[1],V[2],V[3],V[4])
    v.x = V[1]
    v.y = V[2]
    v.z = V[3]
    v.e = V[4]
    return v
end

function getVector(desc::Descriptor)
   N = Int32(size(desc.sign)[1])
   @ccall reaction_get_vector(desc.buff::Ptr{Cdouble}, desc.sign::Ptr{Cint},desc.pid::Ptr{Cint},desc.skip::Ptr{Cint},desc.mass::Ptr{Cdouble},N::Cint)::Cvoid
   desc.vec.x = desc.buff[1]
   desc.vec.y = desc.buff[2]
   desc.vec.z = desc.buff[3]
   desc.vec.e = desc.buff[4]
   return desc.vec
end

function cm()
  V =	[0.0,0.0,0.0,0.0]
  @ccall reaction_get_cm(V::Ptr{Cdouble})::Cvoid
  v = Vector4(0.0,0.0,0.0,0.0)
  v.x = V[1]
  v.y = V[2]
  v.z = V[3]
  v.e = V[4]
  return v  
end

function isValid()
   status = @ccall reaction_is_valid()::Cint
end

function getVector3()
   v = Vector3(0.7,0.7,0.7)
   A = [0.7,0.7,0.7]
   @ccall reaction_get_vector_debug(A::Ptr{Cdouble})::Cvoid
   #@printf("x component = %f %f\n",A[1],A[2])
   v.x = A[1]
   v.y = A[2]
   v.z = A[3]
   return v
end

function getVector2()
   v = Vector3(0.7,0.7,0.7)
   @ccall reaction_get_vector_debug(v::Ptr{Cdouble})::Cvoid
   return v
end

end
