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
 	if(argc>=3){
 		disk_id id;
 		int i=1;
 		int n=argc;
 		error e;
 		if(estNombre(argv[argc-1]) == 1){
 			c=argv[argc-1];
 			n=argc -1;
 		} 
 		e=start_disk(c,&id);
 		if(e.erro==-1){
 			return -1;
 		}
 		int count;
 		int somme =0;
 		int tp[nbPartMax];
  		for(i=1;i<n;i+=2){
 			if(strcmp(argv[i],"-p")==0){
 				count = count + 1;
 				if(estNombre(argv[i+1]) == 0){
 					tp[count-1]=atoi(argv[i+1]);
 					somme += tp[count-1];
 					if (somme>id.nbbloc)
 					{
 						fprintf(stderr, "%s\n", "Erreur: taille du disque insuffisante.");
 						return -1;
 					}
 				}
 				else{
	 				fprintf(stderr, "%s\n", "Erreur: mauvais argument(s).");
	 				return -1;
 				}
 			}
 		}
 		int nbloc = id.nbbloc;
 		block tab[nbloc];
 		int j;
 		for(i=0;i<nbloc;i++){
 			for(j=0;j<1024;j++){
				tab[i].t[j] = '\0';
			}	
 		}
 		unsigned char* tb = (unsigned char*)(&(tab[0].t));
 		tb[0] = nbloc%256;
 		tb[1] = (nbloc/256)%256;
 		tb[2] =((nbloc/256)/256)%256;
 		tb[3] = (((nbloc/256)/256)/256)%256;
 		tb[4] = count%256;
 		tb[5] = (count/256)%256;
 		tb[6] =((count/256)/256)%256;
 		tb[7] = (((count/256)/256)/256)%256;
 		int k=8;
 		int l = 0;
 		int compt =0;
 		for(k;k<1024;k+=4){
 			tb[k] = tp[compt]%256;
 			tb[k+1] = (tp[compt]/256)%256;
 			tb[k+2] = ((tp[compt]/256)/256)%256;
 			tb[k+3] = (((tp[compt]/256)/256)/256)%256;
 			compt++;
 			if((k+4)>1024){
 				printf("%d\n", k+4);
 				printf("%d\n", nbloc);
 				l=l+1;
 				if(l<nbloc){
 					unsigned char* tb = (unsigned char*)(&(tab[l].t));
 					k=0;
 				}
 				else{
 					fprintf(stderr, "%s\n", "Trop de partition");
 					return -1;
 				}	
 			}
 			/*if(compt=count){
 				printf("%d\n", k);
 				break;
 			}*/
 		}
 		for(i=0;i<nbloc;i++){
 			write_block(id,tab[i],i);
 		}
 	}
 	else{
 		fprintf(stderr, "%s\n", "Pas assez d'argument");
 	}
 }