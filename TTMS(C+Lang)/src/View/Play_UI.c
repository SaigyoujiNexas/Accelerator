#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Play_UI_ShowList(play_list_t list,Pagination_t paging)//���溯�� 
{
	int i = 0,j;
	char choice;
	play_node_t *pos;

		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t\t\t\t*********��Ŀ��Ϣ**********\n");
        printf("\n\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t%4s\t %10s\t %10s\t %3s\t %3s\t %6s\t %10s\t %3s",
			"ID", "����", "��Ʒ����", "����", "�ȼ�", "ʱ��(����)", "��ӳʱ��", "Ʊ��");
		printf("\n\t\t\t\t------------------------------------------------------------------\n");
		Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i){
			printf("\t\t\t\t%4d\t ", pos->data.id);
			printf("%10s\t ",pos->data.name);
			printf("%10s\t ",pos->data.area);
			j=pos->data.type;
		switch(j){
			case 1:printf(" ��Ӱ\t ");break;
			case 2:printf(" ���\t ");break;
			case 3:printf("���ֻ�\t ");break;
		}
		j=pos->data.rating;
		switch(j){
			case 1:printf("��ͯ��\t ");break;
			case 2:printf("�����\t ");break;
			case 3:printf("���˾�\t ");break;
		}
		printf("%6d\t\t ",pos->data.duration);
		printf("%4d/%02d/%02d\t ",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);;
		printf("%3d\n",pos->data.price);
		}
		printf("\t\t\t\t------- �� %2d �� ----------------------- �� %2d/%2d ҳ ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));

}//�������� 

