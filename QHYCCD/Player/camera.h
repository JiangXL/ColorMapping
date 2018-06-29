#define  camera_stop  0
#define  camera_live  1
#define  camera_capture  2

int caminit(void);
int camlive(void);
int timeseries(int[]);
void setexposure(int);
unsigned char *getcamlivedate(void);
void camlivestop(void);
void camclose(void);
char cam_info(void);

typedef struct
{
    char            model;
} cam_parameter; // camera parameters

extern cam_parameter cam_para;
