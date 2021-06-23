
#include "Schedule.h"
#include "../Common/list.h"
#include "Ticket.h"
#include <stdio.h>
#include "../Persistence/Schedule_Persist.h"

int Schedule_Srv_Add(const schedule_t *data) 
{
	return Schedule_Perst_Insert(data);
}

int Schedule_Srv_Modify(const schedule_t *data) {
	return Schedule_Perst_Update(data);
}


int Schedule_Srv_DeleteByID(int ID) {
	return Schedule_Perst_DeleteByID(ID);
}

int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
	return Schedule_Perst_SelectByID(ID, buf);
} 

int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id)
{
	return Schedule_Perst_SelectByPlay(list, play_id);
}

int Schedule_Srv_FetchAll(schedule_list_t list) 
{
	return Schedule_Perst_SelectAll(list);
}



int Schedule_Srv_StatRevByPlay(int play_id, int* soldCount)
{
	*soldCount = 0;
	int value = 0;
	int sold = 0;
	schedule_list_t list;
	schedule_node_t* pos;

	List_Init(list, schedule_node_t);

	Schedule_Perst_SelectByPlay(list, play_id);
	List_ForEach(list, pos)
	{
		value += Ticket_Srv_StatRevBySchID(pos->data.id, &sold);
		(*soldCount) += sold;
	}
	List_Destroy(list, schedule_node_t);
	return value;
}

int Schedule_Srv_FetchByStudio(schedule_list_t list, int studio_id)
{
	return Schedule_Perst_SelectByStudio(list, studio_id);
}