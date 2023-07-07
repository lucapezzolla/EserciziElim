#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


void Otsu(Mat img);

void Otsu(Mat img) {


float histogram[256] = {0};

//calcolo istogramma
for(int i = 0; i < img.rows; i++) {
 for(int j = 0; j < img.cols; j++) {
  histogram[img.at<uchar>(i,j)]++;
 }
}


float totalPix = img.rows*img.cols;
float sigmaMax = 0.0;
int optimalTh = 0;


for(int k = 0; k < 256; k++) {

float p1 = 0.0, p2 = 0.0;
float m1 = 0.0, m2 = 0.0;


//probabilità che un pixel appartenga alla classe 1

for(int i = 1; i <= k; i++) {

p1 += histogram[i]/totalPix;


}

//probabilità che un pixel appartenga alla classe 2
p2 = 1.0-p1;


//media cumulativa classe 1
for(int i = 1; i <= k; i++) {

m1 += (i * histogram[i]/p1);

}

//media cumulativa classe 2
for(int i = k+1; i < 256; i++) {

m2 += (i * histogram[i]/p2);

}

//calcolo sigma (P1P2(m1-m2)^2)
float sigma = p1 * p2 * ((m1 - m2) * (m1 - m2));

if(sigma > sigmaMax) {
    sigmaMax = sigma;
    optimalTh = k;
 }
}

printf("%d",optimalTh);

Mat ot1;
threshold(img,ot1,optimalTh,255,THRESH_BINARY);
imshow("Immagine con sogliatura di Otsu",ot1);
waitKey(0);


}





int main(int argc, char **argv) {

if(argc < 2)  {

printf("L'immagine non è stata caricata correttamente");
exit(-1);

}

Mat img = imread(argv[1],IMREAD_GRAYSCALE);
filter2D(img,img,CV_8UC1,getGaussianKernel(3,1));
Otsu(img);



return 0;

}
