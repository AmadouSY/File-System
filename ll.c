#include "ll.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

error read_physical_block(disk_id id,block* b,uint32_t num){
	error e;
	int f = id.fd;
	if(f!=-1){
		if(num<=id.nbbloc){
			lseek(f,num*1024,SEEK_SET);
			int a = read(f,(b->t),1024);
			return e; 
		}
		else{
			e.erro=-1;
			fprintf(stderr, "%d superieur au nombre de block\n",num);
			return e;
		}
	}
	else {
		e.erro=-1;
		fprintf(stderr,"erreur de lecture du fichier1\n");
		return e;
	}
}

error write_physical_block(disk_id id,block b,uint32_t num) {
	error e;
	int f = id.fd;
	if(f!=-1){
		if(num<=id.nbbloc){
			lseek(f,num*1024,SEEK_SET);
			write(f,b.t,1024);
			return e; 
		}
		else{
			e.erro=-1;
			fprintf(stderr, "%d superieur au nombre de block\n",num);
			return e;
		}
	}
	else {
		e.erro=-1;
			fprintf(stderr,"erreur d'ecriture du fichier\n");
			return e;
	}
}

error read_block(disk_id id,block *b,uint32_t num){
	return read_physical_block(id,b,num);
}
error write_block(disk_id id,block b,uint32_t num){
	return write_physical_block(id,b,num);
}

int estNombre(char * c){
 	int i = 0;
 	do{
 		if(isdigit(c[i]) == 0){
 			return 1;
 		}
 		i++;
 	} while (c[i]!='\0');
 	return 0;
 } 
void int_to_little_endian2(unsigned char* t,int i, int x){
 	t[i] = x%256;
 	t[i+1] = (x/256)%256;
 	t[i+2] =((x/256)/256)%256;
 	t[i+3] = (((x/256)/256)/256)%256;
} 
uint32_t int_to_little_endian(int size){
	uint32_t u;
	unsigned char* t = (unsigned char*)(&u);
 		t[0] = size%256;
 		t[1] = (size/256)%256;
 		t[2] =((size/256)/256)%256;
 		t[3] = (((size/256)/256)/256)%256;
 		return u;
}

int little_endian_to_int_(uint32_t u){
	unsigned char* ta = (unsigned char*)(&u);
	int tai = ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
	return tai;
}

error start_disk(char *name,disk_id *id){
 	error e;
 	int cr = open(name,O_RDWR | O_EXCL, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH );
 	id->fd=cr;
 	block fi;
 	uint32_t rec = int_to_little_endian(0);
 	e = read_block(*id,&fi,rec);
 	uint32_t n;
 	unsigned char* ta = (unsigned char*)(&n);
 	int i;
 	for(i=0;i<4;i++){
 		ta[i]=fi.t[i];
 	}
 	int tai = ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
 	id->nbbloc=tai;
 	if(e.erro==-1){
 		return e;
 	}
 	rec = int_to_little_endian(0);
 	//e = read_block(*id,&fi,rec);
 	ta = (unsigned char*)(&n);
 	for(i=4;i<8;i++){
 		ta[i-4]=fi.t[i];
 	}
 	int nbp	= ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
 	if(e.erro==-1){
 		return e;
 	}
 	id->nbPart = nbp+1;
 	int tp;
 	int compt = 8;
 	int ind=0;
 	int j;
 	int taillefin = tai;
 	for(i=2;i<(2+nbp);i++){
 		compt=i*4;
 		if(compt>=1024){
 			ind = ind +1;
 			e = read_block(*id,&fi,ind);
 			if(e.erro==-1){
	 			return e;
	 		}
 		}
 		for(j=compt;j<(compt+4);j++){
 			ta[j-compt]=fi.t[j];
 		}
 		tp	= ta[0] + 256 *ta[1] + (256*256) * ta[2] +(256*256*256) * ta[3];
 		taillefin = taillefin - tp;
 		id->taillePart[i-2] = tp;
 	}	
 	id->taillePart[nbp] = taillefin;
 	e.erro=0;
 	return e;
 }