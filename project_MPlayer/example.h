#ifndef __EXAMPLE_H
#define __EXAMPLE_H

#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


//外部全局变量
extern int *lcd;

//出错对象结构体
struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;


//处理出错信息
METHODDEF(void)
my_error_exit (j_common_ptr cinfo);


//解码jpeg文件成功返回值1  失败返回0
GLOBAL(int)
read_JPEG_file (char * filename ,int px,int py);


#endif
