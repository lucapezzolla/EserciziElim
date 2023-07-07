#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


void myCanny(Mat img, int low, int high) {

filter2D(img,img,CV_32FC1,getGaussianKernel(3,1));
Mat dx, dy;
Sobel(img,dx,CV_32F,1,0);
Sobel(img,dy,CV_32F,0,1);

Mat mag = abs(dx) + abs(dy);
normalize(mag,mag,0,255,NORM_MINMAX,CV_32FC1);
Mat angle;
phase(dy,dx,angle,true);
normalize(angle,angle,-180,180,NORM_MINMAX,CV_32FC1);
float p,q;

//no maxima suppression
for(int i = 0; i < mag.rows; i++) {
  for(int j = 0; j < mag.cols; j++) {
     if((angle.at<float>(i,j) >= -22.5 && angle.at<float>(i,j) <= 22.5) || (angle.at<float>(i,j) >= -157.5 && angle.at<float>(i,j) <= 157.5)) {
      p = mag.at<float>(i,j-1);
      q = mag.at<float>(i,j+1);
     }
     else if((angle.at<float>(i,j) > 22.5 && angle.at<float>(i,j) <= 67.5) || (angle.at<float>(i,j) > -157.5 && angle.at<float>(i,j) <= -112.5)) {
      p = mag.at<float>(i-1,j+1);
      q = mag.at<float>(i+1,j-1);
     }
     else if((angle.at<float>(i,j) > 67.5 && angle.at<float>(i,j) <= 112.5) || (angle.at<float>(i,j) > -112.5 && angle.at<float>(i,j) <= -67.5)) {
      p = mag.at<float>(i-1,j);
      q = mag.at<float>(i+1,j);
     }
     else if((angle.at<float>(i,j) > 112.5 && angle.at<float>(i,j) < 157.5) || (angle.at<float>(i,j) > -67.5 && angle.at<float>(i,j) < -22.5)) {
      p = mag.at<float>(i-1,j-1);
      q = mag.at<float>(i+1,j+1);
     }

     if(mag.at<float>(i,j) < p || mag.at<float>(i,j) < q) {
      mag.at<float>(i,j) = 0;
     }
 }
}

imshow("NO maxima suppression",mag);
waitKey(0);

// Applicazione della soglia
for (int i = 1; i < mag.rows-1; i++) {
  for (int j = 1; j < mag.cols-1; j++) {
    if (mag.at<float>(i, j) > high)
      mag.at<float>(i, j) = 255;
     else if (mag.at<float>(i, j) < low)
       mag.at<float>(i, j) = 0;
      else if (mag.at<float>(i,j) < high && mag.at<float>(i,j) > low) {
        bool strong = false;
       for (int k = -1; k <= 1; k++) {
         for (int l = -1; l <= 1; l++) {
           if (mag.at<float>(i + k, j + l) == 255)
            strong = true;
        }
      }
       if(strong)
         mag.at<float>(i,j) = 255;
         else
          mag.at<float>(i,j) = 0;
    }
  }
}




imshow("Canny personale",mag);
waitKey(0);


}




int main(int argc, char **argv) {


Mat img = imread(argv[1],IMREAD_GRAYSCALE);

if(img.empty()) {

cout<<"Immagine non caricata correttamente"<<endl;

}

imshow("Immagine originale",img);
waitKey(0);

myCanny(img,35,52);





return 0;

}
