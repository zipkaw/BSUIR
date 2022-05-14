#ifndef __EXT2_RECOVERY__
#define __EXT2_RECOVERY__

#include "ext_source.h"

/*check all inode*/
/*file write realization needed*/
struct inode_d_time *all_deleted_inodes(struct struct_ext2_filsys filsys, unsigned int *size)
{
    unsigned const int ext_first_non_reserv_ino = 12;
    unsigned int size_i = 0;
    struct inode_d_time *ret_inos;
    ret_inos = (struct inode_d_time *)malloc(sizeof(struct inode_d_time));
    struct inode_d_time *temp_ptr;
    struct ext2_inode inode;
    for (int i = ext_first_non_reserv_ino, j = 0; i < filsys.super->s_inodes_count; i++, j++)
    {
        ext2fs_read_inode(&filsys, i, &inode);
        if (inode.i_dtime != 0)
        {
            if ((inode.i_atime == inode.i_ctime) && (inode.i_atime == inode.i_dtime))
            {
                size_i++;
                temp_ptr = ret_inos;
                if (!(temp_ptr = (struct inode_d_time *)realloc(ret_inos, (size_i + 1) * sizeof(struct inode_d_time *))))
                {
                    perror("bad realloc");
                    /*return to main cycle*/
                }
                else
                {
                    ret_inos = temp_ptr;
                    ret_inos[j].inode = i;
                    ret_inos[j].d_time = inode.i_dtime;
                }
            }
        }
    }
    *size = size_i;
    return ret_inos;
}


int ret_d_inode_by_num(struct inode_d_time *d_inos, unsigned int inode, int i_size)
{
    int ret = 0;
    for (int i = 0; i < i_size; i++)
    {
        if (d_inos[i].inode == inode)
            return i;
    }
    return 0;
}
void recovery_file(struct struct_ext2_filsys filsys, unsigned int inode_n, struct inode_d_time *d_inos, int i_size)
{
    struct ext2_inode inode;
    ext2fs_read_inode(&filsys, inode_n, &inode);

    char file_name[100];

    int fd = 0; 
    
    unsigned int file_size; 
    void* buff[filsys.blocksize];
    if((fd = creat(file_name, O_WRONLY | O_CREAT)) > 0){
        perror ("Can't create recovery file, exit...");
        exit(-1);
    }
    /*to fix: 
    **correct buff size*/

    for(int i = 0; i< inode.i_blocks; i++){
        memset(buff, 0, filsys.blocksize);
        memcpy(buff, inode.i_block[i], sizeof(inode.i_block[i]));
        write(fd, buff, sizeof(inode.i_block[i]));
    }

    close(fd);
}

#endif //__EXT2_RECOVERY__
