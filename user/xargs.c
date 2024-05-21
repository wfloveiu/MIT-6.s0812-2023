#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define PER_INPUT_MAX_LENGTH 100

char* ugets(char *buf, int max);
int ugetcmd(char *buf, int nbuf);

int main(int argc, char *argv[])
{
    char *argv_para[MAXARG];
    char stard_input[MAXARG][PER_INPUT_MAX_LENGTH];
    int len = 0; //length of input;
    if(argc == 1)
    {
        fprintf(2, "Usage: xargs COMMAND ...\n");
        exit(1);
    }
    if(argc-1 > MAXARG)
    {
        fprintf(2, "COMMAND length too long ...\n");
        exit(1);
    }

    for(int i=1; i<argc; i++)
        argv_para[i-1] = argv[i];

    // handle the standardinput
    char buf[PER_INPUT_MAX_LENGTH];
    while((ugetcmd(buf, sizeof(buf))) == 0)
    {
        // printf("buffer:%s\n",buf);
        strcpy(stard_input[len++], buf);
    }
    
    for(int i=0; i<len; i++)
    {
        if(fork() == 0)//child
        {
            argv_para[argc-1] = stard_input[i];
            exec(argv_para[0],argv_para);
            printf("exec error\n");//return only error
            exit(1);
        }
        else
            wait(0);
    }
    exit(0);
}

char* ugets(char *buf, int max)
{
    int i, cc;
    char c;

    for(i=0; i+1 < max; )
    {
        cc = read(0, &c, 1);
        if(cc < 1)// reach the end
            break;
        if(c == '\n' || c == '\r')
            break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}
int ugetcmd(char *buf, int nbuf) //in the sh.c, get the command of user's input
{
  memset(buf, 0, nbuf);
  ugets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}


