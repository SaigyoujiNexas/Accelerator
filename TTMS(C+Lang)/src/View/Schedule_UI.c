#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Persistence/Query_Persist.h"
#include "../Persistence/Ticket_Persist.h"
#include "../View/Studio_UI.h"
#include "../Common/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
static const int STUDIO_PAGE_SIZE = 5;
void Schedule_UI_ListAll(void) 
{
	int i, id;
	char choice;
	char name[30];

	schedule_list_t head;
	schedule_node_t *p;
	Pagination_t paging;

	List_Init(head, schedule_node_t);

	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		Schedule_UI_Show_List(head, paging);
		printf("\t\t\t\t******************************************************************\n" );
		printf("\t\t\t\t	[P]上页		[N]下页		[E]根据剧目名称查询		[R]返回\n");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t请选择:");
			choice=toupper(getchar());
			while (getchar() != '\n')
				continue;
			switch (choice) {
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
			case 'E':
				printf("\t\t\t\t请输入剧名:");
				s_gets(name, 30);
				Schedule_UI_Query(name);
				break;
			}
		} while (choice != 'r' && choice != 'R');
		//释放链表空间
		List_Destroy(head, schedule_node_t);
}
int Schedule_UI_Add(int play_id)
{
	int id;
	char temp[30];
	int studio_id;
	Pagination_t paging_studio;
	studio_list_t head_studio;
	List_Init(head_studio, studio_node_t);

	paging_studio.totalRecords = Studio_Srv_FetchAll(head_studio);
	paging_studio.pageSize = STUDIO_PAGE_SIZE;
	Paging_Locate_FirstPage(head_studio, paging_studio);
	
	schedule_t schedule;
	play_t play;
	studio_t studio;
	Play_Srv_FetchByID(play_id, &play);

	int newscheduleCount = 0;
	char choice;
	do {
		do
		{
			system("cls");
			Studio_UI_Show_Studio(head_studio, paging_studio);
			printf("\n\t\t\t\t==================================================================\n");
			printf("\t\t\t\t*********添加演出计划*******\n");
			printf("\t\t\t\t------------------------------------------------------------------\n");
			printf("\t\t\t\t[P]查看上一页演出厅	[N]查看下一页演出厅	[A]添加播放计划\n");
			choice = toupper(getchar());
			while (getchar() != '\n')
				continue;
			switch (choice)
			{
			case 'P':
				if (!Pageing_IsFirstPage(paging_studio))
					Paging_Locate_OffsetPage(head_studio, paging_studio, -1, studio_node_t);
				break;
			case 'N':
				if (!Pageing_IsLastPage(paging_studio))
					Paging_Locate_OffsetPage(head_studio, paging_studio, 1, studio_node_t);
				break;
			default:
				break;
			}
		} while (choice != 'A');

		schedule.id = EntKey_Srv_CompNewKey(SCHEDULE_KEY_NAME);
		schedule.play_id = play_id;
		printf("\t\t\t\t演出厅ID:");
		scanf("%d", &studio_id);
		while (!Studio_Srv_FetchByID(studio_id, &studio))
		{
			printf("\t\t\t\t演出厅ID不存在！请重新输入：");
			scanf("%d", &studio_id);
		}
		schedule.studio_id = studio.id;
		printf("\t\t\t\t放映日期:");
		scanf("%d %d %d", &(schedule.date.year), &(schedule.date.month), &(schedule.date.day));
		while (getchar() != '\n')
			continue;
		while (!date_Check(schedule.date))
		{
			printf("\t\t\t\t日期规格不正确！请重新输入：");
			scanf("%d %d %d", &(schedule.date.year), &(schedule.date.month), &(schedule.date.day));
			while (getchar() != '\n')
				continue;
		}
		printf("\t\t\t\t放映时间:");
		scanf("%d %d", &(schedule.time.hour), &(schedule.time.minute));
		while (getchar() != '\n')
			continue;
		schedule.time.second = 0;
		while (!time_Check(schedule.time))
		{
			printf("\t\t\t\t时间规格不正确！请重新输入：");
			scanf("%d %d", &(schedule.time.hour), &(schedule.time.minute));
			while (getchar() != '\n')
				continue;
		}
		schedule.seat_count = studio.seatsCount;
		printf("\n\t\t\t\t==================================================================\n");
		if (Schedule_Srv_Add(&schedule))
		{
			newscheduleCount += 1;
			Ticket_Srv_GenBatch(schedule.id);
			printf("\t\t\t\t添加成功！\n");
		}
		else
			printf("\t\t\t\t添加失败!\n");
		printf("\t\t\t\t****************************************************************\n");
		printf("\t\t\t\t[A]继续添加     [R]返回");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
	} while ('a' == choice || 'A' == choice);
	List_Destroy(head_studio, studio_node_t);
	return newscheduleCount;
}
int Schedule_UI_Modify(int id)
{
	
	int ret=0,sid;
       	char choice;
	schedule_t data;
	studio_t sdata;	
	
	if(Schedule_Perst_SelectByID(id, &data))
	{
		while(1){ 

				system("cls");
				printf("\n\t\t\t\t==================================================================\n");
				printf("\t\t\t\t********修改演出计划***********\n");
       			printf("\n\t\t\t\t==================================================================\n");
       			printf("\t\t\t\t%5s %5s %8s %10s %10s %10s \n", "ID", "剧目ID", "演出厅ID","放映日期","放映时间","座位数");
				printf("\t\t\t\t------------------------------------------------------------------\n");
				printf("\t\t\t\t%5d %5d %5d ", data.id, data.play_id,data.studio_id);
				printf("%4d-%02d-%02d ",data.date.year, data.date.month, data.date.day);
				printf("%2d:%02d ", data.time.hour, data.time.minute);
				printf("%5d\n", data.seat_count);
				printf("\t\t\t\t==================================================================\n");
				printf("\t\t\t\t[A]修改演出厅        [B]修改放映时间          [R]返回\n");
				printf("\n\t\t\t\t==================================================================\n");
				printf("\t\t\t\t请选择:");
				choice = toupper(getchar());
				if('r'==choice || 'R'==choice) break;	
				switch(choice){
					case 'a':
					case 'A':
				
						printf("\t\t\t\t请输入演播厅的ID：");
				
						while(1){
							if(scanf("%d",&sid)==1){
								while (getchar() != '\n')
									continue;
							 	if(!Studio_Srv_FetchByID(sid, &sdata))
								{
							 		printf("\t\t\t\tID为%d的演出厅不存在，请重新输入：",sid);
							 		continue;
							 	}
							data.studio_id=sdata.id;
							data.seat_count=Seat_Number_Count(data.studio_id);
							 
							 break;
							}
							else{
								printf("\t\t\t\t您的输入有误！请重新输入:"); 
								}
						}
						break;
					case 'b':
					case 'B':
						printf("\t\t\t\t请输入放映日期:");
						scanf("%d%d%d",&(data.date.year), &(data.date.month), &(data.date.day));
						printf("\t\t\t\t请输入放映时间:");
						scanf("%d%d",&(data.time.hour), &(data.time.minute));
						while (getchar() != '\n')
							continue;
						break;
				}

				if(Schedule_Srv_Modify(&data)){
					
					printf("\t\t\t修改成功！\n");
					ret=1;
				}else{
					printf("\t\t\t修改失败！\n");
				}
				
				printf("\t\t\t按任意键继续修改。");
		
				while(getchar() != '\n')continue;
		
		}//while
	}else{
		printf("\t\t\t\t未找到ID为%d的演出计划！\n",id);
		printf("\t\t\t\t按任意键返回！\n");
		while(getchar() != '\n')continue;
		return 0;
	}
       return ret;
}
int Schedule_UI_Delete(int id) {

	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		if (Ticket_Perst_Delete(id))
			printf("\t\t\t\t座位删除成功\n");
		printf("\t\t\t\t演出删除成功!\n\t\t\t\t按回车键返回\n");
		rtn = 1;
	}
	else {
		printf("\t\t\t\t放映室不存在\n\t\t\t\t按回车键返回\n");
	}

	while (getchar() != '\n')
		continue;
	return rtn;

}
int Schedule_UI_Query(char * play_name)
{
	schedule_t rec;
	play_list_t list_p;

	List_Init(list_p, play_node_t);

	schedule_list_t list_s;
	List_Init(list_s, schedule_node_t);
	play_node_t* node;
	/*int rtn = 0,seatcount;*/
	char choice;
	Pagination_t paging;
	paging.totalRecords = 0;

	/*Load record*/
	system("cls");
	if(Play_Srv_FetchByName(list_p, play_name) == 0)
	{
		printf("\t\t\t\t剧目不存在\n\t\t\t\t按任意键返回\n");
		while (getchar() != '\n')
			continue;
		return 0;
	}
	List_ForEach(list_p, node)
	{
		paging.totalRecords +=Schedule_Srv_FetchByPlay(list_s, node->data.id);
	}
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	Paging_Locate_FirstPage(list_s, paging);
	do
	{
		Schedule_UI_Show_List(list_s, paging);
		printf("\n\t\t\t\t******************************************************************\n");
		printf("\t\t\t\t[P]上一页  |  [N]下一页  |  [R]返回");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t按字母选择功能:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
	} while (choice != 'R');
	List_Destroy(list_p, play_node_t);
	List_Destroy(list_s, schedule_node_t);

	return 0;
}


