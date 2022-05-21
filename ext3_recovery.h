#ifndef __EXT3_RECOVERY__
#define __EXT3_RECOVERY__

#include "ext2_recovery.h"
#include "ext_source.h"
#include "journal.h"
#include "convertLE.h"
#include "reading_blocks.h"
#include <time.h>

/*open journal(inode 8)*/
/*j_desc
 *{j_header ;sequence, blocktype
 * j_tag    ;on-disk blocknum
 * }
 *
 * open journal
 *  read block 1
 *  descr block analyze this
 *  when next block is doesnt have magic and type 2 - commit block
 *  read next block !!is descr block
 *  anlyse and write info in  own tag structure
 *  read until commit block
 *  repeat
 */

void open_journal_file(char *name, int *fd)
{
    *fd = open_device(name);
};

void close_journal_file(int fd)
{
    close_device(fd);
};

void read_journal_sb(struct struct_ext2_filsys *filsys, int fd)
{
    struct ext2_inode inode;
    ext2fs_read_inode(filsys, EXT2_JOURNAL_INO, &inode);
    __u32 pos = (inode.i_block[0]) * (filsys->blocksize);
    // char* buff = (char*)malloc(68); /*super block size is 68*/
    struct journal_superblock_s j_sb;
    /*i dont know its work or no*/
    // j_sb = *((struct journal_superblock_s*)(buff));
    memset(&j_sb, 0, 68);
    pread(fd, &j_sb, 68, pos);

    printf(
        "Superblock info:\n"
        "----Header info:\n"
        "----Magic      :0x%x\n"
        "----Blocktype  :0x%x\n"
        "----Sequence   :0x%x\n"
        "S_blocksize    :0x%x\n"
        "S_maxlen       :0x%x\n"
        "start log      :0x%x\n"
        "journal size   :%u\n",
        convert_from_num(j_sb.s_header.h_magic),
        convert_from_num(j_sb.s_header.h_blocktype),
        convert_from_num(j_sb.s_header.h_sequence),
        convert_from_num(j_sb.s_blocksize),
        convert_from_num(j_sb.s_maxlen),
        convert_from_num(j_sb.s_start),
        inode.i_size

    );
    // free(buff);
}

void read_journal(struct struct_ext2_filsys *filsys, int fd, __u32 offset, __u32 index_in_block)
{   
    struct tm ts;
    char t_buff[80]
    struct ext2_inode inode;
    ext2fs_read_inode(filsys, EXT2_JOURNAL_INO, &inode);
    char *buff;
    __u32 type;
    int seq_flag = 0;
    /* 1 descr
     * 2 metadata
     * 3 commit or revoke
     */
    int num_in_block = 0;
    int find_flag = 0;
    /* 0 not found cont search
        1 found start recover;
     */
    int break_flag = 0;
    int i = 1; 
    do
    {
        buff = read_inode_iblock(inode, fd, i, *(filsys->super), filsys->blocksize);
        /*remember: buff has size = block size*/
        journal_descriptor_block_t j_desc;
        journal_block_tag2_t tag;
        memcpy(&j_desc, buff, sizeof(struct journal_descriptor_block));
        switch (j_desc.d_header.h_blocktype)
        {
        case descriptor_block:
            memcpy(&tag, buff + sizeof(journal_descriptor_block_t), sizeof(journal_block_tag2_s));
            if (tag.t_blocknr == offset && (tag.t_blocknr == 0 && tag.t_flags == 0))
            {
                flag = 1;
                break;
            }
            else
            {
                i++;
                num_in_block++;
            }
        case commit_block:
            break;
        case revoke_block:
            break;
        default:
            break;
        }
        if (flag)
        {
            buff = read_inode_iblock(inode, fd, ++i, *(filsys->super), filsys->blocksize);
            memcpy(&inode, buff + (filsys->super->s_inode_size) * (index_in_block), filsys->super->s_inode_size); 
            if (!read_inode_blocks(inode, fd, i,  *(filsys->super), filsys->blocksize)){
                fprintf(stdout, "File undelete!\n"); 
            }
        }
    } while (break_flag);

}

#endif //__EXT3_RECOVERY__