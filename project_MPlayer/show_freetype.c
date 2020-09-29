#include <stdio.h>

#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wchar.h>
#include "lcd_app.h"
#include <time.h>

void clean(unsigned int*lcd_p,int px,int py,int x,int y,int color)
{

	lcd_p = lcd_p+py*800+px;
	
	for(py=0;py<y;py++)
	for(px=0;px<x;px++)
	{
		*(lcd_p+py*800+px) = color;
	}	
}



int show_freetype(char *ch)
{
	
	//fb0设备需要打开
	int fd = open("/dev/fb0",O_RDWR);
	if(fd == -1)
	{
		perror("test_fifo");
		exit(-1);
	}
	
	int *lcd = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	
	wchar_t tbuf[1000] = {0};
	get_lcd_info();//获取LCD屏幕信息
	
	swprintf(tbuf, 1000, L"%s", ch); 

	clean(lcd,0,0,800,480,0);
	Lcd_Show_FreeType(lcd, tbuf, 50,0xffffff/*白色*/,1,0/*黑色*/, 100, 220);
	
	munmap(lcd,800*480*4);
	close(fd);
	return 0;
}
