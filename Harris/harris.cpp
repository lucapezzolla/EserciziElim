#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void Harris(Mat img, int th) {

Mat dx, dy;
Sobel(img,dx,CV_32F,1,0);
Sobel(img,dy,CV_32F,0,1);

Mat dx2, dy2, dxy;
multiply(dx,dy,dxy);
pow(dx,2,dx2);
pow(dy,2,dy2);


GaussianBlur(dx2,dx2,Size(3,3),0);
GaussianBlur(dy2,dy2,Size(3,3),0);
GaussianBlur(dxy,dxy,Size(3,3),0);

Mat det, track;
Mat res1, res2;
multiply(dx2,dy2,res1);
pow(dxy,2,res2);
det = res1-res2;
track = dx2+dy2;
pow(track,2,track);
Mat r = det - 0.04 * track;
Mat dest;
normalize(r,dest,0,255,NORM_MINMAX,CV_32F);

for(int i = 0; i < dest.rows; i++) {
 for(int j = 0; j < dest.cols; j++) {
  if(dest.at<float>(i,j) > th)
  circle(img,Point(j,i),3,Scalar(0),1,LINE_AA);
 }
}


imshow("Harris personale",img);
waitKey(0);



}





int main(int argc, char **argv) {


Mat img = imread(argv[1],IMREAD_GRAYSCALE);

if(img.empty()) {

cout<<"Immagine non caricata correttamente"<<endl;

}

imshow("Immagine originale",img);
waitKey(0);


Harris(img,65);




return 0;

}
