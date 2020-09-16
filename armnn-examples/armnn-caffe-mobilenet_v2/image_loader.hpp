//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//

#pragma once

constexpr int g_kTestImageWidth = 224;
constexpr int g_kTestImageHeight = 224;
constexpr int g_kTestImageSize = g_kTestImageWidth * g_kTestImageHeight;
constexpr int g_kTestImageByteSize = 3 * g_kTestImageSize;
constexpr int g_color_order = 0; // 0:BGR, 1: RGB
constexpr float g_pixel_mean_R = 123.68;
constexpr float g_pixel_mean_G = 116.78;
constexpr float g_pixel_mean_B = 103.94;
constexpr float g_image_scale = 255.0;
constexpr float g_input_scale = 4.335;

unsigned char bmp_header[54] = {
0x42, 0x4d, // magic identifier = 'B', 'M'
0, 0, 0, 0, // file size in bytes
0, 0, // reserved 1
0, 0, // reserved 2
54, 0, 0, 0, // offset to image data, bytes 
40, 0, 0, 0, // info Header size in bytes 
0, 0, 0, 0, // width of image 
0, 0, 0, 0, // height of image 
1, 0, // number of colour planes 
24, 0, // bits per pixel = 24 bits (BGR)
0, 0, 0, 0, // compression type 
0, 0, 0, 0, // image size in bytes 
0, 0, 0, 0, // pixels per meter : xresolution
0, 0, 0, 0, // pixels per meter : yresolution 
0, 0, 0, 0, // number of colours 
0, 0, 0, 0, // important colours 
};

// Helper struct for loading test data
struct TestImage
{
    unsigned int label;
    float image[g_kTestImageByteSize];
};

// Load a single test image from the BMP file
std::unique_ptr<TestImage> loadTestImage(std::string imagePath, unsigned int imageLabel)
{
    std::vector<unsigned char> I(g_kTestImageByteSize);

    std::ifstream imageStream(imagePath, std::ios::binary);

    if (!imageStream.is_open())
    {
        std::cerr << "Failed to load " << imagePath << std::endl;
        return nullptr;
    }

    int row, col;

    // check the BMP file has the correct header
    imageStream.read(reinterpret_cast<char*>(bmp_header), sizeof(bmp_header));
    col = (int)bmp_header[18] | (int)bmp_header[19] << 8 | (int)bmp_header[20] << 16 | (int)bmp_header[21] << 24;
    row = (int)bmp_header[22] | (int)bmp_header[23] << 8 | (int)bmp_header[24] << 16 | (int)bmp_header[25] << 24;
    if (bmp_header[0] != 0x42 || bmp_header[1] != 0x4d)
    {
        std::cerr << "Failed to read " << imagePath << ": magic identifier != 'B', 'M'" << std::endl;
        return nullptr;
    }
    else if (col != g_kTestImageWidth || row != g_kTestImageHeight)
    {
        std::cerr << "Failed to read " << imagePath << ": width or height is not correct" << std::endl;
        std::cout << "Width of image: " << col  << std::endl;
        std::cout << "Height of image: " << row << std::endl;
        return nullptr;
    }
    else if (bmp_header[28] != 24)
    {
        std::cerr << "Failed to read " << imagePath << ":  bits per pixel != 24 bits (BGR)" << std::endl;
        return nullptr;
    }

    // read image
    imageStream.read(reinterpret_cast<char*>(&I[0]), g_kTestImageByteSize);

    if (!imageStream.good())
    {
        std::cerr << "Failed to read " << imagePath << std::endl;
        return nullptr;
    }

    // store image and label in TestImage
    std::unique_ptr<TestImage> ret(new TestImage);
    ret->label = imageLabel;

    unsigned int j = 0, k = 0;
    for (unsigned int i = 0; i < col * row; ++i)
    {
        if(g_color_order) // RGB
        {
            ret->image[g_kTestImageSize*0+i] = ((static_cast<float>(I[j+2]) - g_pixel_mean_R) * g_input_scale) / g_image_scale;
            ret->image[g_kTestImageSize*1+i] = ((static_cast<float>(I[j+1]) - g_pixel_mean_G) * g_input_scale) / g_image_scale;
            ret->image[g_kTestImageSize*2+i] = ((static_cast<float>(I[j+0]) - g_pixel_mean_B) * g_input_scale) / g_image_scale;
    	}
       else // BGR
        {
            ret->image[g_kTestImageSize*0+i] = ((static_cast<float>(I[j+0]) - g_pixel_mean_B) * g_input_scale) / g_image_scale;
            ret->image[g_kTestImageSize*1+i] = ((static_cast<float>(I[j+1]) - g_pixel_mean_G) * g_input_scale) / g_image_scale;
            ret->image[g_kTestImageSize*2+i] = ((static_cast<float>(I[j+2]) - g_pixel_mean_R) * g_input_scale) / g_image_scale;
        }
        j += 3;
    }
    std::cout << "ret->image[0]: " << ret->image[0] << std::endl;
    std::cout << "ret->image[1]: " << ret->image[1] << std::endl;
    std::cout << "ret->image[g_kTestImageSize]: " << ret->image[g_kTestImageSize] << std::endl;
    std::cout << "ret->image[g_kTestImageSize+1]: " << ret->image[g_kTestImageSize+1] << std::endl;
    std::cout << "ret->image[g_kTestImageSize*2]: " << ret->image[g_kTestImageSize*2] << std::endl;
    std::cout << "ret->image[g_kTestImageSize*2+1]: " << ret->image[g_kTestImageSize*2+1] << std::endl;

    return ret;
}
