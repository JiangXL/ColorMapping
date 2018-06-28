#include "actionmap.h"

/*cout << "： ";
cin >> keymap;
cout << name << endl;
*/

int newation =0;
model = camera_live;

void camworking(void){
  while( model != camera_stop ){// until it is close
    int update = 1;
    while( newation == 1 || update == 1){
      switch( model ){
        case camera_live  :
          camlive();// set and start live model
          //exposure_detect();
          update = 0;
          break;

          case camera_capture  :
            //loadingconfig();
            //if no config load default file;
            setexposure(0); //
            int frames = 1000; //

            double lasttime= clock;
            while(clock-lasttime <= timegap && frames >=1){
              if(clock-lasttime == timegap){
                capture(imgname);
                lasttime = clock; //reset clock
                frames = frames-1;
              }
            }
            model = camera_live;
            update = 1;
            break;

            default :
              model = camera_live;
        }
      }
  }
}

void actiondect(){
  model = camera_live;
}
