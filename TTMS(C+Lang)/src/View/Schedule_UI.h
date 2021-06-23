#pragma once

	#include "../Common/list.h"
	#include "../Service/Schedule.h"
	#include "../Service/Play.h"
#define SCHEDULE_PAGE_SIZE 5

	int Schedule_UI_Add(int play_id);
	int Schedule_UI_Modify(int id);
	int Schedule_UI_Delete(int id);
	int Schedule_UI_Query(char * play_name);
	void Schedule_UI_MgtEntry(int play_id);
//	void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging);
	void Schedule_UI_ListAll(void);
	void Schedule_UI_Show_List(schedule_list_t list, Pagination_t paging);


