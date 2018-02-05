#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <uuid/uuid.h>
#include <libqhy/qhyccd.h>
#include <string.h>
#include <unistd.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "log4z.h"
#include "fitsio.h"
#include "resetcam.h"

using namespace zsummer::log4z;

#define IDLE    0
#define EXPSING 1
#define READING 2


#define EXECSTATUSIDLE    0
#define CAMSTATUSNOTIDLE -1
#define CAMNOTEXIST      -2
#define CAMCONNECTERROR  -3

#define PREVIEWMODE       0
#define FOCUSMODE         1
#define CAPTUREMODE       2

struct FITPARAS
{
    char CamName[36];
    int Gain;
    int Offset;
    int Exptime;
    char Date_Fit[36];
    char Time_Fit[36];
    double ImageTEMP;      //发给FIT 文件的开始拍摄时的温度
    char ImageTimeMS[36];  //发给FIT文件的包含毫秒计数的时间
};


struct CAPCTROL
{
   char *username;
   qhyccd_handle *camh;
   int imageW;
   int imageH;
   unsigned char *imageData;
};

MYSQL capcon;
pthread_mutex_t mutexcap;
pthread_mutex_t mutexplanner;

int querydouble(MYSQL connection,char *sqlcmd,double *retdouble)
{
    /*指向查询结果的指针*/
    MYSQL_RES *res_ptr;

    /*字段结构指针*/
    MYSQL_FIELD *field; 

    /*按行返回的查询信息*/
    MYSQL_ROW result_row; 

    /*查询返回的行数和列数*/
    int row, column; 

    int res = mysql_query(&connection,sqlcmd);
    if(res)
    {
        LOGFMTD("select error: %s",mysql_error(&connection));            
    }
    else
    {
        /*将查询的結果给res_ptr*/
        res_ptr = mysql_store_result(&connection);

        if (res_ptr) 
        {
            result_row = mysql_fetch_row(res_ptr);
            if(result_row)
            {
                
                {
                    LOGFMTD("retdouble: %s",result_row[0]);
                }
            
                *retdouble = atoi(result_row[0]);
            }
            mysql_free_result(res_ptr);
        }
        else
        {
            LOGFMTD("mysql_store_result error: %s",mysql_error(&connection));            
            return 1;  
        }
    }
    return 0;  
}

int queryint(MYSQL connection,char *sqlcmd,int *retint)
{
    /*指向查询结果的指针*/
    MYSQL_RES *res_ptr;

    /*字段结构指针*/
    MYSQL_FIELD *field; 

    /*按行返回的查询信息*/
    MYSQL_ROW result_row; 

    /*查询返回的行数和列数*/
    int row, column; 

    int res = mysql_query(&connection,sqlcmd);
    if(res)
    {
        LOGFMTD("select error: %s",mysql_error(&connection));            
    }
    else
    {
        /*将查询的結果给res_ptr*/
        res_ptr = mysql_store_result(&connection);

        if (res_ptr) 
        {
            result_row = mysql_fetch_row(res_ptr);
            if(result_row)
            {
                {
                    LOGFMTD("retint: %s",result_row[0]);
                }
                *retint = atoi(result_row[0]);
            }
            mysql_free_result(res_ptr);
        }
        else
        {
            LOGFMTD("mysql_store_result error: %s",mysql_error(&connection));  
            return mysql_errno(&connection);      
        }
    }
    return res;
}

int updatecmd(MYSQL connection,char *sqlcmd)
{    
    int res = mysql_query(&connection,sqlcmd);    
    if(res)
    {
        LOGFMTD("updatecmd error: %s",mysql_error(&connection));
    }
    return res;
}

int queryresultnum(MYSQL connection,char *sqlcmd,int *resultnum)
{
    MYSQL_RES *resptr;

    int res = mysql_query(&connection,sqlcmd);    
    if(res)
    {
        LOGFMTD("querycmd error: %s",mysql_error(&connection));
    }

    //resptr = mysql_store_result(&connection);

    //*resultnum = mysql_num_rows(resptr);

    return 0;
}

int querystring(MYSQL connection,char *sqlcmd,char *retstr)
{
    /*指向查询结果的指针*/
    MYSQL_RES *res_ptr;

    /*字段结构指针*/
    MYSQL_FIELD *field; 

    /*按行返回的查询信息*/
    MYSQL_ROW result_row; 

    /*查询返回的行数和列数*/
    int row, column; 

    int res = mysql_query(&connection,sqlcmd);
    if(res)
    {
        LOGFMTD("select error: %s",mysql_error(&connection));            
    }
    else
    {
        /*将查询的結果给res_ptr*/
        res_ptr = mysql_store_result(&connection);

        if (res_ptr) 
        {
            result_row = mysql_fetch_row(res_ptr);

            LOGFMTD("retstr: %s",result_row[0]);

            strcpy(retstr,result_row[0]);
            mysql_free_result(res_ptr);
        }
        else
        {
            LOGFMTD("mysql_store_result error: %s",mysql_error(&connection));             
        }
    }
    return res;
}

int querycamcmdcambinspeedcapmode(MYSQL connection,char *username,int *camexp,int *cambin,int *camspeed,int *camcapmode,int *focstartx,int *focstarty)
{
    int ret;
    char sqlcmd[128];
    
    sprintf(sqlcmd,"SELECT CamBin FROM camcmd WHERE UserName='%s'",username); 
    LOGFMTD("Querycamcmdcambinspeedcapmode sqlcmd %s",sqlcmd);

    ret = queryint(connection,sqlcmd,cambin);
    if(ret)
    {
        LOGD("query camcmd cambin error");
        return ret;
    }
    LOGFMTD("camcmd cambin %d",*cambin);
 
    sprintf(sqlcmd,"SELECT CamExp FROM camcmd WHERE UserName='%s'",username);  
    LOGFMTD("Querycamcmdcambinspeedcapmode sqlcmd %s",sqlcmd);

    ret = queryint(connection,sqlcmd,camexp);
    if(ret)
    {
        LOGD("query camcmd camexp error");
        return ret;
    }
    LOGFMTD("camcmd camexp %d",*camexp);

    sprintf(sqlcmd,"SELECT CamSpeed FROM camcmd WHERE UserName='%s'",username);  
    LOGFMTD("Querycamcmdcambinspeedcapmode sqlcmd %s",sqlcmd);

    ret = queryint(connection,sqlcmd,camspeed);
    if(ret)
    {
        LOGD("query camcmd camspeed error");
        return ret;
    }
    LOGFMTD("camcmd camspeed %d",*camspeed);

    sprintf(sqlcmd,"SELECT CamCapMode FROM camcmd WHERE UserName='%s'",username);  
    LOGFMTD("Querycamcmdcambinspeedcapmode sqlcmd %s",sqlcmd);

    ret = queryint(connection,sqlcmd,camcapmode);
    if(ret)
    {
        LOGD("query camcmd camcapmode error");
        return ret;
    }

    sprintf(sqlcmd,"SELECT FocStartX FROM camcmd WHERE UserName='%s'",username);  
    LOGFMTD("Querycamcmdcambinspeedcapmode sqlcmd %s",sqlcmd);

    ret = queryint(connection,sqlcmd,focstartx);
    if(ret)
    {
        LOGD("query camcmd camcapmode error");
        return ret;
    }

    sprintf(sqlcmd,"SELECT FocStartY FROM camcmd WHERE UserName='%s'",username);  
    LOGFMTD("Querycamcmdcambinspeedcapmode sqlcmd %s",sqlcmd);

    ret = queryint(connection,sqlcmd,focstarty);
    if(ret)
    {
        LOGD("query camcmd camcapmode error");
        return ret;
    }

    LOGFMTD("camcmd camcapmode %d",*camcapmode);
   
    return 0;
}