void Play_UI_MgtEntry()
{
	int id;
	char choice;
	play_list_t head;
	Pagination_t paging;
	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;
	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	do
	{
		system("cls");
		Play_UI_ShowList(head,paging);
        printf("\n\t\t\t\t******************************************************************\n" );
		printf("\t\t\t\t[S]�����ݳ��ƻ�    [A]���    [D]ɾ��    [U]�޸�    [Q]����    [R]����\n");
		printf("\t\t\t\t             [P]��һҳ                       [N]��һҳ\n"); 
        printf("\t\t\t\t==================================================================\n");
		printf("\t\t\t\t��ѡ��:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
			case 's':
			case 'S':
				printf("\t\t\t\t�������ĿID: ");
				scanf("%d", &id);
				while (getchar() != '\n')
					continue;
				Schedule_UI_MgtEntry(id);//�����ݳ��ƻ� 
				break;
			case 'a':
			case 'A':
				if (Play_UI_Add())
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'd':
			case 'D':
				printf("\t\t\t\t�������ĿID:");
				scanf("%d", &id);
				while (getchar() != '\n')
					continue;
				if (Play_UI_Delete(id))
				{	
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'u':
			case 'U':
				printf("\t\t\t\t�������ĿID:");
				scanf("%d", &id);
				if (Play_UI_Modify(id))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'q':
			case 'Q':
				printf("\t\t\t\t�������ĿID:");
				scanf("%d", &id);
				while (getchar() != '\n')
					continue;
				if (Play_UI_Query(id))
				{	//������������
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
				}
				break; 
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
				}
				break;
		}	
	}while(choice != 'r' && choice != 'R');
	List_Destroy(head, play_node_t);

} 
int Play_UI_Add(void)
{
	play_t data;
	char choice;
	int newRecCount = 0,type,rating;
	do{
	system("cls");
    printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t����µľ�Ŀ��Ϣ\n");
        printf("\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t�������Ŀ����:");
		s_gets(data.name, 30);
		printf("\t\t\t\t�������Ŀ����:");
		s_gets(data.area, 30);
		printf("\t\t\t\t�������Ŀʱ��:");
		scanf("%d",&data.duration);
		while (getchar() != '\n')
			continue;
		printf("\t\t\t\t�������Ŀ����:\n");
		printf("\t\t\t\t1.��Ӱ 2.��� 3.���ֻ�"); 
		scanf("%d",&type);
		while (getchar() != '\n')
			continue;
		if(type==1 || type==2 || type==3)
		{
			data.type=(play_type_t)type;
		}
		printf("\t\t\t\t��������Ӧ��Ⱥ:\n");
		printf("\t\t\t\t1.��ͯ 2.���� 3.����");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3)
		{
			data.rating=(play_rating_t)rating;
		}
		printf("\t\t\t\t�����뿪ʼ��ӳ����:");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		while (getchar() != '\n')
			continue;
		printf("\t\t\t\t�������ӳ��������:");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		while (getchar() != '\n')
			continue;
		printf("\t\t\t\t������Ʊ��:");
		scanf("%d",&data.price);
		while (getchar() != '\n')
			continue;
        printf("\n\t\t\t\t==================================================================\n");

		//��ȡ����
		data.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&data)) {
			newRecCount += 1;
			printf("\t\t\t\t�¾�Ŀ��ӳɹ�!\n");
		} else
			printf("\t\t\t\t�¾�Ŀ���ʧ��!\n");
        printf("\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t***************************[A]���*******[R]����******************\n");
		fflush(stdin);
		choice = toupper(getchar());

		while (getchar() != '\n')
			continue;
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}
 int Play_UI_Modify(int id)//�޸ľ�Ŀ 
{	int type,rating,i;
	play_t data;
	int rtn=0;
	char newname[31],newarea[9];
	int newduration,newprice,newSyear,newSmonth,newSday,newEyear,newEmonth,newEday,newrate,newtype;
	if (!Play_Srv_FetchByID(id, &data))
	{
		printf("\t\t\t\t��Ŀ������,��ȷ�ϼ�����\n");
		while(getchar() != '\n')continue;
		return 0;
	}
    printf("\n\t\t\t\t==================================================================\n");
	printf("\t\t\t\t�޸ľ�Ŀ��Ϣ\n");
    printf("------------------------------------------------------------------\n");
	printf("��ĿID      %d\n", data.id);
	printf("��Ŀ����    %s\n", data.name);
	printf("��Ŀ����    %s\n",data.area);
	i=data.type;
	switch(i)
	{
		case 1:printf("��Ŀ���� %s\n","��Ӱ");break;
		case 2:printf("��Ŀ���� %s\n","���");break;
		case 3:printf("��Ŀ���� %s\n","���ֻ�");break;
	}
	i=data.rating;
	switch(i)
	{
		case 1:printf("��Ŀ�ȼ� %s\n","��ͯ��");break;
		case 2:printf("��Ŀ�ȼ� %s\n","�����");break;
		case 3:printf("��Ŀ�ȼ� %s\n","���˾�");break;
	}
	printf("��Ŀʱ��      %d\n",data.duration);
	printf("��Ŀ��ӳ����  %d %d %d\n",data.start_date.year,data.start_date.month,data.start_date.day);
	printf("��ӳ��������  %d %d %d\n",data.end_date.year,data.end_date.month,data.end_date.day);
	printf("��ĿƱ�� %d\n",data.price);
    printf("------------------------------------------------------------------\n");
	fflush(stdin);
	if(Play_Srv_FetchByID(id,&data))//������Ҫ�޸ĵľ�Ŀ 
	{

		printf("�������Ŀ����:");
		s_gets(data.name, 30);
		printf("�������Ŀ����:");
		s_gets(data.name, 30);
		printf("�������Ŀʱ��:");
		scanf("%d",&newduration);
		while (getchar() != '\n')
			continue;
		data.duration=newduration;
		printf("�������Ŀ����\n:");
		printf("1.��Ӱ 2.��� 3.���ֻ�"); 
		scanf("%d",&newtype);
		while (getchar() != '\n')
			continue;
		if(newtype==1 || newtype==2 || newtype==3)
		{
			data.type=(play_type_t)newtype;
		}
		printf("��������Ӧ��Ⱥ:\n");
		printf("1.��ͯ 2.���� 3.����");
		scanf("%d",&newrate);
		while (getchar() != '\n')
			continue;
		if(newrate==1 || newrate==2 || newrate==3)
		{
			data.rating=(play_rating_t)newrate;
		}
		printf("�����뿪ʼ��ӳ����:");
		while (getchar() != '\n')
			continue;
		scanf("%d %d %d",&newSyear,&newSmonth,&newSday);
		data.start_date.year=newSyear;
		data.start_date.month=newSmonth;
		data.start_date.day=newSday;
		printf("�������ӳ��������:");
		while (getchar() != '\n')
			continue;
		scanf("%d %d %d",&newEyear,&newEmonth,&newEday);
		data.end_date.year=newEyear;
		data.end_date.month=newEmonth;
		data.end_date.day=newEday;
		printf("������Ʊ��:");
		while (getchar() != '\n')
			continue;
		scanf("%d",&newprice);
		data.price=newprice;
	}
    printf("------------------------------------------------------------------\n");

	if (Play_Srv_Modify(&data))
	{
		rtn = 1;
		printf("\t\t\t\t��Ŀ�޸ĳɹ�!\n\t\t\t\t��ȷ�ϼ�����!\n");
	}
	else
	{
		printf("\t\t\t\t��Ŀ�޸�ʧ��!\n\t\t\t\t��ȷ�ϼ�����!\n");
	}
	while(getchar() != '\n')continue;
	return rtn;
}
int Play_UI_Delete(int id)//ɾ����Ŀ 
{
	char choice;
	schedule_list_t schedule;
	schedule_node_t *pos;
	List_Init(schedule, schedule_node_t);
	int rtn = 0;

	if (Schedule_Srv_FetchByPlay(schedule, id) != 0)
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
	if (Play_Srv_DeleteByID(id))
	{
		printf("\t\t\t\t��Ŀ�ɹ�ɾ��!\n\t\t\t\t��ȷ�ϼ�����!\n");
		rtn=1;
	}
	else
	{
		printf("\t\t\t\t��Ŀɾ��ʧ��!\n\t\t\t\t��ȷ�ϼ�����!\n");
	}

	while (getchar() != '\n')
		continue;
	return rtn;
}
 int Play_UI_Query(int id)//��ѯ��Ŀ 
{
	play_t data;
	int rtn=0,i;
	if(Play_Srv_FetchByID(id,&data))
	{
		printf("��ĿID      %d\n", data.id);
		printf("��Ŀ����    %s\n", data.name);
		printf("��Ŀ����    %s\n",data.area);
		i=data.type;
		switch(i)
		{
			case 1:printf("��Ŀ���� %s\n","��Ӱ");break;
			case 2:printf("��Ŀ���� %s\n","���");break;
			case 3:printf("��Ŀ���� %s\n","���ֻ�");break;
		}
		i=data.rating;
		switch(i)
		{
			case 1:printf("��Ŀ�ȼ� %s\n","��ͯ��");break;
			case 2:printf("��Ŀ�ȼ� %s\n","�����");break;
			case 3:printf("��Ŀ�ȼ� %s\n","���˾�");break;
		}
		printf("��Ŀʱ��      %d\n",data.duration);
		printf("��Ŀ��ӳ����  %d %d %d\n",data.start_date.year,data.start_date.month,data.start_date.day);
		printf("��ӳ��������  %d %d %d\n",data.end_date.year,data.end_date.month,data.end_date.day);
		printf("��ĿƱ�� %d\n",data.price);
		rtn=1;
	}
	else{
		printf("û���ҵ��þ�Ŀ����Ϣ!\n");
	}
	printf("�����������!");
	while(getchar() != '\n')continue;
	return rtn;
}
