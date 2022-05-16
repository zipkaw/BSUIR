#ifndef __EXT3_RECOVERY__
#define __EXT3_RECOVERY__

#include "ext2_recovery.h"
#include "ext_source.h"
#include "journal.h"

/*
**1.находим в какой групп блоков находится 
**наш инод 
**2.вычисляем первый инод в группе блоков
**3.находим блок с которого начинается этот инод
**4.находим блок таблицы инодов 
**5.ищем этот блок в журнале в FS_BLOCK
**6.Сканим его, находим нужный нам инод, извдекаем блоки 
*/
/*calc inode size*/
/*calc seq num inode in table*/

/*----------TASK-----------*/
/*We know that the deleted 
**file allocated inode 488,650, 
**which is located in block group
**30. The first inode in the block 
**group is 488,641, the inode table 
**starts in block 983,044,
**and each block is 4,096 bytes. 
**Therefore, our inode of interest 
**is entry 10 of the group's
**inode table and is in the first 
**block of the table.*/

/*--then we find FSMDBlk with inode 
**  table num block 983044
**--read this block
**--in block skip 
    (target_inode - start_inode_num) * inode_size
    --read inode (128 or 256 b)
    --read i_block
*/

/*open journal(inode 8)*/

/*j_desc
 *{j_header ;sequence, blocktype
 * j_tag    ;on-disk blocknum
 * }
 */
void open_journal_file(char* name, int* fd){
   *fd = open_device(name);
};

void read_journal(struct struct_ext2_filsys filsys){
    struct ext2_inode inode;
    ext2fs_read_inode(&filsys, EXT2_JOURNAL_INO, &inode);   
};  

void read_journal_sb(struct struct_ext2_filsys* filsys, int fd){
    struct ext2_inode inode;
    ext2fs_read_inode(filsys, EXT2_JOURNAL_INO, &inode);
    __u32 pos = (inode.i_block[0])*(filsys->blocksize);
    //char* buff = (char*)malloc(68); /*super block size is 68*/
    struct journal_superblock_s j_sb;
    /*i dont know its work or no*/
    //j_sb = *((struct journal_superblock_s*)(buff));
    memset(&j_sb, 0, 68);
    pread(fd, (char*)&j_sb, 68, pos);

    printf(
            "Superblock info:\n"
            "----Header info:\n"
            "----Magic      :0x%x\n"
            "----Blocktype  :0x%x\n"
            "----Sequence   :0x%x\n" 
            "S_blocksize    :0x%x\n"
            "S_maxlen       :0x%x\n"
            "First log inf  :0x%x\n",
            j_sb.s_header.h_magic,
            j_sb.s_header.h_blocktype,
            j_sb.s_header.h_sequence,
            j_sb.s_blocksize,
            j_sb.s_maxlen, 
            j_sb.s_first
            );
    //free(buff);
}

void read_journal_tag();
void read_journal_descriptor();

void print_journal(){

};

void close_journal_file(int fd){
    close_device(fd);
}; 


#endif //__EXT3_RECOVERY__