int querygeargainoffset(MYSQL connection,char *gearname,int cambin,int camspeed,int *camgain,int *camoffset)
{
    int ret;
    char sqlcmd1[128];
    char sqlcmd2[128];

    if(camspeed == 0)
    {
        sprintf(sqlcmd1,"SELECT OffsetBIN%dLS FROM gear WHERE GearName='%s'",cambin,gearname);  
        sprintf(sqlcmd2,"SELECT GainBIN%dLS FROM gear WHERE GearName='%s'",cambin,gearname); 
    }
    else
    {
        sprintf(sqlcmd1,"SELECT OffsetBIN%dHS FROM gear WHERE GearName='%s'",cambin,gearname);  
        sprintf(sqlcmd2,"SELECT GainBIN%dHS FROM gear WHERE GearName='%s'",cambin,gearname); 
    }

    LOGFMTD("Sqlcmd1 %s",sqlcmd1);
    LOGFMTD("Sqlcmd2 %s",sqlcmd2);

    ret = queryint(connection,sqlcmd1,camoffset);
    if(ret)
    {
        LOGD("query camoffset error");
        return ret;
    }
    LOGFMTD("camoffset %d",*camoffset);
    
    ret = queryint(connection,sqlcmd2,camgain);
    if(ret)
    {
        LOGD("query camgain error");
        return ret;
    }
    LOGFMTD("camgain %d",*camgain);
    
    return 0;
}

int querycamstatus(MYSQL connection,char *defaultcamid,int *camstatus)
{
    char sqlcmd[128];
 
    sprintf(sqlcmd,"SELECT CamStatus FROM status WHERE CamId='%s'",defaultcamid);                
    
    LOGFMTD("Querycamstatus sqlcmd %s",sqlcmd);

    return queryint(connection,sqlcmd,camstatus);
}

int updatecamstatus(MYSQL connection,char *defaultcamid,int camstatus)
{
    char sqlcmd[256];

    sprintf(sqlcmd,"UPDATE status set CamStatus=%d where CamId='%s'",camstatus,defaultcamid);
    
    LOGFMTD("Updatecamstatus sqlcmd %s",sqlcmd);
   
    return updatecmd(connection,sqlcmd);
}

int updatecamexpschedule(MYSQL connection,char *defaultcamid,double expschedule)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set ExpSchedule=%f where CamId='%s'",expschedule,defaultcamid);
    LOGFMTD("Updatecamstatus sqlcmd %s",sqlcmd);
  
    return updatecmd(connection,sqlcmd);
}

int updatestatustotalplanschedule(MYSQL connection,char *defaultcamid,double plannerschedule)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set TotalPlanSchedule=%f where CamId='%s'",plannerschedule,defaultcamid);
    LOGFMTD("updatestatustotalplanschedule sqlcmd %s",sqlcmd);
  
    return updatecmd(connection,sqlcmd);
}

int updatecamreadusbschedule(MYSQL connection,char *defaultcamid,double readusbschedule)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set ReadOutSchedule=%f where CamId='%s'",readusbschedule,defaultcamid);
    
    
    {
        LOGFMTD("Updatecamstatus sqlcmd %s",sqlcmd);
    }

    return updatecmd(connection,sqlcmd);
}

int insertstatus(MYSQL connection,char *camid)
{
    char sqlcmd[256];
    sprintf(sqlcmd,"INSERT into status values('%s',0,0,0,'NULL','NULL','NULL','NULL',0,100,100,0)",camid);
    LOGFMTD("insertstatus Sqlcmd %s",sqlcmd);
    
    int res = mysql_query(&connection,sqlcmd);

    if(res)
    {
        LOGFMTD("INSERT error: %s",mysql_error(&connection));
    }

    return res;
}

int isstatuscamidexistandinsert(char *camid)
{
    char sqlcmd[256];
    int resultnum = 0;
    MYSQL connection;

    sprintf(sqlcmd,"SELECT * from status WHERE CamId='%s'",camid);   

    /*初始化mysql连接*/
    mysql_init(&connection);
	
    /*建立与数据库的连接*/
    if(mysql_real_connect(&connection, "localhost", "root", "root", "ic8300", 0, NULL, 0))
    {
/*
        int res = mysql_query(&connection,sqlcmd);    
        if(res)
        {
            LOGFMTD("querycmd error: %s",mysql_error(&connection));
        }


        MYSQL_RES *resptr = mysql_store_result(&connection);
        if(resptr)
        {
            resultnum = mysql_num_rows(resptr);
            if(resultnum == 0)
            {
                insertstatus(connection,camid);
            }
        }
        else
        {
            LOGD("mysql_store_result");
        }
*/

        insertstatus(connection,camid);   
        
        mysql_close(&connection);
        mysql_library_end();
    }
    return 0; 
}


int insertimagelog(MYSQL connection,char *imagename,char *begindatetime,char *enddatetime)
{
    char sqlcmd[256];
    sprintf(sqlcmd,"INSERT into imagelog values('','%s','%s','%s','','','','')",imagename,begindatetime,enddatetime);
    LOGFMTD("Insertimagelog Sqlcmd %s",sqlcmd);
    
    /*将拍摄产生的数据插入数据库image表*/
    int res = mysql_query(&connection,sqlcmd);

    if(res)
    {
        LOGFMTD("INSERT error: %s",mysql_error(&connection));
    }

    return res;
}

