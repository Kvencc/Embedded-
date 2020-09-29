#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>
int get_event2();
int get_event(int *x,int *y);

int get_event(int *x,int *y){
		//读取触摸设备
	//open read  打印数据 close 
	
	int fd = open("/dev/input/event0", O_RDWR);
	if(fd==-1)
	{
		perror("input error");
		return -1;
	}
	struct input_event ie;
	
	int a=0,b=0;
	//读取触摸设备
	while(1)
	{
		read(fd, &ie, sizeof(ie));
	
		if(ie.type==3 && ie.code==0)//x轴坐标
		{
			*x=ie.value;
			a=1;
		}
		else if(ie.type==3 && ie.code==1)//y轴坐标
		{
			*y=ie.value;
			b=1;
		}
		if(a && b)
		{
			break;
		}
	}
	close(fd);
	return 0;
}


int get_event2()
{
	int x, y, x1, y1;
	int fd = open("/dev/input/event0", O_RDWR);
	if(fd==-1)
	{
		perror("input error");
		return -1;
	}
	struct input_event ie;
	
	int a=0,b=0;
	//读取触摸设备
	while(1)
	{
		read(fd, &ie, sizeof(ie));
	
		//printf("type %d   code %d   value %d\n", ie.type, ie.code, ie.value);
		if(ie.type==3 && ie.code == 0){
			x=ie.value;
		}else if(ie.type==3 && ie.code == 1){
			y=ie.value;
		}
		else if(ie.type==1 && ie.code == 330 && ie.value==1){
			x1=x;
			y1=y;
		}
		else if(ie.type==1 && ie.code == 330 && ie.value==0){
			
			//printf("(%d, %d), (%d, %d)", x1, y1, x, y);
			
			if((x1 - x) > 100)
			{
				//printf("左滑\n");
				close(fd);
				return 1;
			}
			
			if((x - x1) > 100)
			{
				//printf("右滑\n");
				close(fd);
				return 2;
			}
			if((y1 - y) > 100)
			{
				//printf("上滑\n");
				close(fd);
				return 3;
			}
			if((y - y1) > 100)
			{
				//printf("下滑\n");
				close(fd);
				return 4;
			}return 0;
		}
		
	}
	
}
