#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/List.h"
#include "SalesAnalysis.h"


int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate)
{
	int amount = 0;
	sale_list_t saleList;
	List_Init(saleList, sale_node_t);
	sale_node_t* pSale;
	Sale_Perst_SelByUsrID(saleList, usrID);
	List_ForEach(saleList, pSale)
	{
		if (DateCmp(pSale->data.date, stDate) >= 0 && DateCmp(pSale->data.date, endDate) <= 0)
			amount += pSale->data.value;
	}
	return amount;
}



int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list)
{
	assert(list != NULL);

	int count = 0;
	int sold = 0;
	int total;
	play_list_t playList;
	play_node_t* pos;


	List_Free(list, salesanalysis_node_t);

	List_Init(playList, play_node_t);

	Play_Srv_FetchAll(playList);
	List_ForEach(playList, pos)
	{

		user_date_t curDate = DateNow();

		if (DateCmp(pos->data.start_date, curDate) <= 0)	//找到上映剧目
		{
			salesanalysis_node_t* newNode =
				(salesanalysis_node_t*)malloc(sizeof(salesanalysis_node_t));//建立新块， 录入数据
			newNode->data.duration = pos->data.duration;
			newNode->data.start_date = pos->data.start_date;
			newNode->data.end_date = pos->data.end_date;
			strcpy(newNode->data.name, pos->data.name);

			newNode->data.price = pos->data.price;
			newNode->data.play_id = pos->data.id;
			strcpy(newNode->data.area, pos->data.area);
			//还差sales和totaltickets
			newNode->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id, &sold);
			newNode->data.totaltickets = sold;

			//创建
			List_AddTail(list, newNode);
			count++;
		}
	}
	List_Destroy(playList, play_node_t);
	return count;
}



void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list)
{
	if (list->next == list)
		return;
	(list->prev)->next = NULL;
	list->prev = NULL;
	salesanalysis_node_t* listLeft = list->next;		//接盘
	salesanalysis_node_t* pos;
	list->prev = list;
	list->next = list;
	while (listLeft != NULL)
	{
		pos = listLeft;
		listLeft = listLeft->next;
		SalesAnalysis_Srv_AddToSortedList(list, pos);
	}

	return;
}


void SalesAnalysis_Srv_AddToSortedList(salesanalysis_list_t list, salesanalysis_node_t* node)
{
	if (list->next == list)
	{
		List_AddTail(list, node);
		return;
	}

	salesanalysis_node_t* curpos;
	List_ForEach(list, curpos)
	{
		if (node->data.sales > curpos->data.sales)
		{
			List_InsertBefore(curpos, node);
			return;
		}
	}
	if (curpos == list)
	{
		List_AddTail(list, node);
		return;
	}
}

