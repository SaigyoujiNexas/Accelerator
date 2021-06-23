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

//定义分页机制中页面大小为5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//剧院销售排行榜函数,降序显示截止目前剧院电影票房排行榜
void SalesAnalysis_UI_BoxOffice() {
	int i;
	char choice;

	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;

	List_Init(head, salesanalysis_node_t);
	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);   //根据票房排序

	Paging_Locate_FirstPage(head, paging);
	do{
		system("cls");
		printf("\t\t\t\t=========================================================================\n");
		printf("\t\t\t\t票房图表\n");
		printf("\t\t\t\t-------------------------------------------------------------------------\n");
		printf("剧目名\t\t区域\t\t售票数\t票房\t上映时间\t\t下映时间\n");

			for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize && i < paging.totalRecords; i++) {
			printf("\t\t\t\t%-10s\t%-10s\t%-5ld\t%-5ld\t%d-%d-%d\t%d-%d-%d\t\n",
					pos->data.name, pos->data.area, pos->data.totaltickets,pos->data.sales,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t\t---------- 共 %2d 项 --------------------------- 第 %2d/%2d 页 --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t\t*************************************************************************\n");
		printf("[P]上一页	|	[N]下一页	|	[R]返回\n");
		printf("\n\t\t\t\t=========================================================================\n");
		fflush(stdin);
		printf("\t\t\t\t请选择功能：");
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

//显示售票员在给定日期区间的售票情况
void StaSales_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate)
{    //日期参数是用户输入还是？
	account_t usr;
	int sale_count=0;
	sale_count=Salesanalysis_Srv_CompSaleVal(usrID,stDate,endDate);    //获取销售员的销售额

	if(!Account_Srv_FetchByID(usrID,&usr)) {
		printf("\t\t\tID不存在!\n请按回车结束\n");
		while (getchar() != '\n')
			continue;
		return ;
	}

	printf("\t\t\t================售票员%d的销售情况===================\n",usrID);
	printf("\t\t\t销售开始日期:     %d-%d-%d\n",stDate.year,stDate.month,stDate.day);
	printf("\t\t\t销售截至日期:     %d-%d-%d\n",endDate.year,endDate.month,endDate.day);
	printf("\t\t\t销售额：%d\n",sale_count);
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
	printf("\t\t\t\t[D] 统计当日销售额		|	[M]统计当月销售额		|	其余输入退出");
	choice = toupper(getchar());
	while (getchar() != '\n')
		continue;
	if (choice == 'D')
	{
		StaSales_UI_StatSale(id, curdate, curdate);
		printf("\n按任意键返回\n");
		while (getchar() != '\n')
			continue;
	}
	else if (choice == 'M')
	{
		StaSales_UI_StatSale(id, startdate, enddate);
		printf("\n按任意键返回\n");
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
		printf("\t\t\t\t[P]上一页		|	 [N]下一页	|	 [T]选择员工		|	  [R]退出");
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
			printf("\t\t\t\t请输入售票员名称：");
			s_gets(Username, 30);
			if (Account_Srv_FetchByName(Username, &user) == 0)
			{
				printf("\t\t\t\t员工不存在！[按任意键返回]");
				break;
			}
			id = user.id;
			printf("\t\t\t\t请输入时间区间\n");
			printf("\t\t\t\t开始时间：");
			scanf("%d %d %d", &startdate.year,
				&startdate.month, &startdate.day);
			while (getchar() != '\n')
				continue;
			printf("\t\t\t\t截止时间：");
			scanf("%d %d %d", &enddate.year,
				&enddate.month, &enddate.day);
			while (getchar() != '\n')
				continue;
			system("cls");
			StaSales_UI_StatSale(id, startdate, enddate);
			printf("\n\t\t\t\t按任意键返回\n");
			while (getchar() != '\n')
				continue;
			break;
		}
	} while (choice != 'R');
	List_Destroy(clerk, account_node_t);
}


