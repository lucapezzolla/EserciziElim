#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

Mat correlazione(Mat img, Mat mask);
Mat convoluzione(Mat img, Mat mask);
Mat laplaciano(Mat img, int angolo);
Mat sobel(Mat img);


Mat correlazione(Mat img, Mat mask) {

Mat result = Mat::ones(img.rows-mask.rows+1, img.cols-mask.cols+1, CV_8UC1);

for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
        float sum = 0;
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
                sum += img.at<uchar>(i + k, j + l) * mask.at<float>(1-k,1-l);
            }
        }
        result.at<uchar>(i, j) = sum;
    }
}


return result;


}



Mat convoluzione(Mat img, Mat mask) {

Mat result(img.rows-mask.rows+1, img.cols-mask.cols+1, CV_8UC1);

rotate(mask,mask,ROTATE_180);

for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
    float sum = 0.0;
        for (int k = -1; k <= 1; k++) {
            for (int l = -1; l <= 1; l++) {
                sum += img.at<uchar>(i + k, j + l) * mask.at<float>(1-k,1-l);
            }
        }
        result.at<uchar>(i, j) = sum;
    }
}


return result;


}


Mat laplaciano(Mat img, int angolo) {

float data[3][3];

if (angolo == 45) {
    data[0][0] = 1;
    data[0][1] = 1;
    data[0][2] = 1;
    data[1][0] = 1;
    data[1][1] = -8;
    data[1][2] = 1;
    data[2][0] = 1;
    data[2][1] = 1;
    data[2][2] = 1;
}



else if(angolo == 90) {

    data[0][0] = 0;
    data[0][1] = 1;
    data[0][2] = 0;
    data[1][0] = 1;
    data[1][1] = -4;
    data[1][2] = 1;
    data[2][0] = 0;
    data[2][1] = 1;
    data[2][2] = 0;

}

else {

printf("Angolo inserito non valido\n");
exit(-1);

}

Mat mask(3, 3, CV_32FC1, data);
Mat result(img.rows, img.cols, CV_8UC1);
result = convoluzione(img,mask);

normalize(result, result, 0, 255, NORM_MINMAX, CV_8U);


return result;


}

Mat sobel(Mat img) {

    float gx[] = {-1,-2,-1,0,0,0,1,2,1};
    float gy[] = {-1,0,1,-2,0,2,-1,0,1};

    Mat maskx(3, 3, CV_32FC1, gx);
    Mat masky(3, 3, CV_32FC1, gy);

    Mat grad_x, grad_y;
    Mat result(img.rows, img.cols, CV_8UC1);

    grad_x = convoluzione(img, maskx);
    grad_y = convoluzione(img, masky);
    result = grad_x + grad_y;
    normalize(result, result, 0, 255, NORM_MINMAX, CV_8U);


    return result;
}


int main(int argc, char ** argv) {


if(argc < 2) {

cout<<"Errore nel caricare l'immagine"<<endl;
exit(-1);

}

Mat img = imread(argv[1], IMREAD_GRAYSCALE);
Mat mask = Mat::ones(3, 3, CV_32FC1) * (1.0 / 9.0);
Mat imgpadded;
copyMakeBorder(img,imgpadded,1,1,1,1,img.type(),Scalar(0));


Mat result = correlazione(imgpadded,mask);
Mat result2 = convoluzione(imgpadded,mask);
Mat result3;
filter2D(imgpadded,result3,CV_8UC1,mask,Point(-1,1),0,BORDER_DEFAULT);
Mat result4 = laplaciano(imgpadded,45);
Mat result5;
Laplacian(imgpadded,result5,CV_8UC1,3);
Mat result6;
Sobel(imgpadded,result6,CV_32FC1,1,1,3);
Mat result7 = sobel(imgpadded);


imshow("Immagine dopo correlazione", result);
waitKey(0);
imshow("Immagine dopo convoluzione", result2);
waitKey(0);
imshow("Immagine dopo filter2D", result3);
waitKey(0);
imshow("Immagine dopo filtro Laplaciano", result4);
waitKey(0);
imshow("Immagine dopo funzione Laplaciano", result5);
waitKey(0);
imshow("Immagine dopo funzione Sobel", result6);
waitKey(0);
imshow("Immagine dopo filtro Sobel", result7);
waitKey(0);


return 0;

}
