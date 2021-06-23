/*
* Copyright(C), 2007-2008, XUPT Univ.	
* ������ţ�TTMS_UC_01	 
* File name: Studio_UI.c			  
* Description : �ݳ������������	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
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
��ʶ����TTMS_SCU_Studio_UI_MgtEnt 
�������ܣ����������ݳ�������ں�������ʾ��ǰ���ݳ������ݣ����ṩ�ݳ���������ӡ��޸ļ�ɾ�����ܲ�������ڡ�
����˵�����ޡ�
�� �� ֵ���ޡ�
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

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		Studio_UI_Show_Studio(head, paging);
		printf(
				"\t\t\t\t[P]��һҳ | [N]��һҳ | [A]��� | [D]ɾ�� | [U]���� | [S]��λ | [R]����");
		printf(
				"\n\t\t\t\t===================================================================\n");
		printf("\t\t\t\t���ѡ��");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t\t����ID:");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			if (Studio_UI_Delete(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t\t����ID:");
			scanf("%d", &id);
			while (getchar() != '\n')
				continue;
			if (Studio_UI_Modify(id)) 
			{	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\t\t\t\t����ID:");
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
	//�ͷ�����ռ�
	List_Destroy(head, studio_node_t);
}

/*
��ʶ����TTMS_SCU_Studio_UI_Add 
�������ܣ�������ϵͳ�����һ�����ݳ������ݡ�
����˵�����ޡ�
�� �� ֵ�����ͣ��ɹ�������ݳ����ĸ�����
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
		printf("\t\t\t\t********************  ����µķ�ӳ��  *******************\n");
		printf("\t\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t\t��ӳ����:");
		fflush(stdin);
		s_gets(rec.name, 30);
		printf("\t\t\t\t��λ����:");
		scanf("%d", &(rec.rowsCount));
		printf("\t\t\t\t��λ����:");
		scanf("%d", &(rec.colsCount));
		while (getchar() != '\n')
			continue;
		rec.seatsCount = rec.rowsCount * rec.colsCount;
		printf("\t\t\t\t=======================================================\n");

		if (Studio_Srv_Add(&rec)) {
			Seat_Srv_RoomInit(list, rec.id, rec.rowsCount, rec.colsCount);
			newRecCount += 1;
			List_Destroy(list, seat_node_t);
			printf("\t\t\t\t�·�ӳ����ӳɹ�!\n");
		} else
			printf("\t\t\t\t�·�ӳ�����ʧ��!\n");
		printf("\t\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t\t[A]��Ӹ���, [R]����:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
��ʶ����TTMS_SCU_Studio_UI_Mod
�������ܣ������޸�ϵͳ���ִ��һ���ݳ������ݡ�
����˵����idΪ���ͣ�����Ҫ�޸ĵ��ݳ���ID��
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��޸����ݳ����ı�־��
*/
int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t\t��ӳ��������!\n��[Enter]������!\n");
		while(getchar() != '\n')continue;
		return 0;
	}

	printf("\n\t\t\t\t=======================================================\n");
	printf("\t\t\t\t*********************  ���·�ӳ��  **********************\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t��ӳ�� ID:%d\n", rec.id);
	printf("��ӳ������[%s]:", rec.name);
	fflush(stdin);
	s_gets(rec.name, 30);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
		do {			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷����������
			printf("��λ����Ӧ�� >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("��λ����Ӧ�� >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
			while (getchar() != '\n')
				continue;
		} while (newrow < rec.rowsCount || newcolumn < rec.colsCount);
		rec.rowsCount = newrow;
		rec.colsCount = newcolumn;
		rec.seatsCount = seatcount;
	} else {
		printf("��λ����:");
		scanf("%d", &rec.rowsCount);
		printf("��λ����:");
		scanf("%d", &rec.colsCount);
		while (getchar() != '\n')
			continue;
		rec.seatsCount =0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"��ӳ�����ݸ��³ɹ�!\n��[Enter]������!\n");
	} else
		printf("��ӳ�����ݸ���ʧ��!\n��[Enter]������!\n");
	while (getchar() != '\n')
		continue;
	return rtn;
}

/*
��ʶ����TTMS_SCU_Studio_UI_Del
�������ܣ�����ɾ��ϵͳ���ִ��һ���ݳ������ݡ�
����˵����idΪ���ͣ�����Ҫɾ�����ݳ���ID��
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ�����ݳ����ı�־��
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
			printf("\t\t\t\t������ص��ݳ��ƻ��� �Ƿ����ɾ����[Y]���� [N]ȡ��:");
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
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t\t��ӳ������λ����ɾ���ɹ�!\n");
	//	system("cls");
		printf(
				"\t\t\t\t��ӳ��ɾ���ɹ�!\n\t\t\t\t��[Enter]������!\n");
		rtn = 1;
	} else {
		printf("\t\t\t\t��ӳ��������!\n\t\t\t\t��[Enter]������!\n");
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
		"\t\t\t\t*************************** ��ӳ���б� ***************************\n");
	printf("\t\t\t\t%5s  %18s  %10s  %10s  %10s\n", "ID", "��ӳ������", "����", "����", "������λ��");
	printf(
		"\t\t\t\t--------------------------------------------------------------------\n");
	//��ʾ����
	Paging_ViewPage_ForEach(head, paging, studio_node_t, pos, i) {
		printf("\t\t\t\t%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
			pos->data.name, pos->data.rowsCount, pos->data.colsCount,
			pos->data.seatsCount);
	}

	printf(
		"\t\t\t\t----------- ��ӳ������:%2d ------------------------ ҳ�� %2d/%2d ------\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));
	printf(
		"\t\t\t\t*******************************************************************\n");
}

