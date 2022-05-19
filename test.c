#include "ext_source.h"
#include "interface.h"
#include "ext3_recovery.h"
#define DEVICE_PATH "/dev/sda1"
// struct struct_ext2_filsys* filesys;
// unsigned int block_size = 0;
int main(int argc, char *argv[])
{
    // if(argc != 2){
    //     fprintf(stdout, "to few arguments\n");
    //     exit(-1);
    // }
    // unsigned int block_size = 0;
    // /*problem in read function:
    // **-fixed*/
    // block_size = get_block_size(DEVICE_PATH);
    // struct struct_ext2_filsys *file_system = malloc(sizeof(struct struct_ext2_filsys));
    // ext2fs_open(DEVICE_PATH, EXT2_FLAG_RW, 0, block_size, unix_io_manager, &file_system);
    // /*output super block info*/
    // int fd = open_device(DEVICE_PATH);
    // read_sb(*file_system);
    // read_journal_sb(file_system, fd);
    // struct ext2_inode inode;
    // ext2fs_read_inode(file_system, EXT2_JOURNAL_INO, &inode);
    // int j = 0;
    // while (getchar() != 27 && j < 11)
    // {
    //     int pos = (inode.i_block[j] * 4096);
    //     __u32 buffer[1024];
    //     pread(fd, buffer, 4096, pos);
    //     int db = 0;
    //     for (int i = 0; i < 1024; i++)
    //     {
    //         printf("%x  ", convert_from_num(buffer[i]));
    //         if (!(i % 32))
    //             puts("\n");
    //     }
    //     j++;
    //     puts("\n");
    //     print_dash(25);
    //     puts("\n");

    // }
    // close_device(fd);
    // ext2fs_close(file_system);
    return 0;
}
