#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    while(1)
    {
        system("ntpdate cn.pool.ntp.org");
        sleep(60);
    }
    return 0;
}
