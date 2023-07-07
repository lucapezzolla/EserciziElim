#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

double smthreshold = 35.8;
int tsize = 4;



class TNode {
  private:
     Rect region;
     TNode *UL, *UR, *LL, *LR;
     vector<TNode*> merged;
     vector<bool> mergedB = vector<bool>(4,false);
     double stddev, mean;

  public:
     TNode(Rect R) {region = R,UL=UR=LL=LR=nullptr;};

     TNode *getUL() {return UL;};
     TNode *getUR() {return UR;};
     TNode *getLL() {return LL;};
     TNode *getLR() {return LR;};

     void setUL(TNode *t) {UL=t;};
     void setUR(TNode *t) {UR=t;};
     void setLL(TNode *t) {LL=t;};
     void setLR(TNode *t) {LR=t;};

     double getStddev() {return this->stddev;};
     double getMean() {return this->mean;};

     void setStddev(double s) {this->stddev = s;};
     void setMean(double m) {this->mean = m;};

     void addRegion(TNode *r) {merged.push_back(r);};
     vector<TNode *> &getMerged() {return merged;};

     Rect getRegion() {return region;};

     void setMergedB(int i) {mergedB[i]=true;};
     bool getMergedB(int i) {return mergedB[i];};

};


    TNode *split(Mat img, Rect R) {

    TNode *root = new TNode(R);
    Scalar stddev, mean;
    meanStdDev(img(R),mean,stddev);

    root->setMean(mean[0]);
    root->setStddev(stddev[0]);

    if(R.width>tsize && root->getStddev()>smthreshold){

        Rect ul(R.x,R.y,R.height/2,R.width/2);
        root->setUL(split(img,ul));

        Rect ur(R.x,R.y+R.width/2,R.height/2,R.width/2);
        root->setUR(split(img,ur));

        Rect ll(R.x+R.height/2,R.y,R.height/2,R.width/2);
        root->setLL(split(img,ll));

        Rect lr(R.x+R.height/2,R.y+R.width/2,R.height/2,R.width/2);
        root->setLR(split(img,lr));
    }

    rectangle(img,R,Scalar(0));

    return root;
  }


  //UL-UR  0-1
  //LL-LR  3-2


   void Merge(TNode *root) {

    if(root->getRegion().width > tsize && root->getStddev() > smthreshold) {
     if(root->getUL()->getStddev() <= smthreshold && root->getUR()->getStddev() <= smthreshold) {
      root->addRegion(root->getUL());
      root->setMergedB(0);
      root->addRegion(root->getUR());
      root->setMergedB(1);
      if(root->getLL()->getStddev() <= smthreshold && root->getLR()->getStddev() <= smthreshold) {
      root->addRegion(root->getLL());
      root->setMergedB(3);
      root->addRegion(root->getUR());
      root->setMergedB(2);
      }

      else {
       Merge(root->getLL());
       Merge(root->getLR());
     }
     }

     else if(root->getUR()->getStddev() <= smthreshold && root->getLR()->getStddev() <= smthreshold) {
      root->addRegion(root->getUR());
      root->setMergedB(1);
      root->addRegion(root->getLR());
      root->setMergedB(2);
      if(root->getUL()->getStddev() <= smthreshold && root->getLL()->getStddev() <= smthreshold) {
      root->addRegion(root->getUL());
      root->setMergedB(2);
      root->addRegion(root->getLL());
      root->setMergedB(3);
    }
     else {
     Merge(root->getLL());
     Merge(root->getUL());
     }
     }

     else if(root->getLR()->getStddev() <= smthreshold && root->getLL()->getStddev() <= smthreshold) {
     root->addRegion(root->getLR());
     root->setMergedB(2);
     root->addRegion(root->getLL());
     root->setMergedB(3);
     if(root->getUL()->getStddev() <= smthreshold && root->getUR()->getStddev() <= smthreshold) {
      root->addRegion(root->getUL());
      root->setMergedB(0);
      root->addRegion(root->getUR());
      root->setMergedB(1);
     }

     else {
     Merge(root->getUL());
     Merge(root->getUR());
     }
     }

     else if(root->getLL()->getStddev() <= smthreshold && root->getUL()->getStddev() <= smthreshold) {
     root->addRegion(root->getLL());
     root->setMergedB(3);
     root->addRegion(root->getUL());
     root->setMergedB(0);
     if(root->getLR()->getStddev() <= smthreshold && root->getUR()->getStddev() <= smthreshold) {
     root->addRegion(root->getLR());
     root->setMergedB(2);
     root->addRegion(root->getUR());
     root->setMergedB(1);
     }

     else {
     Merge(root->getLR());
     Merge(root->getUR());
     }
     }

     else {
     Merge(root->getUL());
     Merge(root->getUR());
     Merge(root->getLL());
     Merge(root->getLR());
     }
     }

     else {

     root->addRegion(root);
     root->setMergedB(0);
     root->setMergedB(1);
     root->setMergedB(2);
     root->setMergedB(3);
  }

}


   void segment(TNode *root,Mat &img){

   vector<TNode*> tmp = root->getMerged();

   if(!tmp.size()) {
   segment(root->getUL(),img);
   segment(root->getUR(),img);
   segment(root->getLR(),img);
   segment(root->getLL(),img);
 }

  else {
   double val = 0;
   for(auto x:tmp) {
   val+=x->getMean();
  }

   val /= tmp.size();

   for(auto x:tmp){
    img(x->getRegion())=(int)val;
    }

   if(tmp.size() > 1) {
   if(!root->getMergedB(0))
   segment(root->getUL(),img);
   if(!root->getMergedB(1))
   segment(root->getUR(),img);
   if(!root->getMergedB(2))
   segment(root->getLR(),img);
   if(!root->getMergedB(3))
   segment(root->getLL(),img);
 }

}

}




int main(int argc, char **argv) {

Mat img = imread(argv[1],IMREAD_GRAYSCALE);

if(img.empty()) {

cout<<"Immagine non caricata correttamente"<<endl;
exit(-1);

}

GaussianBlur(img, img, Size(5,5), 0, 0);
Rect square = Rect(0,0,300,300);
img = img(square).clone();
Mat seg = img.clone();


TNode *root = split(img,Rect(0,0,img.rows,img.cols));

Merge(root);

segment(root,seg);

imshow("Immagine splittata",img);
imshow("Immagine segmentata",seg);
waitKey(0);



return 0;

}

