# HIPO (Julia)

This is the first draft of Julia interface for reading hipo files.

# Background

With growing popularity of Julia in High Energy Physics (HEP) community leads to desire of having modern
data formats to have an interface in Julia. HiPO is the default data format for CLAS12 collaboration and
this is the first attempt to read HiPO from Julia. The first implementation is very simple but it
demonstrates how to read a bank and variables from file and print them on the screen.

# Example

The following example shows how to read CTOF::adc bank from standard CLAS12 decoded file and print values
of ADCs on the screen. To run the example, install julia on your system, switch to this directory 
and create a symbolic link to your file and run:

``` bash
prompt> ln -s /home/myname/myfile.hipo infile.h5
prompt> julia hipoLib.jl
```

Code:
``` julia
file = hipofile("infile.h5")
println(file)
hdefine(file,"CTOF::adc")
hdescribe(file,"CTOF::adc")

for j = 1:27
  hnext(file)
  size = hsize(file,"CTOF::adc")
  for r = 1:size
     cmp = hgetint(file,"CTOF::adc","component",Int32(r-1))
     adc = hgetint(file,"CTOF::adc","ADC",Int32(r-1))
    time = hgetfloat(file,"CTOF::adc","time",Int32(r-1))
    @printf("%4d [%8d] {%9.3f} , ",cmp, adc, time)
  end
  println()
end
```

Output:

``` bash
schema :      CTOF::adc , group =  20400, item =  11
          sector : (  B)     1     1 , offset =   0 --> [sector]
           layer : (  B)     1     1 , offset =   1 --> [layer]
       component : (  S)     2     2 , offset =   2 --> [component]
           order : (  B)     1     1 , offset =   4 --> [order]
             ADC : (  I)     3     4 , offset =   5 --> [ADC]
            time : (  F)     4     4 , offset =   9 --> [time]
             ped : (  S)     2     2 , offset =  13 --> [ped]
  30 [    3638] {  116.000} ,   30 [    4860] {  113.000} ,
  24 [    2301] {  114.000} ,   24 [    2390] {  114.000} ,
  28 [     869] {  114.000} ,   28 [     771] {  114.000} ,   29 [     762] {  114.000} ,   29 [     780] {  114.000} ,
  41 [    1770] {  115.000} ,   41 [    1989] {  113.000} ,
  31 [    2189] {  114.000} ,   31 [    2006] {  115.000} ,
  40 [    1875] {  114.000} ,   40 [    2026] {  114.000} ,   41 [     470] {  114.000} ,   41 [     509] {  114.000} ,
  44 [    2529] {  115.000} ,   44 [    3159] {  113.000} ,
  26 [    2677] {  114.000} ,   26 [    2774] {  114.000} ,
  32 [    2515] {  114.000} ,   32 [    2536] {  114.000} ,

  45 [    2038] {  116.000} ,   45 [    2976] {  113.000} ,
  26 [    1615] {  115.000} ,   26 [    1936] {  113.000} ,
  36 [    2736] {  114.000} ,   36 [    2573] {  115.000} ,   37 [      66] {  114.000} ,   37 [      63] {  115.000} ,   45 [    3000] {  115.000} ,   45 [    2006] {  118.000} ,
  19 [    2204] {  116.000} ,   19 [    3394] {  112.000} ,
  46 [    4102] {  114.000} ,   46 [    3498] {  115.000} ,
  20 [    1581] {  115.000} ,   20 [    2026] {  113.000} ,
  35 [    1612] {  116.000} ,   35 [    2197] {  113.000} ,
  24 [    1810] {  114.000} ,   24 [    1841] {  114.000} ,
  24 [     915] {  114.000} ,   24 [     884] {  114.000} ,   25 [     914] {  115.000} ,   25 [     909] {  114.000} ,
  38 [    1769] {  115.000} ,   38 [    2265] {  113.000} ,
  36 [    1846] {  115.000} ,   36 [    2092] {  113.000} ,
   1 [    1637] {  116.000} ,    1 [    2353] {  113.000} ,
  40 [    1631] {  114.000} ,   40 [    1647] {  114.000} ,
  25 [    2125] {  117.000} ,   25 [    3200] {  113.000} ,
  24 [    2473] {  115.000} ,   24 [    3050] {  113.000} ,
  38 [    1047] {  114.000} ,   38 [    1053] {  114.000} ,   39 [     670] {  115.000} ,   39 [     776] {  114.000} ,
   1 [    1723] {  114.000} ,    1 [    1802] {  114.000} ,    2 [      94] {  114.000} ,    2 [      84] {  114.000} ,
```

# Plotting

The Plots package in Julia allows histogram plotting from the script. An example can be found in file hipoPlot.jl. 
To use the script you need to install Plots package in julia following:
```
prompt> julia

julia> using Pkg
julia> Pkg.add("Plots");
```

Output:
![ADC spectra for CTOF](adcplot.png?raw=true "CTOF adc plot")
