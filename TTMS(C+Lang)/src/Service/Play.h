#pragma once

#include "../Common/Common.h"
#include <ctype.h>

typedef enum {
	PLAY_TYPE_FILE=1,
	PLAY_TYPE_OPEAR=2,
	PLAY_TYPE_CONCERT=3
} play_type_t;//��Ŀ���Ͷ��壬1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�

typedef enum {
	PLAY_RATE_CHILD = 1,
	PLAY_RATE_TEENAGE = 2,
	PLAY_RATE_ADULT = 3
} play_rating_t;//�ݳ��������Ͷ��壬1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�

typedef struct {//��Ŀ��Ϣ����
	int id;                  //��ϢID��
	char name[31];           //��Ŀ����
	play_type_t type;        //��Ŀ����
	char area[9];            //��Ŀ��Դ����
	play_rating_t rating;    //��Ŀ����
	int duration; //�Է���Ϊ��λ   //�ݳ�ʱ��
	user_date_t start_date;  //��ӳ����
	user_date_t end_date;    //��������
	int price;               //Ʊ�ļ۸�
} play_t;


typedef struct play_node {
	play_t data;
	struct play_node *next, *prev;
} play_node_t, *play_list_t;

int Play_Srv_Add(const play_t *data);

int Play_Srv_Modify(const play_t *data);

int Play_Srv_DeleteByID(int ID);

int Play_Srv_FetchByID(int ID, play_t *buf);

int Play_Srv_FetchAll(play_list_t list);

//�������ƻ�ȡ��Ŀ
int Play_Srv_FetchByName(play_list_t list, char condt[]);
int Play_Srv_FilterByName(play_list_t list, char condt[]);

