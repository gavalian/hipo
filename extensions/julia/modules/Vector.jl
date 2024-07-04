module Vector
import Printf.@printf

struct Vector3
   x::Float64
   y::Float64
   z::Float64
end

struct Vector4
  x::Float64
  y::Float64
  z::Float64
  e::Float64
end


function add(a::Vector3,b::Vector3)
   Vector3(a.x + b.x, a.y + b.y, a.z + b.z)
end

function add(a::Vector4,b::Vector4)
   Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.e+b.e)
end

function show(a::Vector3)
   @printf("vector3 [%8.5f,%8.5f,%8.5f]\n",a.x,a.y,a.z)
end

function mass(a::Vector4)
    sqrt(a.e*a.e-a.x*a.x-a.y*a.y-a.z*a.z)
end

function mag(a::Vector4)
    sqrt(a.x*a.x+a.y*a.y+a.z*a.z)
end

function show(a::Vector4)
   @printf("vector3 [%8.5f,%8.5f,%8.5f] {%f} \n",a.x,a.y,a.z,a.e)
end

end
