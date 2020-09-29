#ifndef __VIDEO_H
#define __VIDEO_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "bmp.c"

//宏定义
#define ABBR "mplayer -ss 00:00:5 -noframedrop -nosound  -nolirc -nojoystick -zoom -x 100 -y 100 -vo jpeg -frames 1 "
#define PLAY "mplayer -quiet -slave -af volume=-20 -zoom -x 800 -y 420 -input file=/root/fifo1 "
#define PAUSE "pause\n"
#define FORWARD "seek +5\n"
#define BACKWARD "seek -5\n"
#define VOLUME_UP "volume +5\n"
#define VOLUME_DOWN "volume -5\n"

//结构体声明
struct list_video
{
	
	char video_name[1024];
	
	struct list_video *prev;
	struct list_video *next;
};

//函数声明
struct list_video* create_video_head(void);
void inst_video(struct list_video *vhead, char* filename);
void find_video(struct list_video *vhead, char *filepath);

void play_video(struct list_video* node);
void del_avi(char *old,char *new);
int screenshots(char *filepath, struct list_pic *sshead);
int video_ctrl(struct list_video* vhead);

#endif