int queryusergearname(MYSQL connection,char *username,char *defaultgearname)
{   
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT DefaultGearName FROM user WHERE UserName='%s'",username);                
    LOGFMTD("Sqlcmd %s",sqlcmd);    
    
    int ret = querystring(connection,sqlcmd,defaultgearname);
    if(ret)
    {
        LOGFMTD("queryusergearname error");
    }
    LOGFMTD("queryusergearname %s",defaultgearname);

    return ret;
}

int queryusercamid(MYSQL connection,char *username,char *defaultcamid)
{    
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT DefaultCamId FROM user WHERE UserName='%s'",username);                
    LOGFMTD("sqlcmd %s",sqlcmd);
    
    int ret = querystring(connection,sqlcmd,defaultcamid);
    if(ret)
    {
        LOGFMTD("queryusercamid error");
    }
    LOGFMTD("queryusercamid %s",defaultcamid);
    
    return ret;
}

int queryuserplannername(MYSQL connection,char *username,char *defaultplannername)
{    
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT DefaultPlannerName FROM user WHERE UserName='%s'",username);                
    LOGFMTD("Sqlcmd %s",sqlcmd);
    
    int ret = querystring(connection,sqlcmd,defaultplannername);
    if(ret)
    {
        LOGD("queryuserplannername error");
    }
    
    LOGFMTD("queryuserplannername %s",defaultplannername);
   
    return ret;
}



int updatestatuslastimage(MYSQL connection,int camcapmode,char *defaultcamid,char *imagename)
{
    int ret = 0;
    char sqlcmd[128];
    char sqlcmd2[128];

    switch(camcapmode)
    {
        case PREVIEWMODE:
        {
             sprintf(sqlcmd,"UPDATE status set LastPreviewImageName='%s' where CamId='%s'",imagename,defaultcamid);
        }
        break;
        case FOCUSMODE:
        {
             sprintf(sqlcmd,"UPDATE status set LastFocusImageName='%s' where CamId='%s'",imagename,defaultcamid);
        }
        break;
        case CAPTUREMODE:
        {
             sprintf(sqlcmd,"UPDATE status set LastCaptureImageName='%s' where CamId='%s'",imagename,defaultcamid);          
        }
        break;
    }
    sprintf(sqlcmd2,"UPDATE status set LastImageName='%s' where CamId='%s'",imagename,defaultcamid);  
    LOGFMTD("Updatecamstatus sqlcmd %s",sqlcmd);
   
    ret = updatecmd(connection,sqlcmd);
    ret |= updatecmd(connection,sqlcmd2);    

    return ret;
}

int updatestatusexecstatus(MYSQL connection,char *defaultcamid,int capexecstatus)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set CapExecStatus=%d where CamId='%s'",capexecstatus,defaultcamid);  

    
    {
        LOGFMTD("Updatecamstatus sqlcmd %s",sqlcmd);
    }

    return updatecmd(connection,sqlcmd);
}

int querycamname(MYSQL connection,char *defaultgearname,char *camname)
{
    char sqlcmd[128];
 
    sprintf(sqlcmd,"SELECT CamName FROM gear WHERE GearName='%s'",defaultgearname);                
    LOGFMTD("Querycamname sqlcmd %s",sqlcmd);

    return querystring(connection,sqlcmd,camname);
}

int querycamtemperature(MYSQL connection,char *defaultcamid,double *camtemperature)
{
    char sqlcmd[128];
 
    sprintf(sqlcmd,"SELECT Temperature FROM status WHERE CamId='%s'",defaultcamid);                
    LOGFMTD("QueryTemperature sqlcmd %s",sqlcmd);

    return querydouble(connection,sqlcmd,camtemperature);
} 

int deletelastimage(MYSQL connection,char *defaultcamid,int capmode)
{
    int ret = 0;
    char sqlcmd[128];
    char tmpname[36];
    char pathname[128];

    if(capmode == 2)
        return 0;

    switch(capmode)
    {
        case 0:
        {
             sprintf(sqlcmd,"SELECT LastPreviewImageName FROM status WHERE CamId='%s'",defaultcamid);
        }
        break;
        case 1:
        {
             sprintf(sqlcmd,"SELECT LastFocusImageName FROM status WHERE CamId='%s'",defaultcamid);
        }
        break;
    }

    LOGFMTD("Querycamstatus sqlcmd %s",sqlcmd);


    querystring(connection,sqlcmd,tmpname);

    sprintf(pathname,"./%s.fit",tmpname);

    remove(pathname);
}

int querycamcmdrundone(MYSQL connection,int *rundone)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT RunDone FROM camcmd");                
        
    LOGFMTD("Sqlcmd %s",sqlcmd);

    int ret = queryint(connection,sqlcmd,rundone);
    if(ret)
    {
        LOGFMTD("querycamcmdrundone error");
    }
   
    LOGFMTD("camcmd rundone %d",*rundone);

    return ret;
}

int querycamcmdcamid(MYSQL connection,char *camid)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT CamId FROM camcmd");                
        
    LOGFMTD("Sqlcmd %s",sqlcmd);

    int ret = querystring(connection,sqlcmd,camid);
    if(ret)
    {
        LOGFMTD("querycamcmdcamid error");
    }
  
    LOGFMTD("camcmd camid %s",camid);

    return ret;
}

int querycamcmdusername(MYSQL connection,char *username)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT UserName FROM camcmd");                
        
    LOGFMTD("Sqlcmd %s",sqlcmd);

    int ret = querystring(connection,sqlcmd,username);
    if(ret)
    {
        LOGFMTD("querycamcmdusername error");
    }
    
    LOGFMTD("camcmd username %s",username);

    return ret;
}

int querycamcmdcfwpos(MYSQL connection,int *cfwpos)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT CFWPOS FROM camcmd");                
        
    LOGFMTD("Sqlcmd %s",sqlcmd);

    int ret = queryint(connection,sqlcmd,cfwpos);
    if(ret)
    {
        LOGFMTD("querycamcmdcfwpos error");
    }
    
    LOGFMTD("camcmd cfwpos %d",*cfwpos);

    return ret;
}

int querycamcmdcmdtype(MYSQL connection,int *cmdtype)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT CmdType FROM camcmd");                
    LOGFMTD("Sqlcmd %s",sqlcmd);

    int ret = queryint(connection,sqlcmd,cmdtype);
    if(ret)
    {
        LOGFMTD("querycamcmdcmdtype error");
    }

    LOGFMTD("camcmd cmdtype %d",*cmdtype);
    return ret;
}

