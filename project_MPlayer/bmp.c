#include "bmp.h"

//��ʾͼƬ����
int show_bmp(char *filename, int x1, int y1)
{
	//��ͼƬ
	int fd = open(filename, O_RDWR);
	if(fd < 0)
	{
		perror("open error ");
		return -1;
	}
	
	/* ʵ��ͷ��Ϣ��ɫ����Ϣ��ƫ���� �����54
	ͼ��Ŀ�� 
	ͼ��ĸ߶� 
	ͼ���λͼ read(); */
	int px,w,h;
	short wt;
	
	lseek(fd, 0xa, SEEK_SET);
	read(fd, &px, 4);
	lseek(fd, 0x12, SEEK_SET);
	read(fd, &w, 4);//���
	read(fd, &h, 4);//�߶�
	lseek(fd, 0x1c, SEEK_SET);
	read(fd, &wt, 2);//λͼ  24  ��������ԭɫ R G B
/* 	printf("px %d\n", px);
	printf("w %d  h %d\n",w,h);
	printf("wt %d\n", wt); */
	//���̫���˾Ͳ�����ʾ�����п��ܻ�δ���
	if(x1+w > 800)
	{
		printf("̫����\n");
		return -1;
	}
	else if(y1+ h > 480)
	{
		printf("̫����\n");
		return -1;
	}
	//��ʾͼƬ
	unsigned char buff[w*h*wt/8 + ((w%4)*h)];
	int x,y;
	unsigned char r,g,b,*p=buff;
	//��ȡԭʼ������Ϣ RGB 
	
	lseek(fd, px, SEEK_SET);
	
	read(fd, buff, w*h*wt/8 + ((w%4)*h));
	
	//����ȡ����rgb��Ϣ��ʾ����������
	for(y = h-1; y >= 0; y--)//д�߶� 0-479  --- 479-0
	{
		for(x=0; x < w; x++)//д��� 0-799
		{
			b = *p++;
			g = *p++;
			r = *p++;
			
			
			*(lcd+(x+x1) + (y1+y)*800) = r << 16 | g << 8 | b; //0xff1122
							//bmp��ȡ�������� ��ɫͨ�� RGB 
		}
		for(x=0; x < (w%4); x++)
		{
			p++;//ָ��p��ƫ���� w%4 �� ��ַ
		}
	}
	close(fd);
}