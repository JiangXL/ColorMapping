#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/qhyccd.h"
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;


int main(int argc,char *argv[])
{
    int num = 0;
    qhyccd_handle *camhandle;
    int ret;
    char id[32]; //camera id
    int found = 0;
    unsigned int w,h,bpp,channels;
    unsigned char *ImgData; //pointer
    int frames = 10000,camgain = 0,camspeed = 0,cambinx = 2,cambiny = 2;

////////////////////////////////////////////////////////////////////////////
    InitQHYCCDResource();
    ScanQHYCCD();
    GetQHYCCDId(0,id);
    camhandle = OpenQHYCCD(id);
    SetQHYCCDStreamMode(camhandle, 1); //1-Live Mode, 0-SingleFrame
    InitQHYCCD(camhandle);

    double chipw,chiph,pixelw,pixelh;
    GetQHYCCDChipInfo(camhandle,&chipw,&chiph,&w,&h,&pixelw,&pixelh,&bpp);
    printf("dep%d\n",bpp );
    //SetQHYCCDBinMode(camhandle, cambinx, cambiny);
    //SetQHYCCDParam(camhandle, DDR_BUFFER_CAPACITY, 128);
    SetQHYCCDParam(camhandle,CONTROL_USBTRAFFIC,0);
    //SetQHYCCDParam(camhandle,CONTROL_EXPOSURE,5000);

    /*w=1920;
    h=1080;*/
    SetQHYCCDResolution(camhandle,0,0,w,h);
    printf("w%d h%d\n",w,h );
    //BeginLiveExposure(400);
    BeginQHYCCDLive(camhandle);
    //ExpQHYCCDSingleFrame(camhandle);

    int length = GetQHYCCDMemLength(camhandle);
    if(length > 0)
      {
          ImgData = (unsigned char *)malloc(length);
          memset(ImgData,0,length);
      }


////////////////////////////////////////////////////////////////////////////////

      int t_start,t_end;
      t_start = time(NULL);
      double fps = 0;
      Mat img=Mat(h,w,CV_8UC1);// The imge of liveframe is 8 bit.
      //if it is set wrong, opencv can not show normally.
      // live- 3fps-8bit
      // single - 0.4-16bit
      char filename[100];
      int name =0;
      namedWindow("Live", w);

      //while(frames > 0)
      while(1)
      {
          ret = GetQHYCCDLiveFrame(camhandle,&w,&h,&bpp,&channels,ImgData);
          //ExpQHYCCDSingleFrame(camhandle);
          //SetQHYCCDParam(camhandle,CONTROL_EXPOSURE,5000);
          //ret=GetQHYCCDSingleFrame(camhandle, &w, &h, &bpp, &channels, ImgData);
          if(ret == QHYCCD_SUCCESS)
          {
              img.data=ImgData;
              //sprintf(filename,"%d.tiff",name);
              //imwrite(filename,img); // save the tiff

              imshow("Live", img); // show
              if(waitKey(3)==27){
                break;
              }

              fps++;              // calculate fps
              t_end = time(NULL);
              if(t_end - t_start >= 5)
              {
                  printf("fps = %g\n",fps / 5);
                  fps = 0;
                  t_start = time(NULL);
              }
              frames --;
              name++;
	}
      }
      delete(ImgData);

////////////////////////////////////////////////////////////////////////////////
    if(camhandle)
    {
        StopQHYCCDLive(camhandle);
        CloseQHYCCD(camhandle);
    }
    ret = ReleaseQHYCCDResource();
    if (QHYCCD_SUCCESS == ret) {
        printf("SDK resources released.\n");
    } else {
        printf("Cannot release SDK resources, error %d.\n", ret);
        return 1;
    }
    return 0;

}
