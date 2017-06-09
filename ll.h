#include <stdint.h>
#define nbPartMax 512
#define magicnumber 31534654
#define TTTFS_VOLUME_BLOCK_SIZE 1024
typedef struct { // creer un structure block
	char t[1024];
} block;

typedef struct {
	int erro;

} error;
// description d'un bloc
typedef struct {
	int taille; // nombre de bloc dans la partition
	int nbBlocLibre; // bloc libre
	int firstFreeBloc;
	int maxFile;
	int nbFreeFile;
	int firstFile;

} partition;
// fichier dans le disque
typedef struct {
	int sizeFile; // nombre de bloc dans la partition
	int typeFile; // bloc libre
	int subtypeFile;
	// les 10 premiers numero des bloc ou sont situés le contenu des format
	int directFile[10];
	// 
	int indirectFile1[256];
	// contient un tableau de tableau de numero de bloc 
	// ou sont situé le contenu du fichier
	int indirectFile2[256][256];
	int freeFile;

} file;


typedef struct { // creer un structure block
	int fd;
	int nbbloc;
	int taille;	
	int nbPart;
	int taillePart[512];
} disk_id;

int estNombre(char * c);
void int_to_little_endian2(unsigned char* t,int i, int x);
error read_physical_block(disk_id id,block* b,uint32_t num);
error write_physical_block(disk_id id,block b,uint32_t num);
error start_disk(char *name,disk_id *id);
error write_block(disk_id id,block b,uint32_t num);
error read_block(disk_id id,block* b,uint32_t num);
error sync_disk(disk_id id);
error stop_disk(disk_id id);
int estNombre(char * c);
int little_endian_to_int_(uint32_t u);
uint32_t int_to_little_endian(int size);