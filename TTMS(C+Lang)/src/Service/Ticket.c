#include "Ticket.h"

#include "../Common/list.h"
#include "../Service/schedule.h"
#include "../Service/play.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Persistence/Play_Persist.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/Seat.h"
#include "../Service/EntityKey.h"

#include "../Service/Sale.h"
#include <stdio.h>
#include <stdlib.h>



//根据座位ID在list中找对应票
ticket_node_t * Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id){
	ticket_node_t *p;
	List_ForEach(list, p){
		if(p->data.seat_id==seat_id)
			return p;
	}
	return NULL;
}


//根据计划ID提取所有演出票
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id) 
{

	return Ticket_Perst_SelectBySchID(list, schedule_id);

}


int Ticket_Srv_DeleteBatch(int schedule_id) {
	return Ticket_Perst_Delete(schedule_id);
}

int Ticket_Srv_Modify(const ticket_t * data){
	return Ticket_Perst_Update(data);
}

int Ticket_Srv_FetchByID(int ID, ticket_t *buf) {
	return Ticket_Perst_SelectByID(ID, buf);
}
int Ticket_Srv_StatRevBySchID(int schedule_id, int* soldCount)
{
	int value = 0;
	*soldCount = 0;
	sale_t sale;
	ticket_list_t list;
	ticket_node_t* pos;

	List_Init(list, ticket_node_t);

	*soldCount = Ticket_Srv_FetchBySchID(list, schedule_id);
	List_ForEach(list, pos)
	{
		if (Sale_Srv_FetchByTicketID(pos->data.id, &sale) == 0)
			continue;
		printf("type:%d\n", sale.type);
		if (pos->data.status == TICKET_SOLD && sale.type == SALE_SELL)
		{
			printf("test");
			(*soldCount)++;
			value += pos->data.price;
		}
	}
	List_Destroy(list, ticket_node_t);
	return value;
}

int Ticket_Srv_FetchAll(ticket_list_t list)
{
	return Ticket_Perst_SelectAll(list);
}


void Ticket_Srv_GenBatch(int schedule_id)
{
	int i;
	int count;
	seat_list_t seat_head;
	schedule_t schedule;
	ticket_list_t ticket_head;
	ticket_t data;
	play_t play;
	long id_seed;

	seat_node_t* pos;

	List_Init(ticket_head, ticket_node_t);
	List_Init(seat_head, seat_node_t);

	Schedule_Srv_FetchByID(schedule_id, &schedule);
	Play_Srv_FetchByID(schedule.play_id, &play);
	count = Seat_Srv_FetchValidByRoomID(seat_head, schedule.studio_id);
	
	id_seed = EntKey_Srv_CompNewKeys("Ticket", count);

	List_ForEach(seat_head, pos)
	{
		data.seat_id = pos->data.id;
		data.schedule_id = schedule_id;
		data.status = TICKET_AVL;	//表示待售
		data.id = id_seed++;
		data.price = play.price;		//默认统一票价
		ticket_node_t* newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
		if (!newNode)
		{
			printf("错误!!!\n");
			break;
		}
		newNode->data = data;
		List_AddTail(ticket_head, newNode);
	}
	Ticket_Perst_Insert(ticket_head);
	List_Destroy(ticket_head, ticket_node_t);
	List_Destroy(seat_head, seat_node_t);
	return;

}
int Ticket_Srv_FetchSold(ticket_list_t list, ticket_list_t listSold)
{
	int retval = 0;
	ticket_node_t* node;
	List_ForEach(list, node)
	{
		if (node->data.status == TICKET_SOLD)
		{
			ticket_node_t* newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
			newNode->data = node->data;
			List_AddTail(listSold, newNode);
			retval++;
		}
	}
	return retval;
}

