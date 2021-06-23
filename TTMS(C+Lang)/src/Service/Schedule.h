#pragma once

#include "../Common/Common.h"


#define SCHEDULE_KEY_NAME "Schedule"

typedef struct {//�ݳ��ƻ����Ͷ���
	long int id;            //�ݳ��ƻ�ID��
	int play_id;       //�ݳ��ľ�ĿID��
	int studio_id;     //�ݳ��ķ�ӳ��ID��
	user_date_t date;  //�ݳ�����
	user_time_t time;  //�ݳ�ʱ��
	int seat_count;    //��λ����
}schedule_t;


typedef struct schedule_node{
	schedule_t data;
	struct schedule_node* next, * prev;
} schedule_node_t, * schedule_list_t;


int Schedule_Srv_Add(const schedule_t *data);


int Schedule_Srv_Modify(const schedule_t *data);

int Schedule_Srv_DeleteByID(int ID);


int Schedule_Srv_FetchByID(int ID, schedule_t *buf);

int Schedule_Srv_FetchAll(schedule_list_t list);


int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id);


int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount);



int Schedule_Srv_FetchByStudio(schedule_list_t list, int studio_id); 



