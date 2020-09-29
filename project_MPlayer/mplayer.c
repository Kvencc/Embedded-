#include "mplayer.h"

int *lcd = NULL;

int main()
{
	
	int lcdfd = open("/dev/fb0", O_RDWR);
	if(lcdfd < 0)
	{
		perror("open error");
	}
	
	lcd = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcdfd, 0);
	if(lcd  != ((void *) -1))
	{
		printf("mmap ok\n");
	}
	
	struct list_pic *head = create_head();
	struct list_video *vhead = create_video_head();
	struct list_pic *sshead = create_head();
	show_bmp("mainUI.bmp", 0, 0);
	find_pic(head,"/root/bmp");
	screenshots("/root/video", sshead);
	find_video(vhead, "/root/video");
	
	int x,y;
	
	while(1)
	{	
		get_event(&x, &y);
			
		if(x>400 && x<520 && y>125 && y<220)
		{
			pic_bianli(head);
		} 
		
		if(x>220 && x<350 && y>240 && y<350)
		{
			pic_ctrl(head);
		}
		
		if(x>220 && x<340 && y>120 && y<220)
		{
			video_ctrl(vhead);
		}
		
		if(x>400 && x<515 && y>255 && y<360)
		{
			//说明--使用字库
		}
		
		if(x>686 && x<777 && y>388 && y<475)
		{
			show_bmp("001.bmp",0,0);
			exit(0);
		}
	}

	munmap(lcd, 800*480*4);
	close(lcdfd);
	
	
	return 0;
}