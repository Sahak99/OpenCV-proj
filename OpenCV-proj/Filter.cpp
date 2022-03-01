//
//  Filter.cpp
//  OpenCV-proj
//
//  Created by Code Republic on 24.02.22.
//

#include "/Users/coderepublic/Desktop/OpenCV-proj/OpenCV-proj/Filter.hpp"
//ctors and dtor
Filter::Filter(std::string path)
{
    m_originalImg = cv::imread(path);
    
    if(m_originalImg.empty())
    {
        perror("Could not read the image\n");
    }
}

Filter::~Filter()
{
    
}

//filter functions
void Filter::setImg(cv::Mat original)
{
    // Error handling TBD
    if(!original.empty())
    m_originalImg = original;
    else {
        perror("Cannot read the image\n");
    }
}

cv::Mat Filter::getImg()
{
    if(!m_filteredImg.empty())
    {
        return m_filteredImg;
    }
    
    return m_originalImg;
}

void Filter::pencilSketchFilter()
{
    cv::Mat output;
    // convert to gray scale
    cv::cvtColor(m_originalImg, output, cv::COLOR_BGR2GRAY);
    
    //apply gaussian blur
    
    cv::GaussianBlur(output, output, cv::Size(3,3), 0);
    
    //detect edges
    
    cv::Laplacian(output, output, -1, 5);
    
    // invert the binary image
    
    output = 255 - output;
    
    //binary thresholding
    
    cv::threshold(output, output, 150, 255, cv::THRESH_BINARY);
    
    m_filteredImg = output.clone();
    
   
}

void Filter::showImages()
{
    // create window to display images
    
    cv::namedWindow("Petros", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Filtrac Petros", cv::WINDOW_AUTOSIZE);
    
    //display images
    
    cv::imshow("Petros", m_originalImg);
    cv::imshow("Filtrac Petros", m_filteredImg);
    
    cv::waitKey(0);
    
    cv::destroyAllWindows();
}

// helper functions

void Filter::interpolation(uchar *lut, float *curve, float *originalValue)
{
    for (int i = 0; i < 256; ++i)
    {
        int j = 0;
        float a = i;
        
        while(a > originalValue[j])
        {
            j++;
        }
        
        if (a == originalValue[j]) {
            lut[i] = curve[j];
            continue;
        }
        
        float slope = ((float)(curve[j] - curve[j-1])) /
        ((float)(originalValue[j] - originalValue[j - 1]));
        
        float constant = curve[j] - slope * originalValue[j];
        lut[i] = slope * a + constant;
    }
}

void Filter::kelvinFilter()
{
    cv::Mat output;
    
    output = m_originalImg.clone();
    
    std::vector<cv::Mat> channels;
    split(output, channels);
    
    // Interpolating values
    
    float redValuesOriginal[] = {0, 60, 110, 150, 235, 255};
    float redValues[] =         {0, 102, 185, 220, 245, 245};
    float greenValuesOriginal[] = {0, 68, 105, 190, 255};
    float greenValues[] =         {0, 68, 120, 220, 255};
    float blueValuesOriginal[] = {0, 88, 145, 185, 255};
    float blueValues[] =         {0, 12, 140, 212, 255};
    
    //create look up table
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar *lut = lookupTable.ptr();
    
    //Apply interpolation and create lookup table
    interpolation(lut, blueValues, blueValuesOriginal);
    //Apply mapping for blue channel
    cv::LUT(channels[0], lookupTable, channels[0]);
    
    //Apply interpolation and create lookup table
    interpolation(lut, greenValues, greenValuesOriginal);
    //Apply mapping for green channel
    cv::LUT(channels[1], lookupTable, channels[1]);
    
    //Apply interpolation and create lookup table
    interpolation(lut, redValues, redValuesOriginal);
    //Apply mapping for red channel
    cv::LUT(channels[2], lookupTable, channels[2]);
    
    cv::merge(channels, output);
    
    m_filteredImg = output.clone();
}

void Filter::moonFilter()
{
    cv::Mat output;
    
    cv::cvtColor(m_originalImg, output, cv::COLOR_BGR2Lab);
    
    std::vector<cv::Mat> channels;
    split(output, channels);
    
    float originalValues[] = {0, 15, 30, 50, 70, 90, 120, 160, 180, 210, 255 };
    float values[]=          {0, 0, 5, 15, 60, 110, 150, 190, 210, 230, 255 };
    
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar *lut = lookupTable.ptr();
    
    interpolation(lut, values, originalValues);
    
    cv::LUT(channels[0], lookupTable, channels[0]);
    
    cv::merge(channels, output);
    
    cv::cvtColor(output, output, cv::COLOR_Lab2BGR);
    
    cv::cvtColor(output, output, cv::COLOR_BGR2HSV);
    
    split(output, channels);
    
    channels[1] = channels[0] * 0.01;
    
    cv::min(channels[1], 255, channels[1]);
    cv::max(channels[1], 0, channels[1]);
    
    merge(channels, output);
    
    cv::cvtColor(output, output, cv::COLOR_HSV2BGR);
    
    m_filteredImg = output.clone();
}

void Filter::cartoonFilter()
{
    cv::Mat grayImg, edgeImage, edgePresImg, output;
    
    cv::cvtColor(m_originalImg, grayImg, cv::COLOR_BGR2GRAY);
    
    cv::GaussianBlur(grayImg, grayImg, cv::Size(3, 3), 0);
    
    cv::Laplacian(grayImg, edgeImage, -1, 5);
    
    cv::convertScaleAbs(edgeImage, edgeImage);
    
    edgeImage = 255 - edgeImage;
    
    cv::threshold(edgeImage, edgeImage, 150, 255, cv::THRESH_BINARY);
    
    cv::edgePreservingFilter(m_originalImg, edgePresImg, 2, 50, 0.4);
    
    output = cv::Scalar::all(0);
    
    cv::bitwise_and(edgePresImg, edgePresImg, output, edgeImage);
    
    m_filteredImg = output.clone();
    
}

void Filter::clarendonFilter()
{
    cv::Mat output;
    
    float originValues[] = {0, 28, 56, 85, 113, 141, 170, 198, 227, 255};
    float blueValues[] =   {0, 38, 66, 104, 139, 175, 206, 226, 245 , 255};
    float redValues[] =    {0, 16, 35, 64, 117, 163, 200, 222, 237, 249};
    float greenValues[] =  {0, 24, 49, 98, 141, 174, 201, 223, 239, 255};
    
    output = m_originalImg.clone();
    
    std::vector<cv::Mat> channels;
    split(output, channels);
    
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar *lut = lookupTable.ptr();
    
    interpolation(lut, blueValues, originValues);
    cv::LUT(channels[0], lookupTable, channels[0]);
    
    interpolation(lut, greenValues, originValues);
    cv::LUT(channels[1], lookupTable, channels[1]);
    
    interpolation(lut, redValues, originValues);
    cv::LUT(channels[2], lookupTable, channels[2]);
    
    cv::merge(channels, output);
    
    m_filteredImg = output.clone();
    
}