int querytargettemperature(MYSQL connection,char *defaultcamid,double *temprunstatus)
{
    char sqlcmd[128];
 
    sprintf(sqlcmd,"SELECT TargetTemperature FROM status WHERE CamId='%s'",defaultcamid);                
    LOGFMTD("QueryTargetTemperature sqlcmd %s",sqlcmd);

    return querydouble(connection,sqlcmd,temprunstatus);
}

int updatecamcmdcamid(MYSQL connection,char *username,char *camid)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE camcmd set CamId='%s' where UserName='%s'",camid,username);
    LOGFMTD("updatecamcmdcamid sqlcmd %s",sqlcmd);
   
    return updatecmd(connection,sqlcmd);
}

int updatecamcmdrundone(MYSQL connection,char *camid,int rundone)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE camcmd set RunDone=%d where CamId='%s'",rundone,camid);
    LOGFMTD("Updatecamcmdrundone sqlcmd %s",sqlcmd);
   
    return updatecmd(connection,sqlcmd);
}

int updatecamtemperature(MYSQL connection,char *defaultcamid,double camtemperature)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set Temperature=%f where CamId='%s'",camtemperature,defaultcamid);
    LOGFMTD("Updatecamstatus sqlcmd %s",sqlcmd);
  
    return updatecmd(connection,sqlcmd);
}


void fitwrite(char *filename,struct FITPARAS fitParas,int ImageWidth, int ImageHeight, unsigned char *Buf)
{
    IplImage *FitImg;

    FitImg = cvCreateImage(cvSize(ImageWidth,ImageHeight), IPL_DEPTH_16U, 1);
    FitImg->imageData = (char*)Buf;

    fitsfile *fptr;
    int status;//, ii, jj;
    long fpixel = 1, naxis = 2, nelements;//, exposure;
    long naxes[2] = {ImageWidth, ImageHeight};
    char ss[81];

    status = 0;

    fits_create_file(&fptr, filename, &status);

    fits_create_img(fptr, USHORT_IMG, naxis, naxes, &status);

    fits_update_key(fptr, TLONG, (char *)"EXPOSURE", &fitParas.Exptime ,(char *)"Total Exposure Time[ms]", &status);

    if(fitParas.Gain >= 0)
    {
        fits_update_key(fptr, TINT, (char *)"GAIN", &fitParas.Gain,(char *)"Analog Gain", &status);
    }

    if(fitParas.Offset >= 0)
    {
        fits_update_key(fptr, TINT, (char *)"OFFSET", &fitParas.Offset,(char *)"Analog Offset", &status);
    }

    fits_update_key(fptr, TSTRING, (char *)"TIME", fitParas.Time_Fit,(char *)"Begin captuer time", &status);

    fits_update_key(fptr, TSTRING, (char *)"DATE", fitParas.Date_Fit,(char *)"Capture Date", &status);

    fits_update_key(fptr, TSTRING, (char *)"Camera",fitParas.CamName,(char *)"Camera model", &status);

    sprintf(ss,"%f",fitParas.ImageTEMP);
    fits_update_key(fptr, TSTRING, (char *)"Temperature", &ss,(char *)"Begin capture temperature", &status);

    fits_update_key(fptr, TSTRING, (char *)"Presion Time",fitParas.ImageTimeMS,(char *)"Begin capture Presion Time", &status);

    nelements = naxes[0] * naxes[1];

    fits_write_img(fptr, TUSHORT, fpixel, nelements, &FitImg->imageData[0], &status);
    fits_close_file(fptr, &status);
    fits_report_error(stderr, status);

    FitImg->imageData=0;
    cvReleaseImage(&FitImg);
}

int getcurrentdatetime(char *datetimebuffer)
{
    time_t timep;
    struct tm *p;

    /*获得time_t结构的时间，UTC时间*/
    time(&timep);
    /*转换为struct tm结构的当地时间*/
    p = localtime(&timep); 

    /*save begin datetime*/
    sprintf(datetimebuffer,"%d-%d-%d %d:%d:%d",1900 + p->tm_year,1 + p->tm_mon,p->tm_mday,
                                              p->tm_hour,p->tm_min,p->tm_sec);
    return 0;
}

int setfitheaderinfo(struct FITPARAS *fitParas,char *camname,int camexp,int camgain,int camoffset,int camtemperature)
{
    time_t timep;
    struct tm *p;

    /*获得time_t结构的时间，UTC时间*/
    time(&timep);
    /*转换为struct tm结构的当地时间*/
    p = localtime(&timep); 

    sprintf(fitParas->Date_Fit,"%d-%d-%d",1900 + p->tm_year,1 + p->tm_mon,p->tm_mday);
    sprintf(fitParas->Time_Fit,"%d:%d:%d",p->tm_hour,p->tm_min,p->tm_sec);
    sprintf(fitParas->ImageTimeMS,"%d:%d:%d:%d",p->tm_hour,p->tm_min,p->tm_sec,0);

    strcpy(fitParas->CamName,camname);
    fitParas->Gain = camgain;
    fitParas->Offset = camoffset;
    fitParas->Exptime = camexp; 
    fitParas->ImageTEMP = camtemperature;

    return 0;
}

int waitforexpscheduleandupdatestatus(qhyccd_handle *camhandle,int camexp,MYSQL connection,char *defaultcamid)
{
    double expschedule;
    double curexpschedule;
    unsigned char tmpbuf[64];
    int ret = 1;

    /*update exposure schedule*/
    curexpschedule = 0;

    ret = GetQHYCCDCameraStatus(camhandle,tmpbuf);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("GetQHYCCDCameraStatus");
    }

    while(curexpschedule < camexp && camexp > 1000 && tmpbuf[0] != 3)
    {
        curexpschedule = tmpbuf[4] * 256 * 256 * 256 + tmpbuf[5] * 256 * 256 + tmpbuf[6] * 256 + tmpbuf[7];
        expschedule = (camexp - curexpschedule) / camexp * 100;
        ret = updatecamexpschedule(connection,defaultcamid,expschedule);
        if(ret)
        {
            LOGD("updatecamexpschedule");
        }

        ret = GetQHYCCDCameraStatus(camhandle,tmpbuf);
        if(ret != QHYCCD_SUCCESS)
        {
            LOGD("GetQHYCCDCameraStatus");
        }
    }
    expschedule = 100.0;
    ret = updatecamexpschedule(connection,defaultcamid,expschedule);
    if(ret)
    {
        LOGD("updatecamexpschedule");
    }

    return 0;
}

