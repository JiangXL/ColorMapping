
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/qhyccd.h"

#define VERSION 1.10

int main(int argc, char *argv[]) {

    int USB_TRAFFIC = 10;
    int CHIP_GAIN = 10;
    int CHIP_OFFSET = 140;
    int EXPOSURE_TIME = 20000;
    int camBinX = 1;
    int camBinY = 1;

    double chipWidthMM;
    double chipHeightMM;
    double pixelWidthUM;
    double pixelHeightUM;

    unsigned int roiStartX;
    unsigned int roiStartY;
    unsigned int roiSizeX;
    unsigned int roiSizeY;

    unsigned int overscanStartX;
    unsigned int overscanStartY;
    unsigned int overscanSizeX;
    unsigned int overscanSizeY;

    unsigned int effectiveStartX;
    unsigned int effectiveStartY;
    unsigned int effectiveSizeX;
    unsigned int effectiveSizeY;

    unsigned int maxImageSizeX;
    unsigned int maxImageSizeY;
    unsigned int bpp;
    unsigned int channels;

    unsigned char *pImgData = 0;

    const char *pFirmware = "/usr/local/lib/qhy";

    printf("SingleFrameMode, Version: %.2f\n", VERSION);

    // First of all, we have to call the InitQHYCCDResource function, which also
    // initializes the low level libusb library.
    int retVal = InitQHYCCDResource();
    if (QHYCCD_SUCCESS == retVal) {
        printf("SDK resources initialized.\n");
    }

    // scan cameras
    int camCount = ScanQHYCCD();

    // iterate over all attached cameras
    bool camFound = false;
    char camId[32];

    retVal = GetQHYCCDId(0, camId); //0 mean the first camera

    // open camera
    qhyccd_handle *pCamHandle = OpenQHYCCD(camId);

    // set single frame mode
    int mode = 0;
    retVal = SetQHYCCDStreamMode(pCamHandle, mode);
    printf("SetQHYCCDStreamMode set to: Single Frame\n");

    // initialize camera
    retVal = InitQHYCCD(pCamHandle);
    if (QHYCCD_SUCCESS == retVal) {
        printf("InitQHYCCD success.\n");
    } else {
        printf("InitQHYCCD faililure, error: %d\n", retVal);
        return 1;
    }

    // get chip info
    retVal = GetQHYCCDChipInfo(pCamHandle, &chipWidthMM, &chipHeightMM, &maxImageSizeX, &maxImageSizeY, &pixelWidthUM, &pixelHeightUM, &bpp);
    if (QHYCCD_SUCCESS == retVal) {
        printf("Chip  size width x height     : %.3f x %.3f [mm]\n", chipWidthMM, chipHeightMM);
        printf("Pixel size width x height     : %.3f x %.3f [um]\n", pixelWidthUM, pixelHeightUM);
        printf("Image size width x height     : %d x %d\n", maxImageSizeX, maxImageSizeY);
    } else {
        printf("GetQHYCCDChipInfo failure, error: %d\n", retVal);
        return 1;
    }

    // set ROI
    roiStartX = 0;
    roiStartY = 0;
    roiSizeX = maxImageSizeX;
    roiSizeY = maxImageSizeY;

    // set exposure time
    retVal = SetQHYCCDParam(pCamHandle, CONTROL_EXPOSURE, EXPOSURE_TIME);
        printf("SetQHYCCDParam CONTROL_EXPOSURE set to: %d, success.\n", EXPOSURE_TIME);

    // set image resolution
    retVal = SetQHYCCDResolution(pCamHandle, roiStartX, roiStartY, roiSizeX, roiSizeY);

    // set binning mode
    retVal = SetQHYCCDBinMode(pCamHandle, camBinX, camBinY);

    // single frame
    retVal = ExpQHYCCDSingleFrame(pCamHandle);

    // get requested memory length
    uint32_t length = GetQHYCCDMemLength(pCamHandle);
    printf("Will try to allocate: %d [uchar] for image data.\n", length);

    if (length > 0) {
        pImgData = new unsigned char[length];
        memset(pImgData, 0, length);
        printf("Allocated memory for frame: %d [uchar].\n", length);
    } else {
        printf("Cannot allocate memory for frame.\n");
        return 1;
    }

    // get single frame
    retVal = GetQHYCCDSingleFrame(pCamHandle, &roiSizeX, &roiSizeY, &bpp, &channels, pImgData);
    if (QHYCCD_SUCCESS == retVal) {
        printf("GetQHYCCDSingleFrame: %d x %d, bpp: %d, channels: %d, success.\n", roiSizeX, roiSizeY, bpp, channels);
        //process image here
    }

    delete [] pImgData;

    retVal = CancelQHYCCDExposingAndReadout(pCamHandle);
    if (QHYCCD_SUCCESS == retVal) {
        printf("CancelQHYCCDExposingAndReadout success.\n");
    }

    // close camera handle
    retVal = CloseQHYCCD(pCamHandle);
    if (QHYCCD_SUCCESS == retVal) {
        printf("Close QHYCCD success.\n");
    } else {
        printf("Close QHYCCD failure, error: %d\n", retVal);
    }

    // We have to call ReleaseQHYCCDResource function to release libusb
    // and other sdk resources
    retVal = ReleaseQHYCCDResource();
    if (QHYCCD_SUCCESS == retVal) {
        printf("SDK resources released.\n");
    } else {
        printf("Cannot release SDK resources, error %d.\n", retVal);
        return 1;
    }

    return 0;
}
