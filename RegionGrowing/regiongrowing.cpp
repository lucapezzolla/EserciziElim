#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>
using namespace std;
using namespace cv;

const int th = 200;
const double min_region_area_factor = 0.01;

void regionGrowing(Mat &img, Point seed, Mat &mask, Mat &dest, int th) {

stack<Point> region;
region.push(seed);
mask.at<uchar>(seed) = 1;

while(!region.empty()) {
Point actual = region.top();
region.pop();

for(int i = -1; i <= 1; i++) {
  for(int j = -1; j <= 1; j++) {
   Point possible(actual.x+i,actual.y+j);
    if(possible == actual)
     continue;
    if(possible.x >= 0 && possible.x <= img.cols && possible.y >= 0 && possible.y <= img.rows) {
     int delta = pow(img.at<Vec3b>(actual)[0] - img.at<Vec3b>(possible)[0],2) + pow(img.at<Vec3b>(actual)[1] - img.at<Vec3b>(possible)[1],2) + pow(img.at<Vec3b>(actual)[2] - img.at<Vec3b>(possible)[2],2);
     if(delta < th && mask.at<uchar>(possible) == 0 && dest.at<uchar>(possible) == 0) {
      mask.at<uchar>(possible) = 1;
      region.push(possible);
     }
    }
   }
  }
 }
}





int main(int argc, char **argv) {

Mat img = imread(argv[1],IMREAD_COLOR);

imshow("Immagine originale",img);
waitKey(0);

if(img.empty()) {

cout<<"Errore nel caricamento dell'immagine"<<endl;
exit(-1);

}

Mat mask = Mat::zeros(img.size(),CV_8U);
Mat dest = Mat::zeros(img.size(),CV_8U);
int min_region_area = (int) (min_region_area_factor * img.rows * img.cols);
uchar padding = 1;

for(int i = 0; i < img.rows; i++) {
  for(int j = 0; j < img.cols; j++) {
   if(dest.at<uchar>(i,j) == 0) {
    regionGrowing(img,Point(j,i),mask,dest,th);

    int area = (int)sum(mask).val[0];

    if(area > min_region_area) {
    dest += mask*padding;
    imshow("Regione",mask*255);
    waitKey(0);
    padding++;
    }

    else dest += mask*255;

    mask.setTo(0);

    }
  }
}

return 0;

}
