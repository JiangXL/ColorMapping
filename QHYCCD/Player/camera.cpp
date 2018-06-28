#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/qhyccd.h"
#include <time.h>

#include "camera.h"

int num = 0;
qhyccd_handle *camhandle;
int ret;
char id[32]; //camera id
char *model;
int found = 0;
unsigned int w,h,bpp,channels;
unsigned char *ImgData; //unsigned char type's pointer
int camgain = 0,camspeed = 0,cambinx = 1,cambiny = 1;
int exposure=100;
double chipw,chiph,pixelw,pixelh;

int caminit(void)
{
    InitQHYCCDResource();
    ScanQHYCCD();
    GetQHYCCDId(0,id);
    camhandle = OpenQHYCCD(id);

    GetQHYCCDChipInfo(camhandle,&chipw,&chiph,&w,&h,&pixelw,&pixelh,&bpp);
    printf("dep%d\n",bpp );
    return 1;
}

void setexposure(int time){
  if(time > 0){
    SetQHYCCDParam(camhandle,CONTROL_EXPOSURE, time);
  }else{
    // how to set auto??
  }
}

int camlive(void){
    SetQHYCCDStreamMode(camhandle, 1); //1-Live Mode, 0-SingleFrame
    InitQHYCCD(camhandle);

    SetQHYCCDBinMode(camhandle, cambinx, cambiny); // What it mean?
    //SetQHYCCDParam(camhandle, DDR_BUFFER_CAPACITY, 128);
    SetQHYCCDParam(camhandle,CONTROL_USBTRAFFIC,0);
    SetQHYCCDResolution(camhandle,0,0,w,h);

    setexposure(0);

    GetQHYCCDChipInfo(camhandle,&chipw,&chiph,&w,&h,&pixelw,&pixelh,&bpp);
    printf("w%d h%d bpp%d\n",w,h, bpp);

    BeginQHYCCDLive(camhandle);

    int length = GetQHYCCDMemLength(camhandle);
    if(length > 0)
      {
          ImgData = (unsigned char *)malloc(length); // what is mean?
          memset(ImgData,0,length);
      }
    return 1;
}

unsigned char *getcamlivedate(){
    ret = GetQHYCCDLiveFrame(camhandle,&w,&h,&bpp,&channels,ImgData);
    return ImgData;
}

int timeseries(int a[]){
  //ExpQHYCCDSingleFrame(camhandle);
  //SetQHYCCDParam(camhandle,CONTROL_EXPOSURE,5000);
 //ret=GetQHYCCDSingleFrame(camhandle, &w, &h, &bpp, &channels, ImgData);
  return 1;
}

void camlivestop(void){
  if(camhandle)
  {
      ret = StopQHYCCDLive(camhandle);
      delete(ImgData);
      if (QHYCCD_SUCCESS != ret) {
          printf("SDK resources released failed.\n");
        }
  }
}

void camclose(void){
    if(camhandle)
    {
        CloseQHYCCD(camhandle);
    }
    ret = ReleaseQHYCCDResource();
    if (QHYCCD_SUCCESS == ret) {
        printf("SDK resources released.\n");
    } else {
        printf("Cannot release SDK resources, error %d.\n", ret);
    }
}
