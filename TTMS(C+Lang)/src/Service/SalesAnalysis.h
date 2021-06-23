#pragma once

#include "../Common/Common.h"
#include "../Service/Play.h"
#include "../Service/Sale.h"
#include "../Common/List.h"

typedef struct {
	int play_id; 	//剧目编号
	char name[31]; 	//剧目名称
	char area[9]; 	//剧目区域
	int duration; 	//以分钟为单位
	long totaltickets; //剧目票销售数量
	long sales; 	//电影票房
	int price; 		//剧目票价
	user_date_t start_date; 	//剧目上映日期
	user_date_t end_date; 	//剧目下映日期
} salesanalysis_t;


typedef struct salesanalysis_node {
	salesanalysis_t data;
	struct salesanalysis_node *next, *prev;
} salesanalysis_node_t, *salesanalysis_list_t;



int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate);


int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list);


void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list);

void SalesAnalysis_Srv_AddToSortedList(salesanalysis_list_t list, salesanalysis_node_t* node);

