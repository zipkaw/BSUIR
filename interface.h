#ifndef __INTERFACE__
#define __INTERFACE__

#include "ext_source.h"
#include "ext2_recovery.h"

void print_dash(int count){
    for(int i = 0; i< count; i++){
        printf("-");
    }
}

void read_sb(struct struct_ext2_filsys file_sys)
{

    if (file_sys.super->s_magic != EXT2_SUPER_MAGIC)
    {
        fprintf(stderr, "Not a ExtX filesystem\n");
        exit(1);
    }
    // block_size = 1024 << super->s_log_block_size;
    struct ext2_super_block *sb = file_sys.super;

    if (extXdetector != 0){
        print_dash(5);
        printf("Filesystem is: EXT%d", extXdetector(*(file_sys.super)));
        print_dash(5); 
        puts("\n");
    } else {
        fprintf(stderr, "Can not a define ExtX filesystem, exit....\n");
        exit(1);

    }
    print_dash(5);
    printf("Reading super-block from device:");
    print_dash(5);
    puts("\n");
    printf("Inodes count            : %u\n"
           "Blocks count            : %u\n"
           "Reserved blocks count   : %u\n"
           "Free blocks count       : %u\n"
           "Free inodes count       : %u\n"
           "First data block        : %u\n"
           "Block size              : %u\n"
           "Blocks per group        : %u\n"
           "Inodes per group        : %u\n"
           "Creator OS              : %u\n"
           "First non-reserved inode: %u\n"
           "Size of inode structure : %hu\n",
           sb->s_inodes_count,
           sb->s_blocks_count,
           sb->s_r_blocks_count, /* reserved blocks count */
           sb->s_free_blocks_count,
           sb->s_free_inodes_count,
           sb->s_first_data_block,
           file_sys.blocksize,
           sb->s_blocks_per_group,
           sb->s_inodes_per_group,
           sb->s_creator_os, //== 0 ? "LINUX" : "Other OS",
           sb->s_first_ino, /* first non-reserved inode */
           sb->s_inode_size);
    print_dash(20);  
    puts("\n");
};

void print_deleted(struct struct_ext2_filsys filsys){ 
    int size = 0;
    unsigned int * read_inos = all_deleted_inodes(filsys, &size); 
    struct ext2_inode inode; 
    if(size > 0){
        for(int i = 0; i < size; i++){
            ext2fs_read_inode(&filsys, read_inos[i], &inode);
            printf("%u. -inode- %u               -deleted time- %u\n", 
                    i,
                    read_inos[i], 
                    inode.i_dtime);
        }
    }else{
        puts("doesn't found deleted files");
    }
}

#endif //__INTERFACE__