#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <thread>
#include <iostream>
#include "camera.h"
#include "image.h"
using namespace cv;

/* H.F. 20180627
* input: the address of qhyccd img
**************************/
int frames = 10000;
int height=3522;
int width=4656;
Mat img=Mat(height,width,CV_8UC1);// The imge of liveframe is 8 bit.

//update raw img and save to opencv img
void acceptrawimg(void){
      img.data = getcamlivedate(); // accept the image date
}

//void save2tiff(char name[100){
void save2tiff(char name[100]) {
      Mat imgbuf = img.clone(); // copy to new memory to avoid image cover
      imwrite(name, imgbuf);
      return;
}

int capture(char imgname[100]){
      acceptrawimg();
      std::thread tiff(save2tiff, imgname);
      tiff.detach();
      return 1;
}

int viewer(void){
      int t_start,t_end;
      t_start = time(NULL);
      double fps = 0;
      int name =0;
      char filename[100];

      namedWindow("QHYCCD Viewer", width/2);

      while(1)
      {
              sprintf(filename,"live/%d.tiff",name);
              capture(filename);
              imshow("QHYCCD Viewer", img); // show
              if(waitKey(3)==27){  // put down esc  to break
                break;
              }

              fps++;              // calculate fps
              t_end = time(NULL);
              if(t_end - t_start >= 5)
              {
                  printf("fps = %g\n",fps / 5);
                  //printf("%s\n", &model);
                  fps = 0;
                  t_start = time(NULL);
              }
              name++;
      }
      return 1;
}
