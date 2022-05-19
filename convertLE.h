#ifndef _CONVERT_
#define _CONVERT_

#include <ext2fs/ext2_types.h>

typedef union u32Format_big_endian
{
    unsigned char buff[4];
    __u32 d_word;
}u32Format_big_endian_t;

__u32 convert_from_num(__u32 number){

    u32Format_big_endian_t num;
    num.d_word= number; 
    __u32 ret = 0; 
    return ret = (__u32)num.buff[3] | (__u32)num.buff[2] << 8 |  (__u32)num.buff[1] << 16 | (__u32)num.buff[0] << 24 ;
}
__u32 convert_from_char(unsigned char *number){

    __u32 ret = 0; 
    return ret = (__u32)number[3] | (__u32)number[2] << 8 |  (__u32)number[1] << 16 | (__u32)number[0] << 24 ;
}

#endif //_CONVERT_