#include "Ticket_UI.h"

#include "../Common/list.h"
#include "../Service/Ticket.h"
#include "../Service/schedule.h"
#include "../Service/play.h"
#include "../Service/studio.h"

#include <stdio.h>
#include <stdlib.h>
inline char Ticket_UI_Status2Char(ticket_status_t status) {

	char statusChar;
	switch (status) 
	{
	case TICKET_AVL:		//待售座位
		statusChar = '#';
		break;
	case TICKET_SOLD:	
		statusChar = '@';
		break;
	case TICKET_RESV:
		statusChar = '%';
	default:
		statusChar = '~';
		break;
	}
	return statusChar;
}
int Ticket_UI_ShowTicket(int ID)
{
	char choice;
	schedule_list_t scheduleList;
	List_Init(scheduleList, schedule_node_t);
	play_list_t playList;
	List_Init(playList, play_node_t);
	Pagination_t paging;
	int play_id;
	int schedule_id;
	play_t play;
	schedule_t schedule;
	ticket_list_t ticketList;
	seat_list_t seatList;

	
	List_Init(seatList, seat_node_t);
	List_Init(ticketList, ticket_node_t);
	paging.totalRecords = Play_Srv_FetchAll(playList);
	paging.pageSize = PLAY_PAGE_SIZE;
	Paging_Locate_FirstPage(playList, paging);

	do
	{
		system("cls");
		Play_UI_ShowList(playList, paging);
		printf("\t\t\t\t\t[E]选择剧目  [P]上一页  [N]下一页  [R]退出\n\t\t\t\t");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
		case 'P':
			if (!Pageing_IsFirstPage(paging))
				Paging_Locate_OffsetPage(playList, paging, -1, play_node_t);
			break;
		case 'N':
			if (!Pageing_IsLastPage(paging))
				Paging_Locate_OffsetPage(playList, paging, 1, play_node_t);
			break;
		case 'R':
			return 0;
			break;
		}

	} while (choice != 'E');
	printf("\t\t\t\t请输入剧目ID:");
	scanf("%d", &play_id);
	while (getchar() != '\n')
		continue;
	while (Play_Srv_FetchByID(play_id, &play) == 0)
	{
		printf("\t\t\t\t输入ID错误, 请重新输入：");
		scanf("%d", &play_id);
		while (getchar() != '\n')
			continue;
	}
	paging.totalRecords = Schedule_Srv_FetchByPlay(scheduleList, play_id);
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	Paging_Locate_FirstPage(scheduleList, paging);
	do
	{
		system("cls");
		Schedule_UI_Show_List(scheduleList, paging);
		printf("\t\t\t\t[E]选择计划  [P]上一页  [N]下一页  [R]退出\n\t\t\t\t");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
		case 'P':
			if (!Pageing_IsFirstPage(paging))
				Paging_Locate_OffsetPage(scheduleList, paging, -1, schedule_node_t);
		case 'N':
			if (!Pageing_IsLastPage(paging))
				Paging_Locate_OffsetPage(scheduleList, paging, 1, schedule_node_t);
		case 'R':
			return 0;
		default:
			break;
		}

	} while (choice != 'E');
	printf("\t\t\t\t请输入演出计划ID:");
	scanf("%d", &schedule_id);
	while (getchar() != '\n')
		continue;
	while (Schedule_Srv_FetchByID(schedule_id, &schedule) == 0)
	{
		printf("\t\t\t\t输入ID错误, 请重新输入：");
		scanf("%d", &schedule_id);
		while (getchar() != '\n')
			continue;
	}
	Ticket_Srv_FetchBySchID(ticketList, schedule_id);
	Seat_Srv_FetchByRoomID(seatList, schedule.studio_id);
	Ticket_UI_ListBySch(&schedule, ticketList, seatList);
	List_Destroy(scheduleList, schedule_node_t);
	List_Destroy(seatList, seat_node_t);
	List_Destroy(playList, play_node_t);
	List_Destroy(ticketList, ticket_node_t);
	return 0;

}

//根据计划ID显示所有票
void Ticket_UI_ListBySch(const schedule_t *sch,	ticket_list_t tickList, seat_list_t seatList) 
{
	assert(NULL!=sch && NULL!=tickList && NULL!=seatList);
	int i, j;

	studio_t studioRec;
	play_t   playRec;
	ticket_node_t *pTicket;
	seat_node_t *pSeat;

	if (!Studio_Srv_FetchByID(sch->studio_id, &studioRec)) {  //获得对应id放映厅的信息
		printf("\t\t\t\t不存在\n");
		while(getchar() != '\n')continue;
		return;
	}

	if (!Play_Srv_FetchByID(sch->play_id, &playRec)) {  //获得对应id剧目的信息
		printf("\t\t\t\t不存在\n");
		while(getchar() != '\n')continue;
		return;
	}
	system("cls");
	printf("\t\t\t\t********************** Ticket List for %s ***********************\n", playRec.name);
	printf("\t\t\t\t%5c", ' ');
	for (i = 1; i <= studioRec.colsCount; i++) {
		printf("%3d", i);
	}

	printf("\n\t\t\t\t------------------------------------------------------------------\n");
	//显示数据
	for (i = 1; i <= studioRec.rowsCount; i++) 
	{
		j = 1;
		printf("\t\t\t\t%2d:%c", i, '|');
		List_ForEach(tickList, pTicket)
		{
			pSeat = Seat_Srv_FindByID(seatList, pTicket->data.seat_id);
			if (NULL != pSeat && pSeat->data.row == i) 
			{
				if (pSeat->data.column != j || pSeat->data.status != SEAT_GOOD) 
				{
					printf("%3c", ' ');
					j++;
				}
				printf("%3c", Ticket_UI_Status2Char(pTicket->data.status));
				j++;
			}
		}
		printf("\n");
	}
	printf("\t\t\t\t——————————————————————————————————\n");
	while (getchar() != '\n')
		continue;
}





void Ticket_UI_Query(void)
{
	Ticket_UI_ShowTicket(0);

}
 
void Ticket_UI_ShowList(ticket_list_t ticket, Pagination_t paging)
{
	ticket_list_t list;
	List_Init(list, ticket_node_t);
	paging.totalRecords = Ticket_Srv_FetchSold(ticket, list);
//	printf("%d", paging.totalRecords);
	Paging_Locate_FirstPage(list, paging);
	int i = 0;
	ticket_node_t* node;
	printf("\n\t\t\t\t==========================订票信息==============================\n");
	printf("\t\t\t\t%4s\t%12s\t%8s\t%4s\n", "ID", "演出计划ID", "座位ID", "价格");
	Paging_ViewPage_ForEach(list, paging, ticket_node_t, node, i)
	{
		if(node->data.status == TICKET_SOLD)
		printf("\t\t\t\t%4d\t%12d\t%8d\t%4d\n",
			node->data.id, node->data.schedule_id, node->data.seat_id, node->data.price);
	}
	printf("\t\t\t\t================================================================\n");
	List_Destroy(list, ticket_node_t);
	return;
}
