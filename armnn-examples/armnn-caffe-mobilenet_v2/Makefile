
all:
	g++ -O3 -std=c++14 -I/home/pi/armnn-pi/armnn/include -I/home/pi/armnn-pi/boost/include mobilenet_v2_caffe.cpp -o mobilenet_v2_caffe -L/home/pi/armnn-pi/armnn/build -larmnn -larmnnCaffeParser

clean:
	-rm -f mobilenet_v2_caffe

test:
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/pi/armnn-pi/armnn/build ./mobilenet_v2_caffe
