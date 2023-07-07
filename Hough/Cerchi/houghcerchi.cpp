#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


void Houghcerchi(Mat img, int rmin, int rmax, int th) {

int sizes[] = {2*img.rows,2*img.cols,rmax-rmin+1};
Mat acc(3,sizes,CV_8UC1);

GaussianBlur(img,img,Size(3,3),0);
Mat canny;
Canny(img,canny,50,80,3);

int a, b;

for(int i = 0; i < canny.rows; i++) {
  for(int j = 0; j < canny.cols; j++) {
   if(canny.at<uchar>(i,j) == 255) {
    for(int theta = 0; theta <= 360; theta++) {
     for(int r = rmin; r <= rmax; r++) {
     a = j - r*cos(theta * (CV_PI/180)) + img.cols;
     b = i - r*sin(theta * (CV_PI/180)) + img.rows;
     if(a > 0 && a < 2*img.cols && b > 0 && b < 2*img.rows)
     acc.at<uchar>(cvRound(b),cvRound(a),r)++;
     }
    }
   }
  }
}

int x, y;

for(int i = 0; i < 2*img.rows; i++) {
 for(int j = 0; j < 2*img.cols; j++) {
  for(int r = rmin; r <= rmax; r++) {
  if(acc.at<uchar>(i,j,r) > th) {
   x = j - img.cols;
   y = i - img.rows;
   circle(img,Point(x,y),r,Scalar(0,0,255),2,LINE_AA);
   }
  }
 }
}

imshow("Hough personale cerchi",img);
waitKey(0);


}






int main(int argc, char ** argv) {

Mat img = imread(argv[1],IMREAD_COLOR);

if(img.empty()) {

cout<<"L'immagine non è stata caricata correttamente"<<endl;
exit(-1);

}

Houghcerchi(img,20,100,150);



return 0;
}
