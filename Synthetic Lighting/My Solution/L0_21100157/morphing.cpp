#include <opencv2/highgui.hpp>
#include "filters.cpp"

using namespace cv;


//Part 1: 
Mat morphing(Mat inp1, Mat inp2, float w){

  Mat morph = (w*inp1) + ((1-w)*inp2) ; 

  return morph;
}

//Part 4:
Mat filtered_morphing(Mat inp1, Mat inp2, float w){

  Mat morph = (w*colored_light(inp1, 2)) + ((1-w)*colored_light(inp2, 0)) ;

  return morph;
}


