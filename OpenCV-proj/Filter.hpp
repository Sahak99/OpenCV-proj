//
//  Filter.hpp
//  OpenCV-proj
//
//  Created by Code Republic on 24.02.22.
//

#ifndef Filter_hpp
#define Filter_hpp

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <iostream>
#include <string>
#include <vector>

class Filter
{
public:
    Filter(std::string);
    ~Filter();
public:
    
    void setImg(cv::Mat);
    cv::Mat getImg();
    
    void pencilSketchFilter();
    void kelvinFilter();
    void moonFilter();
    void cartoonFilter();
    void clarendonFilter();
    
    void showImages();
    static void interpolation(uchar *lut, float *curve, float *originalValue);
    
private:
    cv::Mat m_originalImg;
    cv::Mat m_filteredImg;
};


#endif /* Filter_hpp */
