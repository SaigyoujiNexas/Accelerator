#pragma once 

#include "../Common/common.h"
typedef enum{
	SALE_SELL=1,        //买票 
	SALE_REFOUND=-1     //退票 
}sale_type_t;
typedef struct{
	long id;//销售记录ID 
	int user_id;//售票员ID 
	int ticket_id;//票ID 
	user_date_t date;//处理日期 
	user_time_t time;//处理时间 
	int value;//票价 
	sale_type_t type;//交易类型 
}sale_t;
typedef struct sale_node{
	sale_t data;//实体数据 
	struct sale_node *next;//后方指针 
	struct sale_node *prev;//前方指针 
}sale_node_t,*sale_list_t;

int Sale_Srv_Add(const sale_t* data);



int Sale_Srv_FetchByTicketID(int ticket_id, sale_t* sale);
