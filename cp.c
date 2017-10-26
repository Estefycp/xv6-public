#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf(2, "Usage: cp old new\n");
        exit();
    }

    char buf[512];
    int fd0, fd1;
    fd0 = open(argv[1], 0);
    if((fd1 = open(argv[2], O_WRONLY)) < 0){ // Si argv[2] no existe, se crea.
        fd1 = open(argv[2], O_CREATE | O_WRONLY);
    }

    int n;
    while((n = read(fd0, buf, sizeof(buf))) > 0) {
        write(fd1, buf, n);
    }

    close(fd0);
    close(fd1);

    exit();
}