int waitforcamready(MYSQL connection,char *defaultcamid)
{
    int camstatus = -1;

    do
    {
        /*get cam status */
        int ret = querycamstatus(connection,defaultcamid,&camstatus);
        if(ret)
        {
            LOGD("querycamstatus");
        }

        LOGFMTD("camstatus != IDLE,waitting for IDLE %s",defaultcamid); 
        usleep(1000);
    }while(camstatus != IDLE);

    return 0;
}

int queryparamsandsetcam(qhyccd_handle *camhandle,MYSQL connection,char *username,char *defaultgearname,int *camexp,int *camgain,int *camoffset,int *camcapmode)
{
    int cambin,camspeed,focstartx,focstarty;
    int ret = 1;

    /*get cambin camspeed camcapmode*/
    ret = querycamcmdcambinspeedcapmode(connection,username,camexp,&cambin,&camspeed,camcapmode,&focstartx,&focstarty);
    if(ret)
    {
        LOGD("querycamcmdcambinspeedcapmode");
    }

    /*get gain and offset*/
    ret = querygeargainoffset(connection,defaultgearname,cambin,camspeed,camgain,camoffset);
    if(ret)
    {
        LOGD("querygeargainoffset");
    }

    /*set params to cam*/
    ret = SetQHYCCDParam(camhandle,CONTROL_GAIN,*camgain);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("SetQHYCCDParam,CONTROL_GAIN");
    }

    SetQHYCCDParam(camhandle,CONTROL_OFFSET,*camoffset);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("SetQHYCCDParam,CONTROL_OFFSET");
    }

    SetQHYCCDParam(camhandle,CONTROL_EXPOSURE,*camexp * 1000);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("SetQHYCCDParam,CONTROL_EXPOSURE");
    }

    SetQHYCCDBinMode(camhandle,cambin / 10,cambin % 10);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("SetQHYCCDBinMode");
    }

    if(*camcapmode == 1)
    {
        SetQHYCCDFocusSetting(camhandle,focstartx,focstarty);
    }

    return 0;
}


void *readusbsync(void *pa)
{
    struct CAPCTROL *capparams = (struct CAPCTROL *)pa;
    qhyccd_handle *camhandle = capparams->camh;
    unsigned char *pdata = capparams->imageData;

    unsigned int w,h,bpp,channels;
    int ret = GetQHYCCDSingleFrame(camhandle,&w,&h,&bpp,&channels,pdata);
    if(ret == QHYCCD_SUCCESS)
    {
        capparams->imageW = w;
        capparams->imageH = h;
    }
    else
    {
        LOGD("GetQHYCCDSingleFrame");
    }
}

int genguid(char *guid)
{
    uuid_t uu;
    uuid_t uuid;
    
    /*gen GUID*/
    uuid_generate(uuid);
    uuid_unparse(uuid,guid);
}

int waitforcfwrunready(qhyccd_handle *camhandle,int cfw)
{
    char dstcfw,cfwpos;
    sprintf(&dstcfw,"%d",cfw - 1);
    int ret = SendOrder2QHYCCDCFW(camhandle,&dstcfw,1);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("SendOrder2QHYCCDCFW");
        return 2;
    }


    while(dstcfw != cfwpos)
    {
        /*make sure the cfw done*/
        SendOrder2QHYCCDCFW(camhandle,(char *)"NOW",3);

        ret = GetQHYCCDCFWStatus(camhandle,&cfwpos);  
        if(ret != QHYCCD_SUCCESS)
        {
            LOGD("GetQHYCCDCFWStatus");
            return 2;
        }
 
        usleep(200000);
        LOGD("cfw running...");
    }

    return 0;
}

int waitforreadusbscheduleandupdatestatus(qhyccd_handle *camhandle,MYSQL connection,char *camid)
{
    int ret = 0;
    double readusbschedule = 0;

    while(readusbschedule != 100.0)
    {
        readusbschedule = GetQHYCCDReadingProgress(camhandle);
        ret = updatecamreadusbschedule(connection,camid,readusbschedule);
        if(ret)
        {
            LOGD("updatecamreadusbschedule");
        }
        LOGFMTD("updatecamreadusbschedule %f",readusbschedule);
        usleep(10000);
    }

    readusbschedule = 100;
    ret = updatecamreadusbschedule(connection,camid,readusbschedule);
    if(ret)
    {
        LOGD("updatecamreadusbschedule");
    }

    return ret;
}

int waitforreaddone(MYSQL connection,char *defaultcamid)
{
    int ret = 1,camstatus;

    do
    {
        ret = querycamstatus(connection,defaultcamid,&camstatus);
        if(ret)
        {
            LOGFMTD("querycamstatus"); 
            return ret;
        }
        LOGW("camstatus = READING,waitting for not READING");
        usleep(10000);
    }while(camstatus == READING);

    return ret;
}

