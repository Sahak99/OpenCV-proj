//
//  main.cpp
//  OpenCV-proj
//
//  Created by Code Republic on 24.02.22.
//

#include "/Users/coderepublic/Desktop/OpenCV-proj/OpenCV-proj/Filter.hpp"

int main()
{
    std::string path = "/Users/coderepublic/Desktop/OpenCV-proj/Resources/car.jpeg";
    
    Filter f(path);
    
    f.kelvinFilter();
    f.showImages();

    f.pencilSketchFilter();
    f.showImages();

    f.moonFilter();
    f.showImages();

    f.cartoonFilter();
    f.showImages();

    f.clarendonFilter();
    f.showImages();
    
    return 0;
}
