#ifndef _READING_BLOCKS_
#define _READING_BLOCKS_

#include "ext2_recovery.h"
#include "ext_source.h"
#include "journal.h"
#include "convertLE.h"

typedef unsigned int it; /*for inode type 32 bits num*/
#define IBLOCK_SIZE 1024

/*reading inode blocks in file*/
// int read_ind_blocks();
// int read_dind_blocks();
// int read_tind_blocks();


char *read_inode_iblock(struct ext2_inode inode,
                        int fd,
                        int i_block_num,
                        struct ext2_super_block super,
                        __u32 block_size)
{
    /*
    EXT2_NDIR_BLOCKS 0..11
    EXT2_IND_BLOCK   12
    EXT2_DIND_BLOCK  13
    EXT2_TIND_BLOCK  14
    */
    /*
     offset to info
     */
    /*
     size of adress block
     */
    it count_offset_blks = block_size / 4;

    char *inf_buff = (char *)malloc(sizeof(char) * block_size);
    __u32 *adr_buff = (__u32 *)malloc(sizeof(__u32) * count_offset_blks);
    /*
        0..11
        DIRECT BLOCK
    */
    it first_lvl = EXT2_NDIR_BLOCKS - 1;
    /*
        12..(block_size/4)+11
        INDIRECT BLOCK
    */
    it second_lvl = count_offset_blks + first_lvl;
    /*
        (block_size/4)+12)..((block_size/4)^2) + (block_size/4)+11)
        DOUBLE INDIRECT BLOCK
    */
    it third_lvl = count_offset_blks * count_offset_blks + second_lvl;
    /*
        (((block_size/4)^2) + (block_size/4)+12))..(block_size/4)^3) + (block_size/4)^2 + (block_size/4)+11
        TRIPLE INDIRECT BLOCK
    */
    it fourth_lvl = (count_offset_blks * count_offset_blks * count_offset_blks) + (count_offset_blks * count_offset_blks) + second_lvl;

    if (i_block_num - first_lvl <= 0)
    {
        it pos = inode.i_block[i_block_num] * block_size;
        pread(fd, inf_buff, block_size, pos);
    }
    else if ((i_block_num - second_lvl) <= 0)
    {
        it id = i_block_num - first_lvl - 1;

        while (id - count_offset_blks > 0)
        {
            id -= count_offset_blks;
        }

        it dir_pos = inode.i_block[EXT2_IND_BLOCK] * block_size;
        pread(fd, adr_buff, count_offset_blks, dir_pos);

        it pos = adr_buff[id] * block_size;
        pread(fd, inf_buff, block_size, pos);
    }
    else if (i_block_num - third_lvl <= 0)
    {
        it id = i_block_num - second_lvl - 1;

        it ddir_pos = inode.i_block[EXT2_DIND_BLOCK] * block_size;
        pread(fd, adr_buff, count_offset_blks, ddir_pos);

        for (it i = 0; i < count_offset_blks; i++)
        {
            if (id < count_offset_blks)
            {
                it dir_pos = adr_buff[i] * block_size;
                pread(fd, adr_buff, block_size, dir_pos);

                break;
            }
            else
            {
                id -= count_offset_blks;
                continue;
            }
        }

        it pos = adr_buff[id] * block_size;
        pread(fd, inf_buff, block_size, pos);
    }
    else if (i_block_num - fourth_lvl < 0)
    {
        it id = i_block_num - third_lvl - 1;

        it count_d_offset_blocks = count_offset_blks * count_offset_blks;
        it tdir_pos = inode.i_block[EXT2_TIND_BLOCK] * block_size;
        pread(fd, adr_buff, count_offset_blks, tdir_pos);

        // it d = 0;
        // it i = 0;
        for (it d = 0; d < count_offset_blks; d++)
        {
            if (id < count_d_offset_blocks)
            {
                it ddir_pos = adr_buff[d] * block_size;
                pread(fd, adr_buff, count_offset_blks, ddir_pos);

                for (it i = 0; i < count_offset_blks; i++)
                {
                    if (id < count_offset_blks)
                    {
                        it dir_pos = adr_buff[i] * block_size;
                        pread(fd, adr_buff, block_size, dir_pos);
                        goto read_info;
                        break;
                    }
                    else
                    {
                        id -= count_offset_blks;
                        continue;
                    }
                }
            }
            else
            {
                id -= count_d_offset_blocks;
            }
        }
    read_info:
    {
        it pos = adr_buff[id] * block_size;
        pread(fd, inf_buff, block_size, pos);
    }
    }
    else
    {
        fprintf(stderr, "BLOCK DOES NOT EXIST!");
        exit(-1);
    }
    return inf_buff;
}

int read_inode_blocks(struct ext2_inode inode,
                        int fd,
                        int i_block_num,
                        struct ext2_super_block super,
                        __u32 block_size)
{
    it size = inode.i_size;
    it count_blk = size / block_size;
    it count_offset_blks = block_size / 4;

    it index = 0;
    char inf_buff[block_size];
    int rec_file_fd = creat(inode.i_dtime, O_RDWR);
    char* buff; 
    for (int i = 0; i < count_blk; i++)
    {
        buff = read_inode_iblock(inode, fd, i_block_num, super, block_size); 
        lseek(rec_file_fd, 0, SEEK_END); 
        write(rec_file_fd, buff, block_size);
    }
    fseek(rec_file_fd, 0, SEEK_END);
    if (ftell(fd)){ 
        return 0;
    } else return 1;
}


#endif //_READING_BLOCKS_