#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include <uuid/uuid.h>
#include <string.h>
#include <unistd.h>
#include <libqhy/qhyccd.h>

#define DEBUG

#define IDLE                0
#define EXPSING             1
#define READING             2
#define CONTROLTEMPERATURE  3

#define EXECBUSY          2
#define EXECEXIT          1
#define EXECSTATUSIDLE    0
#define CAMSTATUSNOTIDLE -1
#define CAMNOTEXIST      -2
#define CAMCONNECTERROR  -3


int updatecmd(MYSQL connection,char *sqlcmd)
{    
    int res = mysql_query(&connection,sqlcmd);    
    if(res)
    {
        fprintf(stderr,"updatecmd error: %s\n",mysql_error(&connection));
    }
    return 0;
}

int updatecamstatus(MYSQL connection,char *defaultcamid,int camstatus)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set CamStatus=%d where CamId='%s'",camstatus,defaultcamid);
    #ifdef DEBUG
    printf("Updatecamstatus sqlcmd %s\n",sqlcmd);
    #endif

    return updatecmd(connection,sqlcmd);
}

int updatedefaultcamid(MYSQL connection,char *username,char *camid)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE user set DefaultCamId='%s' where UserName='%s'",camid,username);  

    #ifdef DEBUG
    printf("Updatecamstatus sqlcmd %s\n",sqlcmd);
    #endif

    return updatecmd(connection,sqlcmd);
}

int updatecamconnectstatus(MYSQL connection,char *camid,int camconnectstatus)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"UPDATE status set CamConnectStatus=%d where CamId='%s'",camconnectstatus,camid);  

    #ifdef DEBUG
    printf("Updatecamstatus sqlcmd %s\n",sqlcmd);
    #endif

    return updatecmd(connection,sqlcmd);
}

int main(int argc, char *argv[])
{
    MYSQL connection;

    int res;
    
    char username[36];
    char userpd[36];
    char camid[36];

    int camstatus = -1;
    int camconnectstatus;

    if(argc != 3)
    {
        fprintf(stderr, "parameters error\n");
        return 1;
    }
     
    sprintf(username,"%s",argv[1]);
    sprintf(userpd,"%s",argv[2]);
#ifdef DEBUG
    printf("Parameters: username=%s,userpd=%s\n",username,userpd);
#endif
    /*初始化mysql连接*/
    mysql_init(&connection);

    /*建立与数据库的连接*/
    if(mysql_real_connect(&connection, "localhost", "root", "QHYCCD", "ic8300", 0, NULL, 0))
    {   
        InitQHYCCDResource();

        int numcam = ScanQHYCCD();

        if(numcam > 0)
        {
            int ret = GetQHYCCDId(0,camid);
 
            if(ret == QHYCCD_SUCCESS)
            {
                updatedefaultcamid(connection,username,camid);   

                camconnectstatus = IDLE;
                updatecamconnectstatus(connection,camid,camconnectstatus);    
            }    
        }
        else
        {
            updatedefaultcamid(connection,username,NULL);   

            camconnectstatus = CAMNOTEXIST;
            updatecamconnectstatus(connection,camid,camconnectstatus);    
            fprintf(stderr, "cam connect failed\n");
        }
    }
    else
    {
        fprintf(stderr, "mysql connection failed\n");
        if(mysql_errno(&connection))
        {
            fprintf(stderr, "mysql connection error %d: %s\n",mysql_errno(&connection),mysql_error(&connection));
        }     
    }

    ReleaseQHYCCDResource();

    return EXIT_SUCCESS;
} 
