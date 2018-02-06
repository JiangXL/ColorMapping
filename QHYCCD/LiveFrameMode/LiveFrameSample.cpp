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

using namespace cv;


int main(int argc,char *argv[])
{
    int num = 0;
    qhyccd_handle *camhandle;
    int ret;
    char id[32]; //camera id
    char camtype[16];
    int found = 0;
    unsigned int w,h,bpp,channels;
    unsigned char *ImgData; //pointer
    int camtime = 1,camgain = 0,camspeed = 2,cambinx = 1,cambiny = 1;

    ret = InitQHYCCDResource();
    if(ret == QHYCCD_SUCCESS)
    {
        printf("Init SDK success!\n");
    }

    ScanQHYCCD();
    GetQHYCCDId(0,id);

    camhandle = OpenQHYCCD(id);
      if(camhandle == NULL)
      {
          printf("Open QHYCCD failed \n");
          return -1;
      }

    SetQHYCCDStreamMode(camhandle,1); // Live Mode
    ret = InitQHYCCD(camhandle);
        if(ret != QHYCCD_SUCCESS)
        {
            printf("Init QHYCCD fail code:%d\n",ret);
            return -1;
        }

    double chipw,chiph,pixelw,pixelh;
    ret = GetQHYCCDChipInfo(camhandle,&chipw,&chiph,&w,&h,&pixelw,&pixelh,&bpp);
    if(ret == QHYCCD_SUCCESS)
    {
            printf("Chip width %3f mm,Chip height %3f mm\n",chipw,chiph);
            printf("Chip pixel width %3f um,Chip pixel height %3f um\n",pixelw,pixelh);
            printf("Chip Max Resolution is %d x %d,depth is %d\n",w,h,bpp);
    }

    ret = SetQHYCCDResolution(camhandle,0,0,w,h);
      if(ret != QHYCCD_SUCCESS)
      {
          printf("SetQHYCCDResolution fail\n");
          return -1;
      }

      ret = BeginQHYCCDLive(camhandle);
      if(ret != QHYCCD_SUCCESS)
      {
          printf("BeginQHYCCDLive failed\n");
          //goto failure;
          return -1;
      }

      int length = GetQHYCCDMemLength(camhandle);

      if(length > 0)
      {
          ImgData = (unsigned char *)malloc(length);
          memset(ImgData,0,length);
      }


////////////////////////////////////////////////////////////////////////////////

      int t_start,t_end;
      t_start = time(NULL);
      int fps = 0;
      Mat img=Mat(w,h,CV_8UC1);
      namedWindow("Live", w);

      ret = QHYCCD_ERROR;
//      while(ret != QHYCCD_SUCCESS)
      while(true)
      {
          ret = GetQHYCCDLiveFrame(camhandle,&w,&h,&bpp,&channels,ImgData);
          if(ret == QHYCCD_SUCCESS)
          {
              //img=Mat(w,h,CV_8UC1, ImgData);
              img.data=ImgData;
              imshow("Live", img);
              if(waitKey(10)==27){
                break;
              }

              fps++;
              t_end = time(NULL);
              if(t_end - t_start >= 5)
              {
                  printf("fps = %d\n",fps / 5);
                  fps = 0;
                  t_start = time(NULL);
              }
            }
      }
      delete(ImgData);


    if(camhandle)
    {
        StopQHYCCDLive(camhandle);

        ret = CloseQHYCCD(camhandle);
        if(ret == QHYCCD_SUCCESS)
        {
            printf("Close QHYCCD success!\n");
        }
        else
        {
            return -1;
        }
    }

    ret = ReleaseQHYCCDResource();
    if(ret == QHYCCD_SUCCESS)
    {
        printf("Rlease SDK Resource  success!\n");
    }
    else
    {
      return -1;
    }

    return 0;

}
