#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{
	const char *name = "COLLATZ";
	const int SIZE = 4096;

	pid_t pid;
	int shm_fd;
	void *ptr;
	int n;
	char str[128];

	if (argc == 1) {
		fprintf(stderr,"Usage: ./a.out <starting value>\n");
		return -1;
	}

	n = atoi(argv[1]); /* 從命令列取得要計算的考拉茲臆測數列第一個數字 */
	
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);/* 建立一個共用記憶體物件 */ 
	ftruncate(shm_fd,SIZE); /* 設定共用記憶體物件的大小 */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	 /* 共用記憶體物件映射至行程位址空間 */
	const int* message1 ;
	
	int num[128],i=0;
	num[i++]=n;
	int k;
	while(1){
		
		if(n==1)
			break;
		else if(n%2==0){
			n/=2;
			num[i++]=n;
		}
		else{
			n*=3;
			n+=1;
			num[i++]=n;
		}
	}
	k=i;
	message1 = num;

	
	for (i =0; i < k; i++)
	{
		sprintf(ptr,"%d",*(message1+i));
		ptr += sizeof(*(message1));
	}
	
	

	return 0;
	
}
