#ifndef __EXAMPLE_H
#define __EXAMPLE_H

#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


//�ⲿȫ�ֱ���
extern int *lcd;

//�������ṹ��
struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;


//���������Ϣ
METHODDEF(void)
my_error_exit (j_common_ptr cinfo);


//����jpeg�ļ��ɹ�����ֵ1  ʧ�ܷ���0
GLOBAL(int)
read_JPEG_file (char * filename ,int px,int py);


#endif
