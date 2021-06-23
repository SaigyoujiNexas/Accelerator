/*
* Copyright(C), 2007-2008, XUPT Univ.	
* 用例编号：TTMS_UC_01	 
* File name: Studio_UI.c			  
* Description : 演出厅用例界面层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/

#include "../View/Studio_UI.h"

#include "../Common/List.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "Seat_UI.h"
#include "../Common/common.h"


static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

/*
标识符：TTMS_SCU_Studio_UI_MgtEnt 
函数功能：界面层管理演出厅的入口函数，显示当前的演出厅数据，并提供演出厅数据添加、修改及删除功能操作的入口。
参数说明：无。
返 回 值：无。
*/
void Studio_UI_MgtEntry(void) 
{
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		Studio_UI_Show_Studio(head, paging);
		printf(
				"\t\t\t\t[P]上一页 | [N]下一页 | [A]添加 | [D]删除 | [U]更新 | [S]座位 | [R]返回");
		printf(
				"\n\t\t\t\t===================================================================\n");
		printf("\t\t\t\t你的选择：");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t\t输入ID:");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			if (Studio_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t\t输入ID:");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			if (Studio_UI_Modify(id)) 
			{	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\t\t\t\t输入ID:");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t)
			;
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

/*
标识符：TTMS_SCU_Studio_UI_Add 
函数功能：用于向系统中添加一个新演出厅数据。
参数说明：无。
返 回 值：整型，成功添加新演出厅的个数。
*/
int Studio_UI_Add(void) {
	studio_t rec;
	seat_list_t list;
	List_Init(list, seat_node_t);
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n\t\t\t\t=======================================================\n");
		printf("\t\t\t\t********************  添加新的放映室  *******************\n");
		printf("\t\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t\t放映厅名:");
		fflush(stdin);
		s_gets(rec.name, 30);
		printf("\t\t\t\t座位行数:");
		scanf("%d", &(rec.rowsCount));
		printf("\t\t\t\t座位列数:");
		scanf("%d", &(rec.colsCount));
		while (getchar() != '\n')
			continue;
		rec.seatsCount = rec.rowsCount * rec.colsCount;
		printf("\t\t\t\t=======================================================\n");

		if (Studio_Srv_Add(&rec)) {
			Seat_Srv_RoomInit(list, rec.id, rec.rowsCount, rec.colsCount);
			newRecCount += 1;
			List_Destroy(list, seat_node_t);
			printf("\t\t\t\t新放映厅添加成功!\n");
		} else
			printf("\t\t\t\t新放映厅添加失败!\n");
		printf("\t\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t\t[A]添加更多, [R]返回:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
标识符：TTMS_SCU_Studio_UI_Mod
函数功能：用于修改系统中现存的一个演出厅数据。
参数说明：id为整型，是需要修改的演出厅ID。
返 回 值：整型，表示是否成功修改了演出厅的标志。
*/
int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t\t放映厅不存在!\n按[Enter]键返回!\n");
		while(getchar() != '\n')continue;
		return 0;
	}

	printf("\n\t\t\t\t=======================================================\n");
	printf("\t\t\t\t*********************  更新放映厅  **********************\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t放映厅 ID:%d\n", rec.id);
	printf("放映厅名字[%s]:", rec.name);
	fflush(stdin);
	s_gets(rec.name, 30);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
		do {			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("座位行数应该 >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("座位列数应该 >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
			while (getchar() != '\n')
				continue;
		} while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
		rec.rowsCount = newrow;
		rec.colsCount = newcolumn;
		rec.seatsCount = seatcount;
	} else {
		printf("座位行数:");
		scanf("%d", &rec.rowsCount);
		printf("座位列数:");
		scanf("%d", &rec.colsCount);
		while (getchar() != '\n')
			continue;
		rec.seatsCount =0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"放映厅数据更新成功!\n按[Enter]键返回!\n");
	} else
		printf("放映厅数据更新失败!\n按[Enter]键返回!\n");
	while (getchar() != '\n')
		continue;
	return rtn;
}

/*
标识符：TTMS_SCU_Studio_UI_Del
函数功能：用于删除系统中现存的一个演出厅数据。
参数说明：id为整型，是需要删除的演出厅ID。
返 回 值：整型，表示是否成功删除了演出厅的标志。
*/
int Studio_UI_Delete(int id) 
{
	schedule_list_t schedule;
	char choice;
	schedule_node_t *pos;
	List_Init(schedule, schedule_node_t);
	if (Schedule_Srv_FetchByStudio(schedule, id) != 0)
	{
		do
		{
			system("cls");
			printf("\t\t\t\t存在相关的演出计划！ 是否继续删除？[Y]继续 [N]取消:");
			choice = toupper(getchar());
			while (getchar() != '\n')
				continue;
			if (choice == 'N')
				return 0;
		} while (choice != 'Y');
		List_ForEach(schedule, pos)
		{
			Schedule_UI_Delete(pos->data.id);
		}
	}
	List_Destroy(schedule, schedule_node_t);

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t\t放映厅的座位数据删除成功!\n");
	//	system("cls");
		printf(
				"\t\t\t\t放映厅删除成功!\n\t\t\t\t按[Enter]键返回!\n");
		rtn = 1;
	} else {
		printf("\t\t\t\t放映厅不存在!\n\t\t\t\t按[Enter]键返回!\n");
	}
	while (getchar() != '\n')
		continue;
	return rtn;
}

void Studio_UI_Show_Studio(studio_list_t head, Pagination_t paging)
{
	studio_node_t *pos;
	int i = 0;
	printf(
		"\n\t\t\t\t==================================================================\n");
	printf(
		"\t\t\t\t*************************** 放映室列表 ***************************\n");
	printf("\t\t\t\t%5s  %18s  %10s  %10s  %10s\n", "ID", "放映厅名字", "行数", "列数", "可用座位数");
	printf(
		"\t\t\t\t--------------------------------------------------------------------\n");
	//显示数据
	Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i) {
		printf("\t\t\t\t%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
			pos->data.name, pos->data.rowsCount, pos->data.colsCount,
			pos->data.seatsCount);
	}

	printf(
		"\t\t\t\t----------- 放映厅数量:%2d ------------------------ 页数 %2d/%2d ------\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));
	printf(
		"\t\t\t\t*******************************************************************\n");
}

