#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libqhy/qhyccd.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "viewer.h"
#include "camera.h"
using namespace cv;

/*
* input: the address of qhyccd img
**************************/
int frames = 10000;
int height=3522;
int width=4656;
int viewer(void){
      int t_start,t_end;
      t_start = time(NULL);
      double fps = 0;
      Mat img=Mat(height,width,CV_8UC1);// The imge of liveframe is 8 bit.
      //if it is set wrong, opencv can not show normally.
      char filename[100];
      int name =0;
      namedWindow("QHYCCD Viewer", width/2);

      while(1)
      {
              img.data = getcamlivedate(); // accept the image date
              //sprintf(filename,"%d.tiff",name);
              //imwrite(filename,img); // save the tiff

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
              frames --;
              name++;

      }
      return 1;
}
