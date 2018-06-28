#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include "camera.h"
#include "image.h"
#include "actionmap.h"
using namespace std;

int main(int argc,char *argv[]){
  caminit();
// camlive();

  // Viewer windows detach thread
  std::thread viewerwindow(viewer);
  viewerwindow.detach();

  // keyboard command dection detach thread
  std::thread actiondector(actiondect);
  actiondector.detach();

  // block fucntion to grab, present and save image
  camworking();

  // relative computer and cammera resources
  camlivestop();
  camclose();
}

static void help()
{
    cout
        << "------------------------------------------------------------------------------" << endl
        << "QHYCCD Cammera Capture"                                   << endl
        << "You can choose live model or single frame model."              << endl
        << "Usage:"                                                                         << endl
        << "./Player  [ R | G | B] [Y | N]"                          << endl
        << "" << endl
        << "------------------------------------------------------------------------------" << endl
        << endl;
}
