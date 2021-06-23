#pragma once



//票状态类型
typedef enum{
	TICKET_AVL=0,         //待售 
	TICKET_SOLD=1,        //已售 
	TICKET_RESV=9         //预留 
}ticket_status_t;

//票实体数据类型 
typedef struct{
	int id;       
	int schedule_id;
	int seat_id;
	int price;
	ticket_status_t status; 
}ticket_t;

//票链表结点
typedef struct ticket_node {
	ticket_t data;
	struct ticket_node* next, * prev;
}ticket_node_t;

typedef ticket_node_t* ticket_list_t;


//根据计划ID提取所有演出票
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id);

int Ticket_Srv_FetchAll(ticket_list_t list);

//根据座位ID在list中找对应票
ticket_node_t * Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id);

void Ticket_Srv_GenBatch(int schedule_id);


int Ticket_Srv_DeleteBatch(int schedule_id);

int Ticket_Srv_Modify(const ticket_t * data);

int Ticket_Srv_FetchByID(int ID, ticket_t *buf) ;

int Ticket_Srv_StatRevBySchID(int schedule_id, int* soldCount);

int Ticket_Srv_FetchSold(ticket_list_t list, ticket_list_t listSold);
