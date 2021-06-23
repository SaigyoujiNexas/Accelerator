#include "StaSales_UI.h"
#include "../Common/List.h"
#include "../Common/Common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "../View/Account_UI.h"
extern const account_t* User;

extern struct month months[12];

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_BoxOffice() {
	int i;
	char choice;

	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;

	List_Init(head, salesanalysis_node_t);
	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//��������
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);   //����Ʊ������

	Paging_Locate_FirstPage(head, paging);
	do{
		system("cls");
		printf("\t\t\t\t=========================================================================\n");
		printf("\t\t\t\tƱ��ͼ��\n");
		printf("\t\t\t\t-------------------------------------------------------------------------\n");
		printf("��Ŀ��\t\t����\t\t��Ʊ��\tƱ��\t��ӳʱ��\t\t��ӳʱ��\n");

			for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize && i < paging.totalRecords; i++) {
			printf("\t\t\t\t%-10s\t%-10s\t%-5ld\t%-5ld\t%d-%d-%d\t%d-%d-%d\t\n",
					pos->data.name, pos->data.area, pos->data.totaltickets,pos->data.sales,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t\t---------- �� %2d �� --------------------------- �� %2d/%2d ҳ --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t\t*************************************************************************\n");
		printf("[P]��һҳ	|	[N]��һҳ	|	[R]����\n");
		printf("\n\t\t\t\t=========================================================================\n");
		fflush(stdin);
		printf("\t\t\t\t��ѡ���ܣ�");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;

		switch(choice)
		{
			case 'P':
			case 'p':
					if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
					}
				break;
			case 'N':
			case 'n':
					if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
					}
				break;
		}
	} while(choice!='R' && choice!='r');
		List_Destroy(head, salesanalysis_node_t);
}

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void StaSales_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate)
{    //���ڲ������û����뻹�ǣ�
	account_t usr;
	int sale_count=0;
	sale_count=Salesanalysis_Srv_CompSaleVal(usrID,stDate,endDate);    //��ȡ����Ա�����۶�

	if(!Account_Srv_FetchByID(usrID,&usr)) {
		printf("\t\t\tID������!\n�밴�س�����\n");
		while (getchar() != '\n')
			continue;
		return ;
	}

	printf("\t\t\t================��ƱԱ%d���������===================\n",usrID);
	printf("\t\t\t���ۿ�ʼ����:     %d-%d-%d\n",stDate.year,stDate.month,stDate.day);
	printf("\t\t\t���۽�������:     %d-%d-%d\n",endDate.year,endDate.month,endDate.day);
	printf("\t\t\t���۶%d\n",sale_count);
	printf("\t\t\t======================================================\n");

}

void StaSales_UI_Self()
{
	int count;
	char choice;
	int id = User->id;
	user_date_t curdate, startdate, enddate;
	curdate = DateNow();
	enddate = (startdate = curdate);
	startdate.day = 1;
	enddate.day = months[curdate.month].days;
	system("cls");
	printf("\t\t\t\t[D] ͳ�Ƶ������۶�		|	[M]ͳ�Ƶ������۶�		|	���������˳�");
	choice = toupper(getchar());
	while (getchar() != '\n')
		continue;
	if (choice == 'D')
	{
		StaSales_UI_StatSale(id, curdate, curdate);
		printf("\n�����������\n");
		while (getchar() != '\n')
			continue;
	}
	else if (choice == 'M')
	{
		StaSales_UI_StatSale(id, startdate, enddate);
		printf("\n�����������\n");
		while (getchar() != '\n')
			continue;
	}
	return;
}

void StaSales_UI_Clerk()
{
	int count;
	int id;
	user_date_t startdate, enddate;
	char Username[30];
	account_t user;
	char choice;
	Pagination_t paging;
	account_list_t clerk;
	paging.pageSize = ACCOUNT_PAGE_SIZE;
	List_Init(clerk, account_node_t);
	paging.totalRecords = Account_Srv_FetchByType(clerk, USR_CLERK);
	Paging_Locate_FirstPage(clerk, paging);
	do
	{
		Account_UI_ShowList(clerk, paging);
		printf("\t\t\t\t[P]��һҳ		|	 [N]��һҳ	|	 [T]ѡ��Ա��		|	  [R]�˳�");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
		case 'P':
			if (!Pageing_IsFirstPage(paging))
				Paging_Locate_OffsetPage(clerk, paging, -1, account_node_t);
			break;
		case'N':
			if (!Pageing_IsLastPage(paging))
				Paging_Locate_OffsetPage(clerk, paging, 1, account_node_t);
			break;
		case 'T':
			printf("\t\t\t\t��������ƱԱ���ƣ�");
			s_gets(Username, 30);
			if (Account_Srv_FetchByName(Username, &user) == 0)
			{
				printf("\t\t\t\tԱ�������ڣ�[�����������]");
				break;
			}
			id = user.id;
			printf("\t\t\t\t������ʱ������\n");
			printf("\t\t\t\t��ʼʱ�䣺");
			scanf("%d %d %d", &startdate.year,
				&startdate.month, &startdate.day);
			while (getchar() != '\n')
				continue;
			printf("\t\t\t\t��ֹʱ�䣺");
			scanf("%d %d %d", &enddate.year,
				&enddate.month, &enddate.day);
			while (getchar() != '\n')
				continue;
			system("cls");
			StaSales_UI_StatSale(id, startdate, enddate);
			printf("\n\t\t\t\t�����������\n");
			while (getchar() != '\n')
				continue;
			break;
		}
	} while (choice != 'R');
	List_Destroy(clerk, account_node_t);
}


