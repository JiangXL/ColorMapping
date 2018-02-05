#include <stdio.h>
#include <string.h>
#include <mpsse.h>

int main(int argc, char *argv[])
{
    char *data = NULL;
    struct mpsse_context *flash = NULL;
 
    if(argc < 2)
    {
        fprintf(stderr,"not enough params\n");
        return 1;
    } 

    int value = atoi(argv[1]);

    flash = MPSSE(GPIO,1,LSB);

    if(value == 0)
    {
        PinLow(flash,GPIOL3);
    }
    
    if(value == 1)
    {
        PinHigh(flash,GPIOL3);
    }
    return 0;
}
