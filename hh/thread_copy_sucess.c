#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<pthread.h>
int hyq1=0;
int blocksize;
char *mmap_src=NULL;
char *mmap_dest=NULL;

void * thread_copy(void * arg)
{
	int i=*(int*)arg;
	int POS=i*blocksize;
	printf("copy thread 0x%x,i=%d,pos=%d,blocksize=%d\n",(unsigned int)pthread_self(),i,POS,blocksize);
	memcpy(mmap_dest+POS,mmap_src+POS,blocksize);


	pthread_exit(NULL);
}

void * thread_copy2(void * arg)
{
	int i=*(int*)arg;
	memcpy(mmap_dest+5*blocksize,mmap_src+5*blocksize,i);
//	printf("hyq1=%d\n",i);
	pthread_exit(NULL);
}
	

int main(int argc,char* argv[])
{
if(argc!=3){
printf("please input correct parametersn");
return -1; 
}   
int fd_src=open(argv[1],O_RDONLY);
int fd_dest=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0664);

struct stat buf;
fstat(fd_src,&buf);
int src_size=buf.st_size;
truncate(argv[2],src_size);

mmap_src=mmap(NULL,src_size,PROT_READ,MAP_PRIVATE,fd_src,0);
mmap_dest=mmap(NULL,src_size,PROT_WRITE,MAP_SHARED,fd_dest,0);
if(mmap_src==MAP_FAILED||mmap_dest==MAP_FAILED){
perror("mmap err");
return -1;
}
close(fd_src);
close(fd_dest);

int n=5;
if(src_size%5==0)
	blocksize=src_size/n;
else
{
	blocksize=src_size/n;
	hyq1=src_size%n;
}	
int rear_paragraph_size=src_size%(n-1);

int i;
pthread_t tids[n];
for(i=0;i<n;i++)
{
 pthread_create(&tids[i],NULL,thread_copy,(void *)&i);
for (int xx = 1; xx <= 99; xx++)
{
	double iio=xx*(i+1)/n;
	printf("[%0.2f %%]\r",iio);
	usleep(2000);//1/10s
}
printf("\n");
//usleep(10000);
}
while(i--)
{
	pthread_join(tids[i],NULL);
}
pthread_t tidss;
pthread_create(&tidss,NULL,thread_copy2,(void*)&hyq1);
printf("[100%%]\n");
usleep(10000);
pthread_join(tidss,NULL);

return 0;
}































