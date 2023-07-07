#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;



void multiOtsu(Mat img) {

float sigma[256][256] = {0.0};
float histogram[256] = {0.0};

for(int i = 0; i < img.rows; i++) {
 for(int j = 0; j < img.cols; j++) {
  histogram[img.at<uchar>(i,j)]++;
 }
}

float mg = 0.0, sigmaMax = 0.0;
int th1, th2;
int totalPix = img.rows*img.cols;


//calcolo media cumulativa su tutta l'immagine
for(int i = 0; i < 256; i++)
mg += i*histogram[i]/totalPix;

for(int k1 = 0; k1 < 256; k1++) {
 for(int k2 = k1+1; k2 < 256; k2++) {

 float p1 = 0.0, p2 = 0.0, p3 = 0.0;
 float m1 = 0.0, m2 = 0.0, m3 = 0.0;

 for(int i = 0; i <= k1; i++) {

 p1 += histogram[i]/totalPix;

 }

 for(int i = k1 + 1; i <= k2; i++) {

 p2 += histogram[i]/totalPix;

 }


 for(int i = k2; i <= 256; i++) {

 p3 += histogram[i]/totalPix;

 }

 for(int i = 1; i <= k1; i++) {

 m1 += i*histogram[i]/p1;

 }

 for(int i = k1 + 1; i <= k2; i++) {

 m2 += i*histogram[i]/p2;

 }


 for(int i = k2; i <= 256; i++) {

 m3 += i*histogram[i]/p3;

 }


 sigma[k1][k2] = p1*((m1-mg)*(m1-mg)) + p2*((m2-mg)*(m2-mg)) + p3*((m3-mg)*(m3-mg));

 if(sigma[k1][k2] > sigmaMax) {

 sigmaMax = sigma[k1][k2];
 th1 = k1;
 th2 = k2;
  }

 }
}
   for(int i = 0; i < img.rows; i++) {
    for(int j = 0; j < img.cols; j++) {
     if(img.at<uchar>(i,j)  >= th2)
      img.at<uchar>(i,j) = 255;
      else if(img.at<uchar>(i,j) < th2 && img.at<uchar>(i,j) >= th1)
       img.at<uchar>(i,j) = 127;
      else
      img.at<uchar>(i,j) = 0;
    }
   }

   cout<<th1<<endl;
   cout<<th2<<endl;

imshow("Otsu soglie multiple",img);
waitKey(0);

}




int main(int argc, char **argv) {

if(argc < 2)  {

cout<<"L'immagine non è stata caricata correttamente"<<endl;
exit(-1);

}

Mat img = imread(argv[1],IMREAD_GRAYSCALE);
GaussianBlur(img,img,Size(3,3),0);
multiOtsu(img);



return 0;

}
