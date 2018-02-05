#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <mysql/mysql.h>
#include "log4z.h"

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
        LOGFMTE("select error: %s",mysql_error(&connection));            
    }
    else
    {
        /*将查询的結果给res_ptr*/
        res_ptr = mysql_store_result(&connection);

        if (res_ptr) 
        {
            result_row = mysql_fetch_row(res_ptr);

            LOGFMTT("retstr: %s",result_row[0]);

            strcpy(retstr,result_row[0]);
            mysql_free_result(res_ptr);
        }
        else
        {
            LOGFMTE("mysql_store_result error: %s",mysql_error(&connection));             
        }
    }
    return res;
}

int querycamcmdcamid(MYSQL connection,char *camid)
{
    char sqlcmd[128];

    sprintf(sqlcmd,"SELECT CamId FROM camcmd");                
        
    LOGFMTT("Sqlcmd %s",sqlcmd);

    int ret = querystring(connection,sqlcmd,camid);
    if(ret)
    {
        LOGFMTE("querycamcmdcamid error");
    }
  
    LOGFMTT("camcmd camid %s",camid);

    return ret;
}


int main()
{
    struct ifreq ifr;
    int sock = -1;
    char smsg[128];
    char camid[64];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {   
        LOGF("socket error");	
        return 1;
    }   
	
    const int opt = 1;
    //设置该套接字为广播类型，
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb == -1)
    {
        LOGF("set socket error...");
        return 1;
    }

    // 广播地址
    struct sockaddr_in from;
    bzero(&from, sizeof(struct sockaddr_in));
    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(10000);


    struct sockaddr_in addrto;
    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family=AF_INET;
    addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
    addrto.sin_port=htons(10000);
    int nlen=sizeof(addrto);


    MYSQL connection;     
    /*初始化mysql连接*/
    mysql_init(&connection);

    /*建立与数据库的连接*/
    if(mysql_real_connect(&connection, "localhost", "root", "root", "ic8300", 0, NULL, 0))
    {
        while(1)
        {
             
            /*get camcmd camid*/    
            int ret = querycamcmdcamid(connection,camid);
            if(ret)
            {
                LOGE("querycamcmdcamid");
            }
            sprintf(smsg,"QHYCCD:%s",camid);
            sleep(1);
            
            //从广播地址发送消息
            ret = sendto(sock, smsg, strlen(smsg), 0, (sockaddr*)&addrto, nlen);
            if(ret < 0)
            {
                LOGFMTE("send error %d....",ret);
            }
            else
            {			
                LOGFMTE("send ok,%s",smsg);
            }
        }
        mysql_close(&connection);
        mysql_library_end();     
    }
    else
    {
        LOGF("could not connect mysql");
    }
    return 0;
}

