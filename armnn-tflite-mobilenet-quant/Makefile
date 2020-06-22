#ARMNN_LIB = /home/pi/armnn-pi2/armnn/build
ARMNN_INC = /home/pi/armnn_test/armnn-dist/armnn
ARMNN_ROOT = /home/pi/armnn_test/armnn-dist
ARMNN_BUILD = /home/pi/armnn_test/armnn-dist/armnn/lib
BOOST_ROOT = /home/pi/armnn_test/armnn-dist/boost

CPPFLAGS=-DARMNN_TF_LITE_PARSER -I$(ARMNN_INC)/include -I$(ARMNN_ROOT)/src/backends -I$(ARMNN_ROOT)/src/armnnUtils -I$(ARMNN_ROOT)/tests -I$(BOOST_ROOT)/include
CFLAGS=-Wall -O3 -std=c++14 -fPIE

LDFLAGS=-pie -L$(ARMNN_BUILD) -L$(ARMNN_BUILD)/tests -L$(BOOST_ROOT)/lib
LDLIBS=-pthread -larmnn -larmnnTfLiteParser -linferenceTest -lboost_system -lboost_filesystem -lboost_program_options

all: mobilenetv1_quant_tflite

mobilenetv1_quant_tflite: mobilenetv1_quant_tflite.cpp model_output_labels_loader.hpp
	$(CXX) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	-rm -f mobilenetv1_quant_tflite

test:
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/pi/armnn_test/armnn-dist/armnn/lib ./mobilenetv1_quant_tflite -m ./model/mobilenet_v1_1.0_224_quant.tflite -d ./data/dog.jpg -p ./model/labels_mobilenet_quant_v1_224.txt -c CpuAcc
#test:
#	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/pi/armnn-pi2/armnn/build ./mobilenetv1_quant_tflite -m ./model/mobilenet_v1_0.25_128_quant.tflite -d ./data/dog.jpg -p ./model/labels_mobilenet_quant_v1_224.txt -c CpuAcc
