#include "kernel/types.h"
#include "user/user.h"

void run_pro(int read_fd)
{
    int pipes[2];
    int out_num = 0;
    int read_num ;
    int forked = 0;
    while(1)
    {
        int ret = read(read_fd, &read_num, 4);
        // printf("%d\n", read_num);
        if(ret == 0) //read over
        {
            close(read_fd);
            if(forked)
            {
                close(pipes[1]);
                int status;
                wait(&status);
            }
            exit(0); //return back
        }
        
        if(out_num == 0)
        {
            out_num = read_num;
            printf("prime %d\n", out_num);
            continue;
        }

        if(read_num % out_num != 0)
        {
            if(!forked)
            {
                pipe(pipes);
                forked = 1;
                int ret = fork();
                if(ret == 0)
                {   //child
                    close(read_fd);//the child do not need parent's read_fd
                    close(pipes[1]);//the child do not need write index
                    run_pro(pipes[0]);
                }
                else
                {   //parent
                    close(pipes[0]);//the parent do not need read index
                }
            }
            write(pipes[1], &read_num, 4);
            
        }
    }
}
int main(int argc, char *argv[])
{
    int pipes[2];
    pipe(pipes);
    for(int i = 2; i<= 35; i++)
    {
        write(pipes[1], &i, 4);
    }
    close(pipes[1]);
    run_pro(pipes[0]);
    exit(0);
}