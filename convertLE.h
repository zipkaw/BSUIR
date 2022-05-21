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
__u32 convert_from_char(unsigned char *number, __u32 i){

    /*i it is 4 next byte we want to read
     *
     */
    if(i>=1024){
        fprintf(stderr, "big offset"); 
    }
    __u32 offset = i * sizeof(__u32); 
    __u32 ret = 0; 
    return ret = (__u32)number[i+3] | (__u32)number[i+2] << 8 |  (__u32)number[i+1] << 16 | (__u32)number[i+0] << 24 ;
}

#endif //_CONVERT_