int handleplanner(MYSQL connection,char *username,char *gearname,char *plannername,char *camid,qhyccd_handle *camhandle)
{
    char sqlcmd[256];
    char camname[64];
    int resultnum;
    MYSQL_RES *resptr;
    int camgain;
    int camoffset;
    int camstatus;
    double camtemperature;
    struct FITPARAS fitParas;
    char imagename[64];
    char begindatetime[64];
    char enddatetime[64];
    char savepath[128];

    int ret = 0;
    
    sprintf(sqlcmd,"SELECT IsUse,BIN,ExpTime,RepeatNum,CFW,Delay FROM planner WHERE(UserName='%s' AND PlannerName='%s')",username,plannername);                
    LOGFMTD("Sqlcmd %s",sqlcmd);

    int res = mysql_query(&connection,sqlcmd);    
    if(res)
    {
        LOGFMTD("querycmd error: %s",mysql_error(&connection));
    }

    resptr = mysql_store_result(&connection);
    if(resptr)
    {
        resultnum = mysql_num_rows(resptr);
    }
    else
    {
        LOGD("mysql_store_result");
    } 

    int resultfields = 0;
    resultfields = mysql_num_fields(resptr);

    MYSQL_ROW sqlrow;
    int i = 0;
    int isuse,bin,exptime,repeatnum,cfw,delay;


    double curplanindex = 0;
    updatestatustotalplanschedule(connection,camid,curplanindex / resultnum);

    while((sqlrow = mysql_fetch_row(resptr)))
    {
        isuse = atoi(sqlrow[0]);
        bin = atoi(sqlrow[1]);
        exptime = atoi(sqlrow[2]);
        repeatnum = atoi(sqlrow[3]);
        cfw = atoi(sqlrow[4]);
        delay = atoi(sqlrow[5]);

        if(isuse)
        {
            /*get gain and offset*/
            ret = querygeargainoffset(connection,gearname,bin,0,&camgain,&camoffset);
            if(ret)
            {
                LOGD("querygeargainoffset");
                return 2;
            }

            /*set params to cam*/
            ret = SetQHYCCDParam(camhandle,CONTROL_GAIN,camgain);
            if(ret != QHYCCD_SUCCESS)
            {
                LOGD("SetQHYCCDParam,CONTROL_GAIN");
                return 2;
            }

            ret = SetQHYCCDParam(camhandle,CONTROL_OFFSET,camoffset);
            if(ret != QHYCCD_SUCCESS)
            {
                LOGD("SetQHYCCDParam,CONTROL_OFFSET");
                return 2;
            }

            ret = SetQHYCCDParam(camhandle,CONTROL_EXPOSURE,exptime * 1000);
            if(ret != QHYCCD_SUCCESS)
            {
                LOGD("SetQHYCCDParam,CONTROL_EXPOSURE");
                return 2;
            }

            ret = SetQHYCCDBinMode(camhandle,bin / 10,bin % 10);
            if(ret != QHYCCD_SUCCESS)
            {
                LOGD("SetQHYCCDParam,CONTROL_EXPOSURE");
                return 2;
            }


            waitforcfwrunready(camhandle,cfw);
   
            /*get camname*/
            ret = querycamname(connection,gearname,camname);
            if(ret)
            {
                LOGD("querycamname");
            }

            int currepeat = 0;
           
            while(currepeat < repeatnum)
            {
                /*get ccd's temperature*/
                ret = querycamtemperature(connection,camid,&camtemperature); 
                if(ret)
                {
                    LOGD("querycamtemperature");
                } 

                strcpy(fitParas.CamName,camname);
                fitParas.ImageTEMP = camtemperature;
                fitParas.Gain = camgain;
                fitParas.Offset = camoffset;
                fitParas.Exptime = exptime; 

                genguid(imagename);

                waitforcamready(connection,camid);

                /*begin cam exposure*/
                ret = ExpQHYCCDSingleFrame(camhandle);
                if(ret != QHYCCD_SUCCESS)
                {
                    LOGD("ExpQHYCCDSingleFrame");
                    return 2;
                }

                getcurrentdatetime(begindatetime);

                setfitheaderinfo(&fitParas,camname,exptime,camgain,camoffset,camtemperature);

                /*exposure start,update camstatus*/
                camstatus = EXPSING;
                ret = updatecamstatus(connection,camid,camstatus);
                if(ret)
                {
                    LOGD("updatecamstatus");
                }

                waitforexpscheduleandupdatestatus(camhandle,exptime,connection,camid);

                getcurrentdatetime(enddatetime);

                /*exposure end,update camstatus*/
                camstatus = READING;
                ret = updatecamstatus(connection,camid,camstatus);
                if(ret)
                {
                    LOGD("updatecamstatus");
                }

                unsigned char *imgdata = (unsigned char *)malloc(GetQHYCCDMemLength(camhandle));
                struct CAPCTROL usbparams;
                usbparams.camh = camhandle;
                usbparams.imageData = imgdata;

                pthread_t ntid;
                ret = pthread_create(&ntid,NULL,readusbsync,&usbparams);
                if (ret != 0)
                {
                    LOGFMTD("Can't create thread for readUSB:%s", strerror(ret));
                }

                waitforreadusbscheduleandupdatestatus(camhandle,connection,camid);

                pthread_join(ntid,NULL);
                int w = usbparams.imageW,h = usbparams.imageH;
                sprintf(savepath,"./%s.fit",imagename);
                fitwrite(savepath,fitParas,w,h,imgdata);
                free(imgdata);

                camstatus = IDLE;
                ret = updatecamstatus(connection,camid,camstatus);
                if(ret)
                {
                    LOGD("updatecamstatus");
                }

                ret = insertimagelog(connection,imagename,begindatetime,enddatetime);
                if(ret)
                {
                    LOGD("insertimagelog");
                }

                ret = updatestatuslastimage(connection,CAPTUREMODE,camid,imagename);
                if(ret)
                {
                    LOGD("updatestatuslastimage");
                }

                currepeat++;
                usleep(delay * 1000);
            }
        }
        curplanindex++;
        updatestatustotalplanschedule(connection,camid,curplanindex / resultnum * 100);

    }
    mysql_free_result(resptr);

    return 0;    
}


void *planner(void *pa)
{
    struct CAPCTROL *capparams = (struct CAPCTROL *)pa;
    char *username = capparams->username;
    qhyccd_handle *camhandle = capparams->camh;

    char defaultgearname[64];
    char defaultcamid[64];
    char defaultplannername[64];

    int ret = 0;
  
    pthread_mutex_lock(&mutexplanner);

    if(camhandle != NULL)
    {
        MYSQL connection;
        MYSQL_RES *resptr;
        /*初始化mysql连接*/
        mysql_init(&connection);
	
        /*建立与数据库的连接*/
        if(mysql_real_connect(&connection, "localhost", "root", "root", "ic8300", 0, NULL, 0))
        {
            /*get gearname*/
            ret = queryusergearname(connection,username,defaultgearname);
            if(ret)
            {
                LOGD("queryusergearname");
            }

            /*get camid*/
            ret = queryusercamid(connection,username,defaultcamid);
            if(ret)
            {
                LOGD("queryusercamid");
            }

            /*get plannername*/
            ret = queryuserplannername(connection,username,defaultplannername);
            if(ret)
            {
                LOGD("queryuserplannername");
            }

            int resultnum = 0;
            ret = handleplanner(connection,username,defaultgearname,defaultplannername,defaultcamid,camhandle);
            if(ret == 2)
            {
                LOGD("handleplanner");
            }
        
            int rundone = 1;
            ret = updatecamcmdrundone(connection,defaultcamid,rundone);
            if(ret)
            {
                LOGD("updatecamcmdrundone");
            }
            mysql_close(&connection);
            mysql_library_end();
        }
        else
        {
            LOGD("mysql connect error");  
        }       
                      
    }
    else
    {
        LOGD("lost usb connect");
    }

    pthread_mutex_unlock(&mutexplanner);

    pthread_exit(NULL);
} 



