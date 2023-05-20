# Drift Chamber De-Noising Software

This direcory contains software for de-noising CLAS12 drift chambers
using Convolutional Auto-Encoders (CAE).

# Building the software

To Build the software some dependencies have to be installed. The package
depends on the project:


[https://github.com/Dobiasd/frugally-deep](https://github.com/Dobiasd/frugally-deep)


The frugally-deep is a header only library allowing to run Keras Models
in C++. The Keras models, usually saved as h5 file, have to be run through
provided converter first, then can be used in C++ code.

The frugally-deep also denpends on few packaged, needed to be installed.
The instructions on what to install can be found [here](https://github.com/Dobiasd/frugally-deep/blob/master/INSTALL.md):

For convenience here are the instructions:

```
git clone -b 'v0.2.18-p0' --single-branch --depth 1 https://github.com/Dobiasd/FunctionalPlus
cd FunctionalPlus
mkdir -p build && cd build
cmake ..
make && sudo make install
cd ../..

git clone -b '3.4.0' --single-branch --depth 1 https://gitlab.com/libeigen/eigen.git
cd eigen
mkdir -p build && cd build
cmake ..
make && sudo make install
sudo ln -s /usr/local/include/eigen3/Eigen /usr/local/include/Eigen
cd ../..

git clone -b 'v3.10.5' --single-branch --depth 1 https://github.com/nlohmann/json
cd json
mkdir -p build && cd build
cmake -DJSON_BuildTests=OFF ..
make && sudo make install
cd ../..

git clone https://github.com/Dobiasd/frugally-deep
cd frugally-deep
mkdir -p build && cd build
cmake ..
make && sudo make install
cd ../..
```

After installing all required packages, the environmental variable "SOFTLOCAL"
has to be defined pointing to where the includes are for all these packages.
After that just type "make" in the directory to build the denoiser2 code.


# Activating Environment

```
$ conda create --name gan
$ conda activate gan
$ conda install -c pytorch pytorch=1.4.0
$ conda install matplotlib jupyter
```

```
conda activate mlp
```

