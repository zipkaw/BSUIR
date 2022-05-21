#include "ext_source.h"
#include "interface.h"
#include "ext3_recovery.h"

// struct struct_ext2_filsys* filesys;
// unsigned int block_size = 0;
int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stdout, "to few arguments\n");
        exit(-1);
    }
    unsigned int block_size = 0;
    block_size = get_block_size(DEVICE_PATH);
    struct struct_ext2_filsys *file_system = malloc(sizeof(struct struct_ext2_filsys));
    ext2fs_open(DEVICE_PATH, EXT2_FLAG_RW, 0, block_size, unix_io_manager, &file_system);
    /*output super block info*/
    read_sb(*file_system);
    /*output journal superblock info*/
    int fd = open_device(DEVICE_PATH);
    read_journal_sb(file_system, fd);
    close_device(fd);
    /*end*/
    /*sizeof deleted inos struct*/
    int size_d;

    print_dash(25); 
    puts("\n");
    fprintf(stdout, "RECOVERING INFO....");
    print_dash(25); 
    puts("\n");

    /*recovering files*/ 
    inode_d_time_t *deleted_inos = all_deleted_inodes(*file_system, &size_d);
    for (int i = 0; i < size_d; i++)
    {   
        int inodes_per_block = (file_system->blocksize / file_system->super->s_inode_size);
        __u32 inodes_blocks_per_group = file_system->super->s_inodes_per_group / inodes_per_block; 
        __u32 group = (deleted_inos[i].inode - 1) / file_system->super->s_inodes_per_group;
        __u32 index = (deleted_inos[i].inode - 1) % inodes_blocks_per_group;
        __u32 index_in_block = deleted_inos[i].inode - (index*inodes_blocks_per_group);
        struct ext2_group_desc group_desc = read_gd(*file_system, fd, group);
        /*calc inode table block num*/ 
        __u32 offset = group_desc.bg_inode_table; 
        /*recovering info using journal*/
        read_journal(file_system, fd, offset, index_in_block); 
    }
    ext2fs_close(file_system);
    return 0;
}
