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

#include "camera.h"
#include "viewer.h"
using namespace cv;



int main(int argc,char *argv[]){

  caminit();
  camlive();
  setexposure(0); //
  viewer();
  camlivestop();
  camclose();
  return 0;

}