void *cap(void *pa)
{
    struct CAPCTROL *capparams = (struct CAPCTROL *)pa;
    char *username = capparams->username;
    qhyccd_handle *camhandle = capparams->camh;

    int ret;

    char defaultgearname[64];
    char defaultcamid[64];
    char imagename[64];
    char begindatetime[64];
    char enddatetime[64];
    char savepath[128];
    char camname[64];
    int camexp;
    int camgain;
    int camoffset;
    int cambin;
    int camspeed; 
    int camcapmode;
    int camstatus;

    double readusbschedule = 0;
    double camtemperature;
    struct FITPARAS fitParas;

  
    pthread_detach(pthread_self());

    pthread_mutex_lock(&mutexcap);

    if(camhandle != NULL)
    {

        /*初始化mysql连接*/
        //mysql_init(&connection);
	
        /*建立与数据库的连接*/
        if(1)//mysql_real_connect(&connection, "localhost", "root", "root", "ic8300", 0, NULL, 0))
        {
            /*get gearname*/
            ret = queryusergearname(capcon,username,defaultgearname);
            if(ret)
            {
                LOGD("queryusergearname");
            }

            /*get camid*/
            ret = queryusercamid(capcon,username,defaultcamid);
            if(ret)
            {
                LOGD("queryusercamid");
            }

            queryparamsandsetcam(camhandle,capcon,username,defaultgearname,&camexp,&camgain,&camoffset,&camcapmode);


            deletelastimage(capcon,defaultcamid,camcapmode);

            /*get camname*/
            ret = querycamname(capcon,defaultgearname,camname);
            if(ret)
            {
                LOGD("querycamname");
            }
                
            /*get ccd's temperature*/
            ret = querycamtemperature(capcon,defaultcamid,&camtemperature);  
            if(ret)
            {
                LOGD("querycamtemperature");
            }

            genguid(imagename);

            waitforcamready(capcon,defaultcamid);

            /*begin cam exposure*/
            ret = ExpQHYCCDSingleFrame(camhandle);
            if(ret != QHYCCD_SUCCESS)
            {
                LOGD("ExpQHYCCDSingleFrame");
            }

            getcurrentdatetime(begindatetime);

            setfitheaderinfo(&fitParas,camname,camexp,camgain,camoffset,camtemperature);

            /*exposure start,update camstatus*/
            camstatus = EXPSING;
            ret = updatecamstatus(capcon,defaultcamid,camstatus);
            if(ret)
            {
                LOGD("updatecamstatus");
            }

            waitforexpscheduleandupdatestatus(camhandle,camexp,capcon,defaultcamid);

            getcurrentdatetime(enddatetime);

            /*exposure end,update camstatus*/
            camstatus = READING;
            ret = updatecamstatus(capcon,defaultcamid,camstatus);
            if(ret)
            {
                LOGD("updatecamstatus");
            }

            unsigned char *imgdata = (unsigned char *)malloc(GetQHYCCDMemLength(camhandle));
            struct CAPCTROL usbparams;
            usbparams.camh = camhandle;
            usbparams.imageData = imgdata;

            pthread_t ntid;
            ret = pthread_create(&ntid,NULL,readusbsync,&usbparams);
            if (ret != 0)
            {
                LOGFMTD("Can't create thread for readUSB:%s", strerror(ret));
            }

            waitforreadusbscheduleandupdatestatus(camhandle,capcon,defaultcamid);

            pthread_join(ntid,NULL);
            int w = usbparams.imageW,h = usbparams.imageH;
            sprintf(savepath,"./%s.fit",imagename);
            fitwrite(savepath,fitParas,w,h,imgdata);

            IplImage *imgstretch = cvCreateImage(cvSize(w,h),8,1);

            system("rm -fr *.jpg");
            sprintf(savepath,"./%s-0.jpg",imagename);
            Bits16ToBits8(camhandle,imgdata,(unsigned char *)imgstretch->imageData,w,h,0,65535);
            cvSaveImage(savepath,imgstretch);

            sprintf(savepath,"./%s-1.jpg",imagename);
            Bits16ToBits8(camhandle,imgdata,(unsigned char *)imgstretch->imageData,w,h,10000,55535);
            cvSaveImage(savepath,imgstretch);

            sprintf(savepath,"./%s-2.jpg",imagename);
            Bits16ToBits8(camhandle,imgdata,(unsigned char *)imgstretch->imageData,w,h,20000,45535);
            cvSaveImage(savepath,imgstretch);

            sprintf(savepath,"./%s-3.jpg",imagename);
            Bits16ToBits8(camhandle,imgdata,(unsigned char *)imgstretch->imageData,w,h,30000,35535);
            cvSaveImage(savepath,imgstretch);

            cvReleaseImage(&imgstretch);

            IplImage *histimg = cvCreateImage(cvSize(192,130),8,3);
            sprintf(savepath,"./%s-hist.jpg",imagename);
            HistInfo192x130(camhandle,w,h,imgdata,(unsigned char *)histimg->imageData);
            cvSaveImage(savepath,histimg);
            cvReleaseImage(&histimg);
           
            free(imgdata);

            camstatus = IDLE;
            ret = updatecamstatus(capcon,defaultcamid,camstatus);
            if(ret)
            {
                LOGD("updatecamstatus");
            }

            ret = insertimagelog(capcon,imagename,begindatetime,enddatetime);
            if(ret)
            {
                LOGD("insertimagelog"); 
            }

            ret = updatestatuslastimage(capcon,camcapmode,defaultcamid,imagename);
            if(ret)
            {
                LOGD("updatestatuslastimage"); 
            }
                  
            int rundone = 1;
            ret = updatecamcmdrundone(capcon,defaultcamid,rundone);
            if(ret)
            {
                LOGD("updatecamcmdrundone");
            }
            //mysql_close(&connection);
            //mysql_thread_end();
            //mysql_library_end();
        }
        else
        {
            LOGD("mysql connection failure");
            if(mysql_errno(&capcon))
            {
                LOGFMTD("mysql connection error %d: %s",mysql_errno(&capcon),mysql_error(&capcon));
            } 
        }       
                      
    }
    pthread_mutex_unlock(&mutexcap);

    pthread_exit(NULL);
} 

int autocontroltemperature(MYSQL connection,char *defaultcamid,qhyccd_handle *camhandle)
{
    double camtemperature = 24.1;
    double camtargettemperature = -10.0;
    int camstatus = -1;
    int ret = 0;

    LOGT("autocontroltemperature");

    if(camhandle != NULL)
    {   
        /*get target temperature*/
        ret = querytargettemperature(connection,defaultcamid,&camtargettemperature);
        if(ret)
        {
            LOGFMTD("querytargettemperature");  
        }
        else
        {
            LOGFMTD("querytargettemperature %f",camtargettemperature);
        }

        waitforreaddone(connection,defaultcamid);

        ret = ControlQHYCCDTemp(camhandle,camtargettemperature);
        if(ret != QHYCCD_SUCCESS)
        {
            LOGFMTD("ControlQHYCCDTemp");
            return 2;
        }

        camtemperature = GetQHYCCDParam(camhandle,CONTROL_CURTEMP);
        LOGFMTD("GetQHYCCDParam %f",camtemperature);

        ret = updatecamtemperature(connection,defaultcamid,camtemperature);
        if(ret)
        {
            LOGFMTD("updatecamtemperature");
        }
        else
        {
            LOGFMTD("CamTemperature = %f",camtemperature);
        }         
    }
    else
    {
        LOGFMTD( "camera lost connect");  
        return 1;
    }
    
    return 0;
}

