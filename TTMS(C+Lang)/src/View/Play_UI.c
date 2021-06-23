#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Play_UI_ShowList(play_list_t list,Pagination_t paging)//界面函数 
{
	int i = 0,j;
	char choice;
	play_node_t *pos;

		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t\t\t\t*********剧目信息**********\n");
        printf("\n\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t%4s\t %10s\t %10s\t %3s\t %3s\t %6s\t %10s\t %3s",
			"ID", "名称", "出品地区", "类型", "等级", "时长(分钟)", "放映时间", "票价");
		printf("\n\t\t\t\t------------------------------------------------------------------\n");
		Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i){
			printf("\t\t\t\t%4d\t ", pos->data.id);
			printf("%10s\t ",pos->data.name);
			printf("%10s\t ",pos->data.area);
			j=pos->data.type;
		switch(j){
			case 1:printf(" 电影\t ");break;
			case 2:printf(" 歌剧\t ");break;
			case 3:printf("音乐会\t ");break;
		}
		j=pos->data.rating;
		switch(j){
			case 1:printf("儿童剧\t ");break;
			case 2:printf("少年剧\t ");break;
			case 3:printf("成人剧\t ");break;
		}
		printf("%6d\t\t ",pos->data.duration);
		printf("%4d/%02d/%02d\t ",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);;
		printf("%3d\n",pos->data.price);
		}
		printf("\t\t\t\t------- 共 %2d 项 ----------------------- 第 %2d/%2d 页 ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));

}//载入数据 

