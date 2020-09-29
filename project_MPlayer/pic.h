#ifndef __APS_H
#define __APS_H

struct list_pic {
	
	char pic_name[1024];
	
	struct list_pic *prev;
	struct list_pic *next;
};

struct list_pic* create_head(void);
void inst_pic(struct list_pic *head, char* filename);
void del_node(struct list_pic *node);
void print(struct list_pic *head);
void *show(void *arg);
void show_one(struct list_pic *node);
void find_pic(struct list_pic *head, char *filepath);

void pic_ctrl(struct list_pic *head);

#endif