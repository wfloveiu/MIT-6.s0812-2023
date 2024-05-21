#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
// void compare(char *path, char *des)
// {
//     char *p;
//     for(p=path+strlen(path); p >= path && *p != '/'; p--)
//         ;
//     p++;
//     if(strcmp(p,des) == 0)
//         printf("%s\n", path);
//     return;

// }

void find(char *path, char *des)
{
    int fd;
    struct stat st;
    char buf[512], *p;
    struct dirent de;

    if((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        // return;
    }
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        // return;
    }

    switch (st.type)
    {
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                *(p+DIRSIZ) = 0;
                if(stat(buf, &st) < 0)
                {
                    printf("find: connot stat file %s\n", buf);
                    continue;
                }
                // printf("%s\n", buf);
                if(st.type == T_DIR)
                    find(buf,des);
                    
                else if(strcmp(p,des) == 0)
                    printf("%s\n", buf);
            }
            break;
    }
    return;
}

int main(int argc, char *argv[])
{
    find(argv[1], argv[2]);
    exit(0);
}
