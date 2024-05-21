#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(2, "sleep error!\n");
        exit(1); //非正常退出
    }
    int num = atoi(argv[1]);
    int ret = sleep(num); //sleep返回值：0表成功，1表示失败
    exit(ret);
}