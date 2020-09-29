#include "bmp.h"

//显示图片函数
int show_bmp(char *filename, int x1, int y1)
{
	//打开图片
	int fd = open(filename, O_RDWR);
	if(fd < 0)
	{
		perror("open error ");
		return -1;
	}
	
	/* 实际头信息与色彩信息的偏移量 结果是54
	图像的宽度 
	图像的高度 
	图像的位图 read(); */
	int px,w,h;
	short wt;
	
	lseek(fd, 0xa, SEEK_SET);
	read(fd, &px, 4);
	lseek(fd, 0x12, SEEK_SET);
	read(fd, &w, 4);//宽度
	read(fd, &h, 4);//高度
	lseek(fd, 0x1c, SEEK_SET);
	read(fd, &wt, 2);//位图  24  它有三个原色 R G B
/* 	printf("px %d\n", px);
	printf("w %d  h %d\n",w,h);
	printf("wt %d\n", wt); */
	//如果太宽了就不能显示正常有可能会段错误
	if(x1+w > 800)
	{
		printf("太长了\n");
		return -1;
	}
	else if(y1+ h > 480)
	{
		printf("太高了\n");
		return -1;
	}
	//显示图片
	unsigned char buff[w*h*wt/8 + ((w%4)*h)];
	int x,y;
	unsigned char r,g,b,*p=buff;
	//读取原始数据信息 RGB 
	
	lseek(fd, px, SEEK_SET);
	
	read(fd, buff, w*h*wt/8 + ((w%4)*h));
	
	//将读取到的rgb信息显示到开发板上
	for(y = h-1; y >= 0; y--)//写高度 0-479  --- 479-0
	{
		for(x=0; x < w; x++)//写宽度 0-799
		{
			b = *p++;
			g = *p++;
			r = *p++;
			
			
			*(lcd+(x+x1) + (y1+y)*800) = r << 16 | g << 8 | b; //0xff1122
							//bmp读取的数据是 颜色通道 RGB 
		}
		for(x=0; x < (w%4); x++)
		{
			p++;//指针p就偏移了 w%4 的 地址
		}
	}
	close(fd);
}