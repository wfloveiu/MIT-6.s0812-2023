#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pipe1[2], pipe2[2];//index 1 is the write place and index 0 is the read place
    char buf[] = {"a"};
    pipe(pipe1);
    pipe(pipe2);

    int ret = fork();

    if(ret != 0)
    {
        //parent
        int pid = getpid();
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], buf, 1);//send 1 byte
        read(pipe2[0], buf, 1);
        printf("%d: received pong\n", pid);
        exit(0);
    }
    else
    {
        //child
        int pid = getpid();
        close(pipe1[1]);
        close(pipe2[0]);
        read(pipe1[0], buf, 1);
        printf("%d: received ping\n", pid);
        write(pipe2[1], buf, 1);
        exit(0);
    }
}