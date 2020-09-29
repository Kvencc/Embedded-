#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h>

#include "bmp.h"
#include "example.h"
#include "pic.h"
#include "event.c"
#include "show_freetype.c"

#define msleep(value) usleep(value*1000);

struct list_pic* create_head(void){
	
	struct list_pic* buff = (struct list_pic*)malloc(sizeof(struct list_pic));
	
	if(buff == NULL){
		printf("create head error");
		return NULL;
	}
	buff->next = NULL;
	buff->prev = NULL;
	
	return buff;
}

void inst_pic(struct list_pic *head, char* filename){
	
	char pic_name[1024];
	struct list_pic *p = head;
	
	struct list_pic *buff = (struct list_pic*)malloc(sizeof(struct list_pic));
	if(buff == NULL){
	printf("inster error");
	return;
	}
	
	strcpy(buff->pic_name, filename);
	
	buff->next = NULL;
	buff->prev = NULL;
	
	for(;p->next!=NULL; p=p->next);
	
	p->next = buff;
	buff->prev = p;
	
}

void del_node(struct list_pic *node){
	
	struct list_pic *p = node, *q;
	q = p->next;
	
	if(p->next!=NULL){
		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->next = NULL;
		p->prev = NULL;
		free(p);
	}else{
		p->prev->next = NULL;
		p->next = NULL;
		p->prev = NULL;
		free(p);
	}
}

void print(struct list_pic *head){
	
	struct list_pic *p = head->next;
	for(;p!=NULL;p=p->next){
		printf("filename: %s", p->pic_name);
	}
}

void *show(void *arg)//arg == &head == 0xbec91a74
{
	//arg == &head;
	
	struct list_pic *p = (struct list_pic*)&arg;
	//arg == p
	printf("&arg %p\n", &arg);
	printf("p %p\n", p);
	printf("*p %p\n", *p);
	while(1)
	{
		for(p = p->next;p!=NULL; p=p->next)
		{
			clean(lcd,0,0,800,480,0);
			if(!strcmp(p->pic_name + (strlen(p->pic_name)-4), ".bmp"))
			{
				show_bmp(p->pic_name, 0, 0);
				msleep(500);
			}else{
				read_JPEG_file(p->pic_name, 0, 0);
				msleep(500);
			}
		}
			
	}
	
}

void show_one(struct list_pic *node){
	
	if(!strcmp(node->pic_name + (strlen(node->pic_name)-4), ".bmp")){
			show_bmp(node->pic_name, 0, 0);
		}else{
			//show_bmp("001.bmp",0,0);
			read_JPEG_file(node->pic_name, 0, 0);
		}
}

void load(struct list_pic *head){
	
	int flag = 0;
	struct list_pic *p = head->next;
	
	for(;p->next!=NULL;p=p->next)
		flag++;
	if(flag <= 2)
		find_pic(head, "./");
	else
		return;
}

void find_pic(struct list_pic *head, char *filepath){
	
	DIR* dir = opendir(filepath);
	if(dir == NULL)
	{
		perror("dir open error!!!");
		return;
	}
	
	struct dirent *di =NULL;
	
	while(1){
		di = readdir(dir);
		if(di == NULL)
			break;
		if(!strcmp(di->d_name, ".") || !strcmp(di->d_name, ".."))
			continue;
		else if(di->d_type == DT_REG){
			char *p = di->d_name;
			char buff[1024] = {0};
			sprintf(buff, "%s/%s", filepath, di->d_name);
			
			if(!strcmp(di->d_name,"mainUI.bmp") || !strcmp(di->d_name,"001.bmp") || !strcmp(di->d_name,"9527.bmp") || !strcmp(di->d_name,"ctrl.bmp"))
				continue;
			
			if(!strcmp(p + (strlen(p)-4), ".jpg")){
				inst_pic(head, buff);
			}
			else if(!strcmp(p + (strlen(p)-4), ".bmp")){
				inst_pic(head, buff);
			}
			
		}else if(di->d_type == DT_DIR){
			char buff[2048]={0};
			sprintf(buff,"%s/%s", filepath, di->d_name);
			find_pic(head, buff);
		}
	}
	closedir(dir);
}

void pic_bianli(struct list_pic *head)
{
	pthread_t thread;
	pthread_create(&thread, NULL, show, (void*)&head);

	printf("&head %p\n", &head);
	while(1)
	{
		int ret = get_event2();
		if(ret==1 || ret==2 || ret==3 || ret==4)
			pthread_cancel(thread);
	}
	pthread_join(thread, NULL);
}

void pic_ctrl(struct list_pic *head){
	
	struct list_pic * p = head->next;
	int x,y;
	int ret, flag=0;
	
	if(p!=NULL)
	{
		flag = 1;
		show_one(p);
	}else;

	if(flag == 1){
		while(1){
			ret = get_event2();
			if(1 == ret){//左滑下一张
				p = p->next;
				if(p!=NULL){
					show_one(p);
				}else
					p=head->next;
					show_one(p);
			}else if(2 == ret){//右滑上一张
				p = p->prev;
				if(p->prev!=NULL){
					show_one(p);
				}else{
					for(;p->next!=NULL;p=p->next);
					show_one(p);
				}
			}else if(3 == ret){//上滑删除
				if(p->next!=NULL){
					struct list_pic *q = p->next;
					show_bmp("9527.bmp", 200, 420);
					get_event(&x, &y);
					if(x>254 && x<320 && y>440 && y<470){
						del_node(p);
						show_one(q);
						p = q;
					}
					else if(x>487 && x<552 && y>444 && y<473){
						show_one(p);
					}
				}else if(p->next==NULL){//p->next==NULL
					if(p->prev != NULL && p->prev!=head){
						struct list_pic *q = p->prev;
						show_bmp("9527.bmp", 200, 420);
						get_event(&x, &y);
						if(x>254 && x<320 && y>440 && y<470){
								del_node(p);
								p = q;
								show_one(p);
						}else if(x>487 && x<552 && y>444 && y<473){
							show_one(p);
						}
					}else{
						/* p->next=NULL;
						p->prev=NULL;
						free(p); */
						show_bmp("main.bmp", 0, 0);
						break;
					}
				}
			}else if(4 == ret){//下滑回到主界面
				for(;p->prev!=NULL; p=p->prev);
				show_bmp("mainUI.bmp", 0, 0);
				break;
			}else{
				;
			}
		}
	}else return;
}