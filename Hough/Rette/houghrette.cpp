#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;



void Houghrette(Mat img, int th) {

int rhomax = hypot(img.rows,img.cols);
Mat acc = Mat::zeros(2*rhomax,180,CV_8U);

GaussianBlur(img,img,Size(3,3),0);
Mat canny;
Canny(img,canny,50,80,3);

int rho;

for(int i = 0; i < canny.rows; i++) {
 for(int j = 0; j < canny.cols; j++) {
  if(canny.at<uchar>(i,j) == 255) {
  for(int theta = 0; theta <= 180; theta++) {
   rho = j*cos(theta) + i*sin(theta) + rhomax;
   acc.at<uchar>(rho,theta)++;
   }
  }
 }
}

int x, y;

for(int i = 0; i < acc.rows; i++) {
 for(int j = 0; j < acc.cols; j++) {
  if(acc.at<uchar>(i,j) > th) {
  rho = i - rhomax;
  x = rho*cos(j);
  y = rho*sin(j);
  Point pt1(x + 1000 * (-sin(j)), y + 1000 * cos(j));
  Point pt2(x - 1000 * (-sin(j)), y - 1000 * cos(j));
  line(img,pt1,pt2,Scalar(0,0,255),2,LINE_AA);
  }
 }
}


imshow("Hough rette personale",img);
waitKey(0);




}



int main(int argc, char ** argv) {

Mat img = imread(argv[1],IMREAD_COLOR);

if(img.empty()) {

cout<<"L'immagine non è stata caricata correttamente"<<endl;
exit(-1);

}

Houghrette(img,125);



return 0;
}
