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
    /*problem in read function: 
    **-fixed*/
    block_size = get_block_size(DEVICE_PATH);
    struct struct_ext2_filsys* file_system = malloc(sizeof(struct struct_ext2_filsys));
    ext2fs_open(DEVICE_PATH, EXT2_FLAG_RW, 0, block_size, unix_io_manager, &file_system);
    /*output super block info*/
    read_sb(*file_system);
    /*dgrp_t i_gd_t = 0;
    struct ext2_group_desc gd = *ext2fs_group_desc(file_system, file_system->group_desc, i_gd_t);
    __u64 pos;
    pos = (inode->i_block[0]) * 4096;
    pread64(3, buff, 4096, pos);
    int rec_len = 0;
    struct ext2_dir_entry_2 dent;
    while (rec_len < 4096)
    {
        memcpy((void *)&dent, (buff + rec_len), sizeof(dent));
        printf("%d----dir---%s\n", dent.inode, dent.name);
        rec_len += dent.rec_len;
    }

    ext2fs_read_inode(file_system, 163201, inode);
    pos = (inode->i_block[0]) * 4096;
    pread64(3, buff, 4096, pos);
    rec_len = 0; 
     while (rec_len < 4096)
    {
        memcpy((void *)&dent, (buff + rec_len), sizeof(dent));
        printf("%d----dir---%s\n", dent.inode, dent.name);
        rec_len += dent.rec_len;
    }
    
    unlink("/media/plantator/EXT2/dir/test.txt");
    ext2fs_read_inode(file_system, 163204, inode);

    printf("Reading super-block from device :\n"
           "dtime          : %u\n"
           "i_size            : %u\n"
           "blocks count   : %u\n",
           inode->i_links_count,
           inode->i_size,
           inode->i_blocks);
    return 0;
}*/
/*
    /*end*/
    ext2fs_close(file_system);
    return 0;
}
