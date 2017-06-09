#include "ll.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
 {
 	int size=0;
 	char *c = "disk.tfs";
 	if(argc==3 || argc ==4){
 		size=atoi(argv[2]);
 		if(strcmp(argv[1],"-s")!=0 || size<=0){
 			fprintf(stderr, "%s\n", "Erreur: mauvais argument(s).");
 		} 
 		if(argc == 4) {
 			c = argv[3];
 		}
 	}
 	if(size>0){
 		block b;
 		block b2;
 		int i;
 		int j;
 		for(j=0;j<1024;j++){
			b.t[j] = '\0';
			b2.t[j] = '\0';
		}	
 		
 		disk_id id;
 		unsigned char* t = (unsigned char*)(&(b.t));
 		t[0] = size%256;
 		t[1] = (size/256)%256;
 		t[2] =((size/256)/256)%256;
 		t[3] = (((size/256)/256)/256)%256;
 		//printf("%d-%d-%d-%d\n",t[0],t[1],t[2],t[3]);
 		int cr = open(c,O_CREAT | O_WRONLY | O_EXCL, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH );
 		id.fd=cr;
 		id.nbbloc=size;
 		for(i=0;i<size;i++){
 			if(i==0){
 				write_block(id,b,i);
 			}
			else{
				write_block(id,b2,i);
			}	
  		}
 		close(id.fd);
 	}
 	return 0;
 } 