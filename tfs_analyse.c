#include "ll.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[])
 {
 	char *c = "disk.tfs";
 	if(argc==2){
 		c= argv[1];
 	}
	if(argc>2){
		fprintf(stderr, "%s\n", "Erreur: trop d'argument(s).");
		return -1;
	}	
 	disk_id id;
 	error e;
 	e=start_disk(c,&id);
 	if(e.erro==-1){
 		return -1;
 	}
 	printf("Nombre de bloc = %d, Nombre de partition = %d\n",id.nbbloc,id.nbPart);
 	int i;
 	for (i = 0; i < (id.nbPart); i++)
 	{
 		printf("Taille partition %d = %d\n",(i+1),id.taillePart[i]);
  	}
  	return 0;
} 		 