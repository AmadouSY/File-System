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
 	if(argc==3){
 		c= argv[2];
 	}
	if(argc>3 || argc<2){
		fprintf(stderr, "%s\n", "Erreur: nombre d'argument(s).");
		return -1;
	}
	disk_id id;
	partition pa;
 	error e;
 	e=start_disk(c,&id);
 	if(e.erro==-1){
 		return -1;
 	}
 	int p = atoi(argv[1])-1;
 	int i;
 	int nblocdescr = 1;
 	for(i=0;i<id.nbPart;i++){
 		if(p==i){
 			break;
 		}
 		nblocdescr = nblocdescr + id.taillePart[i];
 	}

 	// afficher le file tab
 	block b;
 	e = read_block(id,&b,nblocdescr+1);
 	uint32_t n;
 	unsigned char* ta = (unsigned char*)(&n);
 	for(i=0;i<4;i++){
 		ta[i]=b.t[i];
 	}
 	int fileSize = ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];

 	for(i=4;i<8;i++){
 		ta[i-4]=b.t[i];
 	}
 	int fileType = ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
 	for(i=8;i<12;i++){
 		ta[i-8]=b.t[i];
 	}
 	int filePseudo = ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
 	for(i=60;i<64;i++){
 		ta[i-60]=b.t[i];
 	}
 	int prochainFichierLibre = ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
 	
 	printf("fileSize = %d\n" ,fileSize);
 	printf("fileType = %d\n" ,fileType);
	printf("filePseudo =%d\n",filePseudo);
	printf("prochain fichier libre = %d\n",prochainFichierLibre);	
 }