int hardwarereset()
{
    system("./mpsse 1");
    usleep(10000);
    system("./mpsse 0");
    sleep(5);
    return 0;
}


int controlcfwmoving(MYSQL connection,qhyccd_handle *camhandle)
{
    int cfwpos;
    char dstcfw;
 
    querycamcmdcfwpos(connection,&cfwpos);
    sprintf(&dstcfw,"%d",cfwpos);
    
    int ret = SendOrder2QHYCCDCFW(camhandle,&dstcfw,1);
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("SendOrder2QHYCCDCFW");
        return 2;
    }

    return 0;
}

int main(int argc, char *argv[])
{

    MYSQL connection;

    char username[64];
    char camid[64];
    
    int timenum = 0;    

    int rundone,cmdtype;

    int ret = 0;

    int numcam = 0;

    int isexist = 1;

    hardwarereset();

    resetcamera();


    pthread_mutex_init(&mutexcap,NULL);
    pthread_mutex_init(&mutexplanner,NULL);

    ret = InitQHYCCDResource();
    if(ret != QHYCCD_SUCCESS)
    {
        LOGD("InitQHYCCDResource failure");
        goto EXIT_INTERCAM;
    }

    numcam = ScanQHYCCD();
    if(numcam <= 0)
    {
        LOGD("USB link error or other problems?");
        goto EXIT_INTERCAM;
    }
    
    ret = GetQHYCCDId(0,camid);
    if(ret == QHYCCD_SUCCESS)
    {
        LOGFMTD("connected cam id is %s",camid);
    }

    isstatuscamidexistandinsert(camid);

    /*初始化mysql连接*/
    mysql_init(&connection);

    /*建立与数据库的连接*/
    if(mysql_real_connect(&connection, "localhost", "root", "root", "ic8300", 0, NULL, 0))
    {     
        ret = querycamcmdusername(connection,username);
        if(ret)
        {
            LOGD("querycamcmdusername");
        }

        ret = updatecamcmdcamid(connection,username,camid);
        if(ret)
        {
            LOGD("updatecamcmdcamid");
        }

        /*get camcmd camid*/    
        ret = querycamcmdcamid(connection,camid);
        if(ret)
        {
            LOGD("querycamcmdcamid");
        }

        /*open the camera*/
        qhyccd_handle *camhandle = OpenQHYCCD(camid);

        if(camhandle)
        {
            InitQHYCCD(camhandle);
        }
        else
        {
            LOGD("camera init failure");
            goto EXIT_INTERCAM;
        }

        struct CAPCTROL capparams;
        capparams.username = username;
        capparams.camh = camhandle;

        /*global capcon mysql connect for cap thread */
        mysql_init(&capcon);
        mysql_real_connect(&capcon, "localhost", "root", "root", "ic8300", 0, NULL, 0);
        

       
        /*exposure start,update camstatus*/
        int camstatus = IDLE;
        ret = updatecamstatus(connection,camid,camstatus);
        if(ret)
        {
            LOGD("updatecamstatus");
        }

        while(1)
        {
            /*get camcmd rundone status*/
            ret = querycamcmdrundone(connection,&rundone);
            if(ret)
            {
                LOGD("querycamcmdrundone");
            }
            
            if(rundone == 0)
            {
                rundone = 2;
                ret = updatecamcmdrundone(connection,camid,rundone);
                if(ret)
                {
                    LOGD("updatecamcmdrundone");
                }

                ret = querycamcmdcmdtype(connection,&cmdtype);
                if(ret)
                {
                    LOGD("querycamcmdcmdtype");
                }

                if(cmdtype == 0)
                {
                    LOGD("Recive camcmd CaptureMode");
      
                    pthread_t ntid;
                    ret = pthread_create(&ntid,NULL,cap,&capparams);
                    if (ret != 0)
                    {
                        LOGFMTD("Can't create thread for cap:%s", strerror(ret));
                        goto EXIT_INTERCAM;
                    }
                    //pthread_join(ntid,NULL);
                }
                else if(cmdtype == 1)
                {
                    LOGD("Recive camcmd PlannerMode");
                   
                    pthread_t ntid;
                    ret = pthread_create(&ntid,NULL,planner,&capparams);
                    if (ret != 0)
                    {
                        LOGFMTD("Can't create thread for planner: %s", strerror(ret));
                        goto EXIT_INTERCAM;
                    }
                    //pthread_join(ntid,NULL);
                }
                else if(cmdtype == 3)
                {
                    LOGD("Recive camcmd CFW moving");
     
                    controlcfwmoving(connection,camhandle);
		    
                    int rundone = 1;
		    ret = updatecamcmdrundone(connection,camid,rundone);
		    if(ret)
		    {
		        LOGD("updatecamcmdrundone");
		    }
                }

            }   
            else if(rundone == 4)
            {
                ret = CancelQHYCCDExposing(camhandle);
                if(ret != QHYCCD_SUCCESS)
                {
                    LOGF("CancelQHYCCDExposing");
                }
		int rundone = 1;
		ret = updatecamcmdrundone(connection,camid,rundone);
		if(ret)
		{
		    LOGD("updatecamcmdrundone");
		}
            }
      
            if(timenum == 100)
            {
                timenum = 0;
                ret = autocontroltemperature(connection,camid,camhandle);
                if(ret == 1)
                {
                    LOGFMTD("autocontroltemperature"); 
                }
                else if(ret == 2)
                {
                    LOGFMTD("autocontroltemperature");
                    goto EXIT_INTERCAM;
                }
            }
            timenum++;
            usleep(10000);
        }
        CloseQHYCCD(camhandle);
        mysql_close(&connection);
        mysql_close(&capcon);
        mysql_library_end();
    }
    else
    {
         LOGD("mysql connection failure");
         if(mysql_errno(&connection))
         {
             LOGFMTD("mysql connection error %d: %s",mysql_errno(&connection),mysql_error(&connection));
         }  
    }
    ReleaseQHYCCDResource();   

    pthread_mutex_destroy(&mutexcap);
    pthread_mutex_destroy(&mutexplanner);

    return 0;  

EXIT_INTERCAM:
    LOGD("Exit with fatal error");
    return 1;
}
