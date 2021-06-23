#include"Sale_Persist.h"
#include"Ticket_Persist.h"
#include "../Service/Sale.h"
#include "Studio_Persist.h"
#include "../Service/studio.h"
#include "EntityKey_Persist.h"	 
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>
static const char SALE_DATA_FILE[] = "Sale.dat"; //文件名常量 
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat"; //临时文件名常量 
static const char SALE_KEY_NAME[] = "Sale"; //名常量 
//存储新订单信息
int Ticket_Perst_SelByID(int id,ticket_t *buf){
	assert(NULL!=buf);
	long key = EntKey_Perst_GetNewKeys(SALE_KEY_NAME, 1); //为新演出厅分配获取
	if(key<=0)			//主键分配失败，直接返回
		return 0;
	buf->id = key;		//赋给新对象带回到UI层
	FILE *fp = fopen(SALE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("打开文件失败%s!\n", SALE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(buf, sizeof(sale_t), 1, fp);

	fclose(fp);
	return rtn;
}
int Sale_Perst_Insert(const sale_t *data){
	assert(NULL!=data);
	FILE *fp=fopen(SALE_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp)
	{
		printf("打开文件失败 %s!\n", SALE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}
	rtn=fwrite(data,sizeof(sale_t),1,fp);
	fclose(fp);
	return rtn;
}

int Sale_Perst_SelectByTicketID(int ticket_id, sale_t* sale)
{
	int found = 0;
	FILE* fp;
	sale_t temp;
	if (_access(SALE_DATA_FILE, _A_NORMAL))
	{
		printf("无法打开文件%s！", SALE_DATA_FILE);
		return 0;
	}
	if ((fp = fopen(SALE_DATA_FILE, "rb")) == NULL)
	{
		printf("无法打开文件%s！", SALE_DATA_FILE);
		return 0;
	}
	while (!feof(fp))
	{
		fread(&temp, sizeof(sale_t), 1, fp);
		if (temp.ticket_id == ticket_id)
		{
			found = 1;
			*sale = temp;
		}
	}
	fclose(fp);
	return found;
}



int Sale_Perst_SelectAll(sale_list_t list) {
	sale_node_t* newNode;
	sale_t data;
	int recCount = 0;

	assert(NULL != list);

	if (_access(SALE_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", SALE_DATA_FILE);
		return 0;
	}

	List_Free(list, sale_node_t);

	FILE* fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SALE_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(sale_t), 1, fp))
		{
			newNode = (sale_node_t*)malloc(sizeof(sale_node_t));
			if (!newNode)
			{
				printf("文件打开失败\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}
int Sale_Perst_SelByUsrID(sale_list_t list, int usrID)
{
	int count = 0;
	List_Free(list, sale_node_t);
	sale_t temp;
	FILE* fp;
	assert(list != NULL);
	if (_access(SALE_DATA_FILE, _A_NORMAL))
	{
		printf("无法打开文件%s![按任意键继续]", SALE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}
	if ((fp = fopen(SALE_DATA_FILE, "rb")) == NULL)
	{
		printf("无法打开文件%s![按任意键继续]", SALE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}
	while (!feof(fp))
	{
		fread(&temp, sizeof(sale_t), 1, fp);
		if (temp.user_id == usrID)		//ID匹配
		{
			sale_node_t* newNode = (sale_node_t*)malloc(sizeof(sale_node_t));
			newNode->data = temp;
			List_AddTail(list, newNode);
		}
	}
	return count;
}
