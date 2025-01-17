﻿//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <array>
#include <algorithm>
#include "armnn/ArmNN.hpp"
#include "armnn/Exceptions.hpp"
#include "armnn/Tensor.hpp"
#include "armnn/INetwork.hpp"
#include "armnnCaffeParser/ICaffeParser.hpp"

#include "image_loader.hpp"


// Helper function to make input tensors
armnn::InputTensors MakeInputTensors(const std::pair<armnn::LayerBindingId,
    armnn::TensorInfo>& input,
    const void* inputTensorData)
{
    return { { input.first, armnn::ConstTensor(input.second, inputTensorData) } };
}

// Helper function to make output tensors
armnn::OutputTensors MakeOutputTensors(const std::pair<armnn::LayerBindingId,
    armnn::TensorInfo>& output,
    void* outputTensorData)
{
    return { { output.first, armnn::Tensor(output.second, outputTensorData) } };
}

int main(int argc, char** argv)
{
    // Load a test image and its correct label
    std::string imagePath = "data/label17_resize357x256_crop224x224.bmp";
    unsigned int imageLabel = 17;
    std::unique_ptr<TestImage> input = loadTestImage(imagePath, imageLabel);
    if (input == nullptr)
        return 1;

    // Import the Caffe model. Note: use CreateNetworkFromTextFile for text files.
    armnnCaffeParser::ICaffeParserPtr parser = armnnCaffeParser::ICaffeParser::Create();
    armnn::INetworkPtr network = parser->CreateNetworkFromBinaryFile("model/mobilenet_v2_for_armnn.caffemodel",
                                                                   { }, // input taken from file if empty
                                                                   { "prob" }); // output node

    // Find the binding points for the input and output nodes
    armnnCaffeParser::BindingPointInfo inputBindingInfo = parser->GetNetworkInputBindingInfo("data");
    armnnCaffeParser::BindingPointInfo outputBindingInfo = parser->GetNetworkOutputBindingInfo("prob");



    // Optimize the network for a specific runtime compute device, e.g. CpuAcc, GpuAcc
    //armnn::IRuntimePtr runtime = armnn::IRuntime::Create(armnn::Compute::CpuAcc);
    armnn::IRuntime::CreationOptions options;
    armnn::IRuntimePtr runtime = armnn::IRuntime::Create(options);
    armnn::IOptimizedNetworkPtr optNet = armnn::Optimize(*network, {armnn::Compute::CpuRef}, runtime->GetDeviceSpec());

    // Load the optimized network onto the runtime device
    armnn::NetworkId networkIdentifier;
    runtime->LoadNetwork(networkIdentifier, std::move(optNet));

    // Run a single inference on the test image
    std::array<float, 1000> output;
    armnn::Status ret = runtime->EnqueueWorkload(networkIdentifier,
                                                 MakeInputTensors(inputBindingInfo, &input->image[0]),
                                                 MakeOutputTensors(outputBindingInfo, &output[0]));

    // Convert 1-hot output to an integer label and print
    int label = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
    for (unsigned int i = 0; i < 100; ++i)
    {
        std::cout << "output[" << i << "] = " << output[i] << std::endl;
    }
    std::cout << "Predicted: " << label << std::endl;
    std::cout << "   Actual: " << input->label << std::endl;

    return 0;
}
