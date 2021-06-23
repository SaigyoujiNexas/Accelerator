#pragma once

#include "../Common/Common.h"


#define SCHEDULE_KEY_NAME "Schedule"

typedef struct {//演出计划类型定义
	long int id;            //演出计划ID号
	int play_id;       //演出的剧目ID号
	int studio_id;     //演出的放映厅ID号
	user_date_t date;  //演出日期
	user_time_t time;  //演出时间
	int seat_count;    //座位数量
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