void Play_UI_MgtEntry()
{
	int id;
	char choice;
	play_list_t head;
	Pagination_t paging;
	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;
	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	do
	{
		system("cls");
		Play_UI_ShowList(head,paging);
        printf("\n\t\t\t\t******************************************************************\n" );
		printf("\t\t\t\t[S]进入演出计划    [A]添加    [D]删除    [U]修改    [Q]查找    [R]返回\n");
		printf("\t\t\t\t             [P]上一页                       [N]下一页\n"); 
        printf("\t\t\t\t==================================================================\n");
		printf("\t\t\t\t请选择:");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice)
		{
			case 's':
			case 'S':
				printf("\t\t\t\t请输入剧目ID: ");
				scanf("%d", &id);
				while (getchar() != '\n')
					continue;
				Schedule_UI_MgtEntry(id);//进入演出计划 
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
				printf("\t\t\t\t请输入剧目ID:");
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
				printf("\t\t\t\t请输入剧目ID:");
				scanf("%d", &id);
				if (Play_UI_Modify(id))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'q':
			case 'Q':
				printf("\t\t\t\t请输入剧目ID:");
				scanf("%d", &id);
				while (getchar() != '\n')
					continue;
				if (Play_UI_Query(id))
				{	//从新载入数据
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
		printf("\t\t\t\t添加新的剧目信息\n");
        printf("\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t请输入剧目名称:");
		s_gets(data.name, 30);
		printf("\t\t\t\t请输入剧目产地:");
		s_gets(data.area, 30);
		printf("\t\t\t\t请输入剧目时长:");
		scanf("%d",&data.duration);
		while (getchar() != '\n')
			continue;
		printf("\t\t\t\t请输入剧目类型:\n");
		printf("\t\t\t\t1.电影 2.歌剧 3.音乐会"); 
		scanf("%d",&type);
		while (getchar() != '\n')
			continue;
		if(type==1 || type==2 || type==3)
		{
			data.type=(play_type_t)type;
		}
		printf("\t\t\t\t请输入适应人群:\n");
		printf("\t\t\t\t1.儿童 2.青年 3.成人");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3)
		{
			data.rating=(play_rating_t)rating;
		}
		printf("\t\t\t\t请输入开始放映日期:");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		while (getchar() != '\n')
			continue;
		printf("\t\t\t\t请输入放映结束日期:");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		while (getchar() != '\n')
			continue;
		printf("\t\t\t\t请输入票价:");
		scanf("%d",&data.price);
		while (getchar() != '\n')
			continue;
        printf("\n\t\t\t\t==================================================================\n");

		//获取主键
		data.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&data)) {
			newRecCount += 1;
			printf("\t\t\t\t新剧目添加成功!\n");
		} else
			printf("\t\t\t\t新剧目添加失败!\n");
        printf("\t\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t\t***************************[A]添加*******[R]返回******************\n");
		fflush(stdin);
		choice = toupper(getchar());

		while (getchar() != '\n')
			continue;
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}
 int Play_UI_Modify(int id)//修改剧目 
{	int type,rating,i;
	play_t data;
	int rtn=0;
	char newname[31],newarea[9];
	int newduration,newprice,newSyear,newSmonth,newSday,newEyear,newEmonth,newEday,newrate,newtype;
	if (!Play_Srv_FetchByID(id, &data))
	{
		printf("\t\t\t\t剧目不存在,按确认键返回\n");
		while(getchar() != '\n')continue;
		return 0;
	}
    printf("\n\t\t\t\t==================================================================\n");
	printf("\t\t\t\t修改剧目信息\n");
    printf("------------------------------------------------------------------\n");
	printf("剧目ID      %d\n", data.id);
	printf("剧目名称    %s\n", data.name);
	printf("剧目产地    %s\n",data.area);
	i=data.type;
	switch(i)
	{
		case 1:printf("剧目类型 %s\n","电影");break;
		case 2:printf("剧目类型 %s\n","歌剧");break;
		case 3:printf("剧目类型 %s\n","音乐会");break;
	}
	i=data.rating;
	switch(i)
	{
		case 1:printf("剧目等级 %s\n","儿童剧");break;
		case 2:printf("剧目等级 %s\n","少年剧");break;
		case 3:printf("剧目等级 %s\n","成人剧");break;
	}
	printf("剧目时长      %d\n",data.duration);
	printf("剧目放映日期  %d %d %d\n",data.start_date.year,data.start_date.month,data.start_date.day);
	printf("放映结束日期  %d %d %d\n",data.end_date.year,data.end_date.month,data.end_date.day);
	printf("剧目票价 %d\n",data.price);
    printf("------------------------------------------------------------------\n");
	fflush(stdin);
	if(Play_Srv_FetchByID(id,&data))//查找需要修改的剧目 
	{

		printf("请输入剧目名称:");
		s_gets(data.name, 30);
		printf("请输入剧目产地:");
		s_gets(data.name, 30);
		printf("请输入剧目时长:");
		scanf("%d",&newduration);
		while (getchar() != '\n')
			continue;
		data.duration=newduration;
		printf("请输入剧目类型\n:");
		printf("1.电影 2.歌剧 3.音乐会"); 
		scanf("%d",&newtype);
		while (getchar() != '\n')
			continue;
		if(newtype==1 || newtype==2 || newtype==3)
		{
			data.type=(play_type_t)newtype;
		}
		printf("请输入适应人群:\n");
		printf("1.儿童 2.青年 3.成人");
		scanf("%d",&newrate);
		while (getchar() != '\n')
			continue;
		if(newrate==1 || newrate==2 || newrate==3)
		{
			data.rating=(play_rating_t)newrate;
		}
		printf("请输入开始放映日期:");
		while (getchar() != '\n')
			continue;
		scanf("%d %d %d",&newSyear,&newSmonth,&newSday);
		data.start_date.year=newSyear;
		data.start_date.month=newSmonth;
		data.start_date.day=newSday;
		printf("请输入放映结束日期:");
		while (getchar() != '\n')
			continue;
		scanf("%d %d %d",&newEyear,&newEmonth,&newEday);
		data.end_date.year=newEyear;
		data.end_date.month=newEmonth;
		data.end_date.day=newEday;
		printf("请输入票价:");
		while (getchar() != '\n')
			continue;
		scanf("%d",&newprice);
		data.price=newprice;
	}
    printf("------------------------------------------------------------------\n");

	if (Play_Srv_Modify(&data))
	{
		rtn = 1;
		printf("\t\t\t\t剧目修改成功!\n\t\t\t\t按确认键返回!\n");
	}
	else
	{
		printf("\t\t\t\t剧目修改失败!\n\t\t\t\t按确认键返回!\n");
	}
	while(getchar() != '\n')continue;
	return rtn;
}
int Play_UI_Delete(int id)//删除剧目 
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
	if (Play_Srv_DeleteByID(id))
	{
		printf("\t\t\t\t剧目成功删除!\n\t\t\t\t按确认键继续!\n");
		rtn=1;
	}
	else
	{
		printf("\t\t\t\t剧目删除失败!\n\t\t\t\t按确认键继续!\n");
	}

	while (getchar() != '\n')
		continue;
	return rtn;
}
 int Play_UI_Query(int id)//查询剧目 
{
	play_t data;
	int rtn=0,i;
	if(Play_Srv_FetchByID(id,&data))
	{
		printf("剧目ID      %d\n", data.id);
		printf("剧目名称    %s\n", data.name);
		printf("剧目产地    %s\n",data.area);
		i=data.type;
		switch(i)
		{
			case 1:printf("剧目类型 %s\n","电影");break;
			case 2:printf("剧目类型 %s\n","歌剧");break;
			case 3:printf("剧目类型 %s\n","音乐会");break;
		}
		i=data.rating;
		switch(i)
		{
			case 1:printf("剧目等级 %s\n","儿童剧");break;
			case 2:printf("剧目等级 %s\n","少年剧");break;
			case 3:printf("剧目等级 %s\n","成人剧");break;
		}
		printf("剧目时长      %d\n",data.duration);
		printf("剧目放映日期  %d %d %d\n",data.start_date.year,data.start_date.month,data.start_date.day);
		printf("放映结束日期  %d %d %d\n",data.end_date.year,data.end_date.month,data.end_date.day);
		printf("剧目票价 %d\n",data.price);
		rtn=1;
	}
	else{
		printf("没有找到该剧目的信息!\n");
	}
	printf("按任意键返回!");
	while(getchar() != '\n')continue;
	return rtn;
}
