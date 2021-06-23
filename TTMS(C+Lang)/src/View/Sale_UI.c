#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"Sale_UI.h"
#include "../View/Studio_UI.h"
#include "../Service/Play.h"
#include "../Common/List.h"
#include "../Common/Common.h"
#include "../Service/Studio.h"
#include "../Persistence/Sale_Persist.h"
#include "../Service/Sale.h"
#include "../Service/Schedule.h"
#include "../Service/Account.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../View/Schedule_UI.h"
#include "Ticket_UI.h"
#include "../View/Play_UI.h"

extern const account_t* User;

//管理售票界面
void Sale_UI_MgtEntry(void)
{ 
	int i, id;
	char choice;
	char name[30];
	play_list_t head;
	play_node_t* p;

	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;


	List_Init(head, play_node_t);
	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do 
	{
		system("cls");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t--------------------------剧目列表----------------------------------\n");
		printf("\t\t\t\t%2s  %-10s\t%-8s  %-5s %-10s \t %-10s \t %-5s \n", "ID", "名称", "地区",
			"时长", "上映时间", "下线时间", "票价");
		printf("\t\t\t\t------------------------------------------------------------------\n");


		//显示数据
		Paging_ViewPage_ForEach(head, paging, play_node_t, p, i) {
			printf("\t\t\t\t%2d  %-10s %-10s %-5d ", p->data.id, p->data.name, p->data.area, p->data.duration);
			printf("%4d-%02d-%02d ", p->data.start_date.year, p->data.start_date.month, p->data.start_date.day);
			printf("\t%4d-%02d-%02d ", p->data.end_date.year, p->data.end_date.month, p->data.end_date.day);
			printf("\t  %-5d\n", p->data.price);
		}

		printf("\n\t\t\t\t----------- 共 %2d 项 ------------------- %2d/%2d 页  ----------------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t\t\t ******************************************************************\n");
		printf("\n\t\t\t\t[C]显示演出计划[S]查询剧目名称[F]筛去剧目[P]上一页[N]下一页[R]退出系统\n");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t请选择:");
		fflush(stdin);
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;


		switch (choice)
		{
		case 'c':
		case 'C':
			printf("\t\t\t\t请输入剧目ID：");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			Sale_UI_ShowScheduler(id);
			break;
		case'S':
			printf("\t\t\t\t请输入要查找的剧目名[Enter：取消查找]");
			s_gets(name, 30);
			if (name[0] == '\0')
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_FirstPage(head, paging);
			}
			else
			{
				paging.totalRecords = Play_Srv_FetchByName(head, name);
				Paging_Locate_FirstPage(head, paging);
			}
			break;
		case'F':
			printf("\t\t\t\t请输入要筛选的剧目名[Enter：取消筛选]");
			s_gets(name, 30);
			if (name[0] == '\0')
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_FirstPage(head, paging);
			}
			else
			{
				paging.totalRecords -= Play_Srv_FilterByName(head, name);
				Paging_Locate_FirstPage(head, paging);
			}
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, play_node_t);
}
//根据剧目显示演出计划界面
void Sale_UI_ShowScheduler(int playID)
{
	int id;
	char choice;
	schedule_list_t head;
	Pagination_t paging;
	schedule_node_t* pos;
	studio_t studio;
	play_t play;
	int i = 0;
	play_t pdata;
	while (!Play_Srv_FetchByID(playID, &pdata))
	{
		printf("\t\t\t\t输入错误或该剧目ID不存在！请重新输入：");
		scanf("%d", &playID);
		while (getchar() != '\n')
			continue;
	}
	//初始化schedule数据
	List_Init(head, schedule_node_t);
	paging.totalRecords = Schedule_Srv_FetchByPlay(head, playID);
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\t\t\t\t==============================================================================\n");
		printf("\t\t\t\t--------------------------<%10s> 演出计划表-----------------------------------\n",pdata.name);
		printf("\t\t\t\t\t************%6s******%10s****%10s******%10s******%10s******%10s*\n", "演出计划ID", "剧目名称", "演出厅","放映日期","放映时间","座位数");
		printf("\t\t\t\t------------------------------------------------------------------------------\n");
		
		if(!List_IsEmpty(head)){
				Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i)
				{
					Play_Srv_FetchByID(pos->data.play_id, &play);
					Studio_Srv_FetchByID(pos->data.studio_id, &studio);
					printf("\t\t\t\t------------%6d \t  %10s   %10s ", pos->data.id, play.name,studio.name);
					printf("\t----%-4d-%02d-%02d ",pos->data.date.year, pos->data.date.month, pos->data.date.day);
					printf("\t  ----%2d:%02d ", pos->data.time.hour, pos->data.time.minute);
					printf("  ----%5d\n", pos->data.seat_count);
			}
		}else{
		
			printf("\t\t\t\t未搜索到相关剧目\n");
		}

		printf("\n\t\t\t\t--------------------------%2d 项---------------------%2d/%2d 页---------------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("\t\t\t\t********************************************************************************\n");
		printf("\t\t\t\t**************[S]进入售票系统[P]上一页[N]下一页[R]返回上一界面******************\n");
		printf("\t\t\t\t请输入字符来进行操作:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice) {
		case 's':
		case 'S':
			printf("\t\t\t\t请输入演出计划ID: ");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			Sale_UI_ShowTicket(id, playID);

				break;

		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	List_Destroy(head, schedule_node_t);
}
//售票界面
int Sale_UI_SellTicket(ticket_list_t tickList,seat_list_t seatList)
{
	schedule_t sch;
	int row, col;
	seat_node_t* seat;
	ticket_node_t* ticket;
	sale_t sale;
	seat_t tseat;
	ticket_t tticket;
	Schedule_Srv_FetchByID(tickList->next->data.schedule_id, &sch);	//获取安排信息
	Ticket_UI_ListBySch(&sch,tickList,seatList);
	printf("\t\t\t\t请输入要购买票的行数:");
	scanf("%d", &row);
	while (getchar() != '\n')
		continue;
	printf("\t\t\t\t请输入列数: ");
	scanf("%d", &col);
	while (getchar() != '\n')
		continue;
	if ((seat = Seat_Srv_FindByRowCol(seatList, row, col)) == NULL || seat->data.status != SEAT_GOOD)
	{
		printf("\t\t\t\t输入的行数或列数不正确！[按任意键继续]");
		while (getchar() != '\n')
			continue;
		return 0;
	}
	if (seat->data.status == SEAT_NONE)
	{
		printf("\t\t\t\t当前座位不可用！[按任意键继续]");
		while (getchar() != '\n')
			continue;
		return 0;
	}
	if (seat->data.status == SEAT_BROKEN)
	{
		printf("\t\t\t\t当前座位已损坏！[按任意键继续]");
		while (getchar() != '\n')
			continue;
		return 0;
	}
	if ((ticket = Ticket_Srv_FetchBySeatID(tickList, seat->data.id)) == NULL)
	{
		printf("\t\t\t\t获取票单失败！[按任意键继续]");
		while (getchar() != '\n')
			continue;
		return 0;
	}
	if (TICKET_SOLD == ticket->data.status)
	{
		printf("\t\t\t\t当前票单已被售出！[按任意键退出]");
		while (getchar() != '\n')
			continue;
		return 0;
	}

	sale.date = DateNow();			//设置销售数据
	sale.time = TimeNow();
	sale.user_id = User->id;
	sale.id = EntKey_Srv_CompNewKey("Sale");
	sale.ticket_id = ticket->data.id;
	sale.value = ticket->data.price;
	sale.type = SALE_SELL;
	while (getchar() != '\n')
		continue;

	ticket->data.status = TICKET_SOLD;

	tticket = ticket->data;

	Ticket_Srv_Modify(&tticket);

	Sale_Srv_Add(&sale);
	printf("\t\t\t\t操作成功!");
	Ticket_UI_ShowTicket(ticket->data.id);
	while (getchar() != '\n')
		continue;
    return 1;
}

 //退票界面
 void Sale_UI_ReturnTicket()
 {
 	char choice;
	int playID;
	int scheduleID;
	int id,t=0;
	//创建play链表
	play_list_t play;
	play_t tplay;
	List_Init(play, play_node_t);
	schedule_list_t schedule;
	schedule_t tschedule;
	List_Init(schedule, schedule_node_t);
	ticket_list_t tickets;
	ticket_t ticket;
	List_Init(tickets, ticket_node_t);

	//分页器设置
	Pagination_t paging;
	paging.pageSize = PLAY_PAGE_SIZE;
	paging.totalRecords = Play_Srv_FetchAll(play);
	Paging_Locate_FirstPage(play, paging);
	system("cls");
	Play_UI_ShowList(play, paging);
	printf("\t\t\t\t请选择演出ID：");
	scanf("%d", &playID);
	while (getchar() != '\n')
		continue;
	while (Play_Srv_FetchByID(playID, &tplay) == 0)
	{
		printf("\t\t\t\t演出ID输入错误， 请重新输入：");
		scanf("%d", &playID);
		while (getchar() != '\n')
			continue;
	}

	//重置分页器
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	paging.totalRecords = Schedule_Srv_FetchByPlay(schedule, playID);
	Paging_Locate_FirstPage(schedule, paging);
	system("cls");
	do
	{
		Schedule_UI_Show_List(schedule, paging);
		printf("\t\t\t\t******************************************************************\n");
		printf("\t\t\t\t		[P]上页			[N]下页			[R]继续\n");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
		case 'P':
			if (!Pageing_IsFirstPage(paging))
				Paging_Locate_OffsetPage(schedule, paging, -1, schedule_node_t);
			break;

		case 'N':
			if (!Pageing_IsLastPage(paging))
				Paging_Locate_OffsetPage(schedule, paging, 1, schedule_node_t);
			break;
		default:
			break;
		}
	}while (choice != 'R');
	printf("\t\t\t\t请选择演出计划ID：");
	scanf("%d", &scheduleID);
	while (getchar() != '\n')
		continue;
	while (Schedule_Srv_FetchByID(scheduleID, &tschedule) == 0)
	{
		printf("\t\t\t\t演出计划输入错误， 请重新输入：");
		scanf("%d", &scheduleID);
		while (getchar() != '\n')
			continue;
	}

	paging.pageSize = TICKET_PAGE_SIZE;
	paging.totalRecords = Ticket_Srv_FetchBySchID(tickets, scheduleID);
	Paging_Locate_FirstPage(tickets, paging);
	system("cls");
	printf("\t\t\t\t=================================================================================\n");
	printf("\t\t\t\t====================================退   票======================================\n");
	printf("\t\t\t\t=================================================================================\n");
	Ticket_UI_ShowList(tickets, paging);
		//进入主页面
       	printf("\n\t\t\t\t请输入票的ID:\n");
		scanf("%d",&id);
		while (getchar() != '\n')
			continue;
		while (Ticket_Srv_FetchByID(id, &ticket) == 0 || ticket.status == TICKET_AVL)
		{
			printf("\t\t\t\tID输入错误， 请重新输入：");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
		}
			do
			{
				printf("\t\t\t\t=================================================================================\n");
				printf("\t\t\t\t----------------------------[Q]退票[R]返回上一界面---------------------------\n");
				printf("\t\t\t\t=================================================================================\n");
				choice = toupper(getchar());
				while (getchar() != '\n')
					continue;
			} while (choice != 'Q' && choice != 'R');
			if ('r' == choice || 'R' == choice) return;
			if('q'==choice || 'Q'==choice) 
			{
			printf("\n\n\n\n");
	       	ticket.status=TICKET_AVL;
	       	sale_t data;
			data.id = EntKey_Srv_CompNewKey("Sale");
	       	data.user_id=User->id;
	       	data.ticket_id=id;
	       	data.date=DateNow();
	       	data.time=TimeNow();
	       	data.value=ticket.price;
	       	data.type=SALE_REFOUND;
	       					
	       			Sale_Srv_Add(&data);
	       					
	       			Ticket_Srv_Modify(&ticket);
					system("cls");
       				printf("\n\n\n\n\t\t\t\t退票成功!\n\n\n\n");
					while (getchar() != '\n')
						continue;
       			}
					List_Destroy(tickets, ticket_node_t);
					List_Destroy(schedule, schedule_node_t);
					List_Destroy(play, play_node_t);
}
		
 void Sale_UI_ShowTicket(int scheduleID, int playID)
 {
	 char choice;
	 schedule_t sch;
	 play_t play;
	 seat_list_t seats;
	 ticket_list_t tickets;
	 List_Init(tickets, ticket_node_t);
	 List_Init(seats, seat_node_t);
	 while (!Schedule_Srv_FetchByID(scheduleID, &sch))
	 {
		 printf("\t\t\t\t演出计划ID输入不正确， 请重新输入：");
		 scanf("%d", &scheduleID);
		 while (getchar() != '\n')
			 continue;
	 }
	 Seat_Srv_FetchByRoomID(seats, sch.studio_id);
	 Ticket_Srv_FetchBySchID(tickets, scheduleID);
	 do
	 {
		 printf("\t\t\t\t[R]返回上一页\t\t[B]购票:");
		 choice = toupper(getchar());
		 while (getchar() != '\n')
			 continue;
		 if (choice == 'B')
			 Sale_UI_SellTicket(tickets, seats);
	 } while (choice != 'R');
	 List_Destroy(tickets, ticket_node_t);
	 List_Destroy(seats, seat_node_t);
	 return;

 }
