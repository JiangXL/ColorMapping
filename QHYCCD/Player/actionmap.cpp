#include <iostream>
#include "actionmap.h"
#include "camera.h"
#include "image.h"
#include <time.h>

using namespace std;
int camera_state = camera_capture;
int newation =0;
double timegap = 1000000;

void camworking(void){
  int update = 1;
  while( camera_state != camera_stop ){// until it is close
    //printf("Newation%i\n", newation);
    //while( newation == 1 || update == 1){
    while( update == 1){
      switch( camera_state ){
        case camera_live  :
          //camlive();// set and start live model
          //exposure_detect();
          cout<<"start live model"<<endl;
          update = 0;
          break;

        case camera_capture  :
          camlivestop();
            //if possible close live stream when use single frame
            //loadingconfig();
            //if no config load default file;
            //setexposure(0); //
          int frames = 100; //
          char imgname[100];

          double lasttime= clock();
          while(frames >=1){
            if(clock()-lasttime >= timegap){
                //capture(imgname);
                printf("SingleFrame: %i\n", frames);
                lasttime = clock(); //reset clock
                frames = frames-1;
            }
          }
          camera_state = camera_live;
          update = 1;
          break;
        }
      }
  }
}

// Use keyboard to update camera state
void actiondect(){
  char key_buf;
  //while(camera_state != camera_stop){
  while( 1 ){
    newation = 0; // reset 0 to disable state update
    cin >> key_buf;
    int key_ascii = key_buf;
    newation = 1; // enable state update once new key was entered.
    //printf("Enter key:%i\n",key_ascii);
    switch (key_ascii) {
      case key_l:
        camera_state = camera_live;
        break;
      case key_c:
        camera_state = camera_capture;
        break;
      case key_esc:
        camera_state = camera_stop;
        break;
      default :
        newation = 0;
    }
  }

}
