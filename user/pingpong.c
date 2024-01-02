#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 1) {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    char par_buff[1];
    *par_buff = 'x';
    char son_buff[1];
    *son_buff = 'x';
    int p1[2];          // parent -> son
    int p2[2];          // son -> parent
    pipe(p1);
    pipe(p2);
    if (fork() == 0) {  // son process
        // receive
        close(p1[1]);
        read(p1[0], son_buff, 1);
        close(p1[0]);
        fprintf(1, "%d: received ping\n", getpid());
        // fprintf(1, "%c\n", son_buff[0]);
        // send
        close(p2[0]);
        write(p2[1], son_buff, 1);
        close(p2[1]);
    } else {            // parent process
        // send
        close(p1[0]);
        write(p1[1], "a", 1);
        close(p1[1]);
        // wait until the son exits
        wait(0);
        // receive
        close(p2[1]);
        read(p2[0], par_buff, 1);
        fprintf(1, "%d: received pong\n", getpid());
        // fprintf(1, "%c\n", par_buff[0]);
        close(p2[0]);
    }

    exit(0);
}
