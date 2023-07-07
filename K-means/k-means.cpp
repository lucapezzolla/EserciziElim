#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
using namespace std;
using namespace cv;

vector<Scalar> center;
vector<vector<Point>> cluster;
const int th = 0.1;


void initialize_cluster(Mat img, int n) {

for(int i = 0; i < n; i++) {

center.push_back(img.at<Vec3b>(rand()%img.rows,rand()%img.cols));

vector<Point> vec;
cluster.push_back(vec);

}

}

double compute_distance(Scalar a, Scalar b) {

double diff = pow(a[0]-b[0],2) + pow(a[1]-b[1],2) + pow(a[2]-b[2],2);

return sqrt(diff);


}


void find_cluster(Mat img, int n) {


for(int i = 0; i < img.rows; i++) {
 for(int j = 0; j < img.cols; j++) {
 double dist = 0, min_dist = INFINITY;
 int index;
  for(int k = 0; k < n; k++) {
   dist = compute_distance(center[k],img.at<Vec3b>(i,j));

   if(dist < min_dist) {
    min_dist = dist;
    index = k;
   }
  }
    cluster.at(index).push_back(Point(j,i));
  }
 }
}

double calculate_center(Mat img, int n, double &oldC, double &newC) {

double Bdiff = 0, Gdiff = 0, Rdiff = 0, diff = 0;
for(int i = 0; i < n; i++) {
 for(int j = 0; j < cluster.at(i).size(); j++) {

  Bdiff += img.at<Vec3b>(cluster.at(i).at(j))[0];
  Gdiff += img.at<Vec3b>(cluster.at(i).at(j))[1];
  Rdiff += img.at<Vec3b>(cluster.at(i).at(j))[2];
 }

 Bdiff /= cluster.at(i).size();
 Gdiff /= cluster.at(i).size();
 Rdiff /= cluster.at(i).size();

 //newC += compute_distance(oldC,Scalar(Bdiff,Gdiff,Rdiff));
 center.push_back(Scalar(Bdiff,Gdiff,Rdiff));

}

  //newC /= n;
  diff = abs(oldC-newC);
  oldC = newC;

 return diff;


}



void show_output(Mat img, int n) {

for(int i = 0; i < n; i++) {
 Scalar color(rand()%255,rand()%255,rand()%255);
 for(int j = 0; j < cluster.at(i).size(); j++) {
 img.at<Vec3b>(cluster.at(i).at(j))[0] = color[0];
 img.at<Vec3b>(cluster.at(i).at(j))[1] = color[1];
 img.at<Vec3b>(cluster.at(i).at(j))[2] = color[2];
 }
}

 imshow("Immagine clusterizzata",img);
 waitKey(0);


}




int main(int argc, char **argv) {

srand(time(NULL));

Mat img = imread(argv[1],IMREAD_COLOR);

if(img.empty()) {

cout<<"Immagine non caricata correttamente"<<endl;
exit(-1);

}

int n = atoi(argv[2]);
imshow("Immagine originale",img);
waitKey(0);

double oldC = INFINITY, newC = 0;
double diff = oldC-newC;
initialize_cluster(img,n);

while(diff > th) {
for(int i = 0; i < n; i++)
cluster.at(i).clear();

newC = 0;
find_cluster(img,n);
diff = calculate_center(img,n,oldC,newC);
}

show_output(img,n);

return 0;


}

