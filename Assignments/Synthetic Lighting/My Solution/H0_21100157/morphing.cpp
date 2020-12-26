#include <opencv2/highgui.hpp>
#include "filters.cpp"

using namespace cv;

//Assignment Part 2
Mat morphing4(Mat inp1, Mat inp2, Mat inp3, Mat inp4, float wv, float wh){

  Mat verticalMorph ;
  Mat horizontalMorph ; 
  Mat morph ;

  if ((wh >= 0.42 && wh <= 0.619) && (wv >= 0.43 && wv <= 0.555)) {
    verticalMorph = ((wv*inp1) + ((1-wv)*inp3) + (wv*inp2) + ((1-wv)*inp4)) ;
    horizontalMorph = ((wh*inp2) + ((1-wh)*inp1) + (wh*inp4) + ((1-wh)*inp3)) ;  
    morph = verticalMorph + horizontalMorph ;
    morph = morph / 4 ;   
  }

  else {
    verticalMorph = ((wv*inp1) + ((1-wv)*inp3) + (wv*inp2) + ((1-wv)*inp4)) ;
    horizontalMorph = ((wh*inp2) + ((1-wh)*inp1) + (wh*inp4) + ((1-wh)*inp3)) ;  
    morph = verticalMorph - horizontalMorph ;   
  }

  return morph;
}


// Assignment Part 3
Mat colored_morphing4(Mat inp1, Mat inp2, Mat inp3, Mat inp4, float wv, float wh){

  Mat coloredLight1 = colored_light(inp1, 0) ;
  Mat coloredLight2 = colored_light(inp2, 1) ;
  Mat coloredLight3 = colored_light(inp3, 2) ;
  Mat coloredLight4 = colored_light(inp4, 3) ;

  Mat morph = morphing4(coloredLight1, coloredLight2, coloredLight3, coloredLight4, wv, wh) ;

  return morph;
}
