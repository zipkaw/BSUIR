#include "ext_source.h"
#include "interface.h"

#define DEVICE_PATH "/dev/sda1"
//struct struct_ext2_filsys* filesys; 
//unsigned int block_size = 0; 
int main(int argc, char* argv[]){
    
    // if(argc != 2){
    //     fprintf(stdout, "to few arguments\n");
    //     exit(-1);
    // }
    unsigned int block_size = 0; 
    //problem in read function
    block_size = get_block_size(DEVICE_PATH);
    struct struct_ext2_filsys* file_system = malloc(sizeof(struct struct_ext2_filsys));
    ext2fs_open(DEVICE_PATH, EXT2_FLAG_RW, 0, block_size, unix_io_manager, &file_system);
    read_sb(*file_system);
    //ext2fs_close(file_system);
    //free(file_system);
    return 0;
}