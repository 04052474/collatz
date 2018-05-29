#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	const char *name = "COLLATZ";
	const int SIZE = 4096;

	pid_t pid;
	int shm_fd;
	void *ptr;

	if (argc == 1) {
		fprintf(stderr,"Usage: ./a.out <starting value>\n");
		return -1;
	}

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Unable to fork child\n");
		return -1;
	}
	else if (pid == 0) { 

		execlp("./04052474child","./04052474child",argv[1],NULL);
	}
	else { 
		fprintf(stderr,"Child Process ID returned by fork= %d\n", pid);
		wait(NULL);

		shm_fd = shm_open(name, O_RDONLY, 0666);/* 開啟子行程建立的共用記憶體物件 */
		ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);/* 共用記憶體物件映射至行程位址空間 */  
		while(1){
			
			if(*(char*)ptr==0)
				break;
			else{
				printf("%s ",(char *)ptr ); /* 共用記憶體物件內容讀取及輸出*/ 
				ptr+=4;
			}
		}
		printf("\n");
		shm_unlink(name);/* 移除共用記憶體物件*/
	}

	return 0;
}