void Schedule_UI_Show_List(schedule_list_t list, Pagination_t paging)
{
	int i = 0;
	schedule_node_t* p;
	play_t play;
	studio_t studio;
	Play_Srv_FetchByID(list->next->data.play_id, &play);
	printf("\n\t\t\t\t==================================================================\n");
	printf("\t\t\t\t\t\t\t*********演出计划*********\n");
	printf("\t\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t\t%4s\t%10s\t%8s\t%10s\t%5s \t%5s\n", "ID", "剧目", "演出厅", "放映日期", "放映时间", "座位数");
	//显示数据
	Paging_ViewPage_ForEach(list, paging, schedule_node_t, p, i) 
	{
		Play_Srv_FetchByID(p->data.play_id, &play);
		Studio_Srv_FetchByID(p->data.studio_id, &studio);
		printf("\t\t\t\t%4d\t %10s\t %8s\t", p->data.id, play.name, studio.name);
		printf("%4d-%02d-%02d \t", p->data.date.year, p->data.date.month, p->data.date.day);
		printf("%2d:%02d \t", p->data.time.hour, p->data.time.minute);
		printf("\t%5d\n", p->data.seat_count);
	}
	printf("\n\t\t\t\t-------- 共 %2d 项 --------------------------- %2d/%2d 页 --------\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));
}


void Schedule_UI_MgtEntry(int play_id)
{

	int i, ID;
	char choice;

	schedule_list_t head;
	schedule_node_t* pos;
	Pagination_t paging;
	play_t play;

	while (Play_Srv_FetchByID(play_id, &play) == 0)
	{
		printf("\t\t\t\t剧目ID输入错误， 请重新输入:");
		scanf("%d", &play_id);
		while (getchar() != '\n')
			continue;
	}

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
	Paging_Locate_FirstPage(head, paging);

	do
	{
		system("cls");
		Schedule_UI_Show_List(head, paging);
		printf("\n\t\t\t\t******************************************************************\n");
		printf("\t\t\t\t[P]上一页  |  [N]下一页  |  [A]添加  |  [D]删除  |  [U]更新  |  [R]返回");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t按字母选择功能:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
		case 'A':
			if (Schedule_UI_Add(play_id)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'D':
			printf("\t\t\t\t请输入要删除的演出计划ID:");
			scanf("%d", &ID);
			while (getchar() != '\n')
				continue;
			if (Schedule_UI_Delete(ID))
			{	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'U':
			printf("\t\t\t\t请输入要修改的演出计划ID:");
			scanf("%d", &ID);
			while (getchar() != '\n')
				continue;
			if (Schedule_UI_Modify(ID))
			{	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'P':
			if (!Pageing_IsFirstPage(paging))
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			break;
		case 'N':
			if (!Pageing_IsLastPage(paging))
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, schedule_node_t);
}