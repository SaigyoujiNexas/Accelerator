#pragma once


	#include "../Common/list.h"
	#include "../Service/play.h"
#define PLAY_PAGE_SIZE 6;

	int Play_UI_Add(void);

	int Play_UI_Modify(int id);

	int Play_UI_Delete(int id);

	int Play_UI_Query(int id);

	void Play_UI_ShowList(play_list_t list, Pagination_t paging);

	void Play_UI_MgtEntry();


