#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int CHECK_ARG(int argno,int prono,const char *Sfile);
int COPY_BLOCK_CUR(const char * Sfile,int prono);
int PROCESS_CREATE(const char * Sfile,const char * Dfile,int prono,int blocksize);
int PROCESS_WAIT(void);











