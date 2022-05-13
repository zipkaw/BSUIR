#ifndef __EXT2_RECOVERY__
#define __EXT2_RECOVERY__

#include "ext_source.h"

/*check all inode*/

unsigned int *all_deleted_inodes(struct struct_ext2_filsys filsys, unsigned int* size)
{
    unsigned const int ext_first_non_reserv_ino = 12;
    unsigned int size_i = 0;
    unsigned int* ret_inos =(unsigned int*) malloc(sizeof(unsigned int));
    unsigned int* temp_ptr; 
    struct ext2_inode inode;
    for (int i = ext_first_non_reserv_ino, j = 0; i < filsys.super->s_inodes_count; i++, j++)
    {
        ext2fs_read_inode(&filsys, i, &inode);
        if (inode.i_dtime != 0)
        {
            if ((inode.i_atime == inode.i_ctime) && (inode.i_atime == inode.i_dtime)){
                size_i++; 
                temp_ptr = ret_inos; 
                if(!(temp_ptr =(unsigned int*)realloc(ret_inos, (size_i+1)* sizeof(unsigned int)))){
                    perror("bad realloc");
                    /*return to main cycle*/
                } else  {
                    ret_inos = temp_ptr; 
                    ret_inos[j] = i;
                }
            }
        }
    }
    *size = size_i;
    return ret_inos; 
}


#endif //__EXT2_RECOVERY__
