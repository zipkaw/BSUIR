#ifndef __EXT_SOURCE__
#define __EXT_SOURCE__
#define _LARGEFILE64_SOURCE
#include <ext2fs/ext2fs.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

//offset to superblock
#define BASE_OFFSET 1024

const int pos[] = {1, 3, 5, 7, 9, 25, 27, 49, 81};
//calculating group block
#define GROUP_INDEX (inode_num, inodes_per_group) (return ((inode_num - 1)/inodes_per_group))

//cdlculating inode index 
#define INODE_INDEX (inode_num, inodes_per_group) (return ((inode_num - 1)%inodes_per_group))

//#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * block_size)

struct inode_d_time
{
    unsigned int inode;
    unsigned int d_time; 
};

int check_super_block(int super_block_num, int block_size) {
    for(int i = 0; i < 9; i++){
        if((pos[i]*block_size) == super_block_num){
            return 0;
        }
    }
    return 0;
}

int open_device(char* name)
{
    int fd = 0;
    if ((fd = open(name, O_RDONLY)) < 0)
    {
        perror("cant open device file!");
        exit(-1);
    }
    return fd; 
}

void close_device(int fd)
{
    if (close(fd) < 0)
    {
        perror("close");
        exit(-1);
    }
}

unsigned int get_block_size(char * name)
{
    int fd = open_device(name);
    int block_size = 0;
    struct ext2_super_block *super = malloc(sizeof(struct ext2_super_block));
    if (lseek(fd, BASE_OFFSET, SEEK_SET) < 0)
    {
        perror("seek");
        exit(-1);
    }
    if (read(fd, super, sizeof(*super)) < 0)
    {
        perror("read");
        exit(-1);
    }
    if (super->s_magic != EXT2_SUPER_MAGIC)
    {
        fprintf(stderr, "Not a ExtX filesystem\n");
        exit(1);
    }
    block_size = 1024 << super->s_log_block_size;
    close_device(fd);
    free(super);
    return block_size;
}


int extXdetector(struct ext2_super_block super){
    if (EXT2_SUPER_MAGIC != super.s_magic){
        fprintf(stderr, "This not extfs, fs signature is %x", super.s_magic);
        exit(-1);
    }
    int flag = 0; 
    if((ext2fs_has_feature_xattr(&super) != 0) && (ext2fs_has_feature_sparse_super(&super)) != 0){
        flag = 2; 
    } 
    if((ext2fs_has_feature_journal(&super) != 0) && (ext2fs_has_feature_dir_index(&super) != 0)){
        flag = 3; 
    }
    if((ext2fs_has_feature_64bit(&super) != 0) && (ext2fs_has_feature_extents(&super) != 0) &&
        (ext2fs_has_feature_extra_isize(&super) != 0) && (ext2fs_has_feature_huge_file(&super) != 0)){
        flag = 4;
    }
    return flag;
}

#endif //__EXT_SOURCE__