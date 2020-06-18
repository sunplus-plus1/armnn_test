#ARMNN_LIB = ${HOME}/armnn-devenv/armnn/build
#ARMNN_INC = ${HOME}/armnn-devenv/armnn/include
ARMNN_LIB = /home/pi/armnn_test/armnn-dist/armnn/lib
ARMNN_INC = /home/pi/armnn_test/armnn-dist/armnn/include

all: mnist_tf_convol reminder

mnist_tf_convol: mnist_tf_convol.cpp mnist_loader.hpp
		g++ -O3 -std=c++17 -I/home/pi/armnn_test/armnn-dist/boost/include -I$(ARMNN_INC) $< -o $@ -L$(ARMNN_LIB) -larmnn -larmnnTfParser -lpthread

clean:
	-rm -f mnist_tf_convol

.PHONY: reminder
reminder: 
	@echo "Make sure to add Arm NN: export LD_LIBRARY_PATH=$(ARMNN_LIB)"


