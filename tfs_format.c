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
 	if(argc==6){
 		c= argv[5];
 	}
	if(argc>6 || argc<5){
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
 	int p = atoi(argv[2])-1;
 	int i;
 	int nblocdescr = 1;
 	for(i=0;i<id.nbPart;i++){
 		if(p==i){
 			pa.taille = id.taillePart[i];
 			pa.nbBlocLibre = id.taillePart[i] - 3;
 			pa.firstFreeBloc = 3;
 			pa.maxFile = atoi(argv[4]);
			pa.nbFreeFile = atoi(argv[4])-1;
			pa.firstFile = 2;
 			break;
 		}
 		nblocdescr = nblocdescr + id.taillePart[i];
 	}
 	// pour le bloc b0 : la description de la partition
 	block b;
 	unsigned char* t = (unsigned char*)(&(b.t));
 	int_to_little_endian2(t,0,magicnumber);
 	int_to_little_endian2(t,4,TTTFS_VOLUME_BLOCK_SIZE);
 	int_to_little_endian2(t,8,pa.taille);
 	int_to_little_endian2(t,12,pa.nbBlocLibre);
 	int_to_little_endian2(t,16,pa.firstFreeBloc);
 	int_to_little_endian2(t,20,pa.maxFile);
 	int_to_little_endian2(t,24,pa.nbFreeFile);
 	int_to_little_endian2(t,28,pa.firstFile);
 	e = write_block(id,b,nblocdescr);
 	if(e.erro==-1){
 		return -1;
 	}
 	// la table de fichier
 	block rac;
 	t = (unsigned char*)(&(rac.t));
 	int_to_little_endian2(t,0,64);
 	int_to_little_endian2(t,4,1);
 	int_to_little_endian2(t,8,0);
 	int_to_little_endian2(t,12,2);
 	int_to_little_endian2(t,60,1);
 	e = write_block(id,rac,nblocdescr+1);
 	if(e.erro==-1){
 		return -1;
 	}

 	block cont;
 	t = (unsigned char*)(&(cont.t));
 	int_to_little_endian2(t,0,0);
 	t[4]=46;
 	t[5]='\0';
 	int_to_little_endian2(t,32,0);
 	t[36]=46;
 	t[37]=46;
 	t[38]='\0';
 	e = write_block(id,cont,nblocdescr+2);
 	block fr;
 	for(i=3;i<pa.taille;i++){
 		t = (unsigned char*)(&(fr.t));
 		int_to_little_endian2(t,1020,i+1);
 	}
 	if(e.erro==-1){
 		return -1;
 	}

}	