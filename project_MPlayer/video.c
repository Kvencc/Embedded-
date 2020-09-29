#include "video.h"

struct list_video* create_video_head(void)
{
	
	struct list_video* buff = (struct list_video*)malloc(sizeof(struct list_video));
	
	if(buff == NULL)
	{
		printf("create head error");
		return NULL;
	}
	buff->next = NULL;
	buff->prev = NULL;
	
	return buff;
}

void inst_video(struct list_video *vhead, char* filename)
{
	
	char video_name[1024];
	struct list_video *p = vhead;
	
	struct list_video *buff = (struct list_video*)malloc(sizeof(struct list_video));
	if(buff == NULL)
	{
		printf("inster error");
		return;
	}
	
	strcpy(buff->video_name, filename);
	
	buff->next = NULL;
	buff->prev = NULL;
	
	for(;p->next!=NULL; p=p->next);
	
	p->next = buff;
	buff->prev = p;
}


void find_video(struct list_video *vhead, char *filepath)
{
	
	DIR* dir = opendir(filepath);
	if(dir == NULL)
	{
		perror("dir open error!!!");
		return;
	}
	
	struct dirent *di =NULL;
	
	while(1)
	{
		di = readdir(dir);
		if(di == NULL)
			break;
		if(!strcmp(di->d_name, ".") || !strcmp(di->d_name, ".."))
			continue;
		else if(di->d_type == DT_REG)
		{
			char *p = di->d_name;
			char buff[1024] = {0};
			sprintf(buff, "%s/%s", filepath, di->d_name);
			
			if((!strcmp(p + (strlen(p)-4), ".avi")) || (!strcmp(p + (strlen(p)-4), ".mp4")))
			{
				inst_video(vhead, buff);
			}
			
		}else if(di->d_type == DT_DIR)
		{
			char buff[2048]={0};
			sprintf(buff,"%s/%s", filepath, di->d_name);
			find_video(vhead, buff);
		}
	}
	closedir(dir);
}


void del_avi(char *old,char *new)//123.avi  321
{
	if(old==NULL||new==NULL)
		return;
	int i=0,j=0;
	for(i=0;i<strlen(old);i++)
	{
		if(old[i]=='.')
		{
			break;
		}
	}
	for(j=0; j<i; j++)
	{
		new[j]=old[j];
	}
	printf("del_avi %s\n",new);
}

int screenshots(char *filepath, struct list_pic *sshead)
{
	
	DIR *dir = opendir(filepath);//打开目录
	if(dir==NULL)
	{
		return 0;
	}
	while(1)
	{
		struct dirent * di = readdir(dir);//读目录
		if(di==NULL)
		{
			break;
		}
		if(!strcmp(di->d_name,".")||!strcmp(di->d_name,".."))
			continue;
		else if(di->d_type == DT_REG)
		{
			char buff[2048]={0},temp[1024]={0};
			printf("readdir filename %s\n",di->d_name);
			sprintf(buff,"%s %s/%s", ABBR, filepath, di->d_name);
			system(buff);
			//每次生成的略缩图都是 00000001.jpg 修改文件名
			del_avi(di->d_name, temp);//去除avi的函数
			sprintf(buff,"%s.jpg", temp);// 2.avi.jpg
			rename("00000001.jpg",buff);
			inst_pic(sshead, buff);
		}
		//else if(di->d_type);
	}
	//退出
	closedir(dir);
	return 0;
}

void play_video(struct list_video* node)
{
	
	char buff[1024] = {0};
	
	sprintf(buff, "%s%s %s", PLAY, node->video_name, "&");
	show_bmp("/root/bmp/ctrl.bmp", 0, 418);
	system(buff);
}

int video_ctrl(struct list_video* vhead)
{
	struct list_video *p = vhead->next;
	int x,y;
	int flag = 0;
	
	play_video(p);
	
	int fd = open("/root/fifo1", O_RDWR);
	if(fd == -1)
	{
		perror("/root/fifo1");
		return -1;
	}
	
	while(1)
	{	
		get_event(&x, &y);
			
		if(x>24 && x<80 && y>420 && y<475)//播放
		{
			if(flag == 1)
			{
				write(fd, PAUSE, strlen(PAUSE));
				printf("播放\n");
				flag = 0;
			}
			
		} 
		if(x>103 && x<162 && y>420 && y<475)//暂停
		{
			if(flag == 0)
			{
				write(fd, PAUSE, strlen(PAUSE));
				printf("暂停\n");
				flag = 1;
			}
			
		}
		if(x>193 && x<244 && y>420 && y<475)//快进
		{
			write(fd, FORWARD, strlen(FORWARD));
			printf("快进\n");
		}
		if(x>278 && x<335 && y>420 && y<475)//快退
		{
			write(fd, BACKWARD, strlen(BACKWARD));
			printf("快退\n");
		}
		if(x>365 && x<440 && y>420 && y<475)//增大音量
		{
			write(fd, VOLUME_UP, strlen(VOLUME_UP));
			printf("音量+\n");
		}
		if(x>455 && x<510 && y>420 && y<475)//减小音量
		{
			write(fd, VOLUME_DOWN, strlen(VOLUME_DOWN));
			printf("音量-\n");
		}
		if(x>540 && x<600 && y>420 && y<475)//上一首
		{
			if(p->prev != vhead)
			{
				p = p->prev;
			}
			else
			{
				for(;p->next!=NULL;p=p->next);
			}
			system("killall mplayer");
			play_video(p);
		}
		if(x>630 && x<690 && y>420 && y<475)//下一首
		{
			if(p->next!=NULL){
				p = p->next;
			}
			else
			{
				p = vhead->next;
			}
			system("killall mplayer");
			play_video(p);
		}
		if(x>720 && x<775 && y>420 && y<475)//退出
		{
			system("killall mplayer");
			//回到主界面
			show_bmp("mainUI.bmp", 0, 0);
			return 0;
		}
	}
}