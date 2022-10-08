FILE=$1
make
#---- Running converter for writing benchmarks
./converter.exe $1
#---- Running C++ read/write benchmarks
./benchmark.exe $1_w.hipo $1_w.root
#---- Running Java Benchmarks
javac --class-path $J4NPDIR/lib/core/j4np-1.0.5.jar Benchmark.java
java -Xmx512m -Xms128m  -cp $J4NPDIR/lib/core/j4np-1.0.5.jar:. Benchmark $1_w.hipo
