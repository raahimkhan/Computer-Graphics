#include <opencv2/highgui.hpp>

using namespace cv;

//Part2:
Mat color_enhancement_filter(Mat image, int color){

  //Setting up the loop bounds 
  int x_max = image.cols;
  int y_max = image.rows;

  //looping over the entire image
  for (int x=0; x<x_max; x++){
        for (int y=0; y<y_max; y++){

        //Retrieve a single pixel from the image
        Vec3b pixel = image.at<Vec3b>(Point(x,y));

        //perform the required operation here
        /////////////////////////////////////


        /////////////////////////////////////

        //Save the pixel back into the image
        image.at<Vec3b>(Point(x,y)) = pixel;
  
    }
  }

  return image;

}

//Part3:
Mat colored_light(Mat image, int color){


  return image;


}