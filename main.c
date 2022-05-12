
#include "interface.h"

struct struct_ext2_filsys* filesys; 
unsigned int block_size = 0; 
int main(int argc, char *argv[]){
    
    if(argc != 2){
        fprintf(stdout, "to few arguments\n");
        exit(-1);
    }
    filesys = malloc(sizeof(struct struct_ext2_filsys));
    get_block_size(argv[1]);
    ext2fs_open(argv[1], EXT2_FLAG_RW, 0, , unix_io_manager, &filesys);
    
}
