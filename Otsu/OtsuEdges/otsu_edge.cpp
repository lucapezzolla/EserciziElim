#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void Otsu_edge(Mat img) {


GaussianBlur(img,img,Size(5,5),0);
Mat canny;
Canny(img,canny,200,240,3);
imshow("Immagine sogliata con edge",canny);
waitKey(0);

float histogram[256] = {0};
for(int i = 0; i < canny.rows; i++) {
 for(int j = 0; j < canny.cols; j++) {
  if(canny.at<uchar>(i,j) == 255)
   histogram[img.at<uchar>(i,j)]++;
 }
}

float sigmaMax = 0.0;
int thMax;
float totalPix = canny.rows*canny.cols;


for(int k = 0; k <= 255; k++) {

float p1 = 0.0, p2 = 0.0;
float m1 = 0.0, m2 = 0.0;


for(int i = 1; i <= k; i++) {

p1 += histogram[i]/totalPix;

}

p2 = 1 - p1;

for(int i = 1; i <= k; i++) {

m1 += i*histogram[i]/p1;

}

for(int i = k+1; i < 256; i++) {

m2 += i*histogram[i]/p2;

}

float sigma = p1*p2*((m1-m2) * (m1*m2));

if(sigma > sigmaMax) {

sigmaMax = sigma;
thMax = k;
}

}

threshold(img,img,thMax,255,THRESH_BINARY);

imshow("Otsu finale",img);
waitKey(0);


}




int main(int argc, char** argv) {


Mat img = imread(argv[1],IMREAD_GRAYSCALE);

if(img.empty()) {


cout<<"Immagine non caricata correttamente"<<endl;
exit(-1);

}


Otsu_edge(img);




return 0;
}
