#pragma once 

#include "../Common/common.h"
typedef enum{
	SALE_SELL=1,        //��Ʊ 
	SALE_REFOUND=-1     //��Ʊ 
}sale_type_t;
typedef struct{
	long id;//���ۼ�¼ID 
	int user_id;//��ƱԱID 
	int ticket_id;//ƱID 
	user_date_t date;//�������� 
	user_time_t time;//����ʱ�� 
	int value;//Ʊ�� 
	sale_type_t type;//�������� 
}sale_t;
typedef struct sale_node{
	sale_t data;//ʵ������ 
	struct sale_node *next;//��ָ�� 
	struct sale_node *prev;//ǰ��ָ�� 
}sale_node_t,*sale_list_t;

int Sale_Srv_Add(const sale_t* data);



int Sale_Srv_FetchByTicketID(int ticket_id, sale_t* sale);
