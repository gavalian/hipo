.PHONY: eigen json fplus frugal

all: eigen json fplus frugal

eigen:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../eigen && make install
	cd install/include && ln -s eigen3/Eigen .

json:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../json && make -j16 install

fplus:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../FunctionalPlus && make install

frugal:
	rm -rf build && mkdir -p build install && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install ../frugally-deep && make install

clean:
	rm -rf build install

