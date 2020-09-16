//ARMNN_LIB = ${HOME}/devenv/build-x86_64/release/armnn
//ARMNN_INC = ${HOME}/devenv/armnn/include 
//ARMNN_LIB = /home/pi/armnn-pi/armnn-dist/armnn/lib
//ARMNN_INC = /home/pi/armnn-pi/armnn-dist/armnn/include
ARMNN_LIB = /home/pi/armnn-pi/armnn/build
ARMNN_INC = /home/pi/armnn-pi/armnn/include

all: mnist_onnx.cpp mnist_loader.hpp
	g++ -O3 -std=c++14 -I$(ARMNN_INC) mnist_onnx.cpp -o mnist_onnx -L$(ARMNN_LIB) -larmnn -larmnnOnnxParser 

clean:
	-rm -f mnist_onnx

test: 
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(ARMNN_LIB) ./mnist_